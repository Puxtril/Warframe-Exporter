#include "model/ModelExporterGltf.h"

using namespace WarframeExporter::Model;

void
ModelExporterGltf::addModelData(Document& gltfDoc, const ModelHeaderInternal& header, const ModelBodyInternal& body)
{
	if (header.boneTree.size() > 0)
		_addModelDataRigged(gltfDoc, header, body);
	else
		_addModelDataStatic(gltfDoc, header, body);
}

void
ModelExporterGltf::_addModelDataRigged(Document& gltfDoc, const ModelHeaderInternal& header, const ModelBodyInternal& body)
{
	_modifyAsset(gltfDoc);

	Attributes vertsAttrs = _addVertexDataRigged(gltfDoc, body, header.vertexCount);
	unsigned int indicesBufViewIndex = _addIndexData(gltfDoc, body.indices);
	std::vector<int32_t> meshIndices = _createMeshes(gltfDoc, header.meshInfos, vertsAttrs, indicesBufViewIndex);

	int32_t rootBoneNodeIndex = _createBones(gltfDoc, header.boneTree);
	int32_t inverseMatricesIndex = _addInverseBindMatrices(gltfDoc, header.boneTree, header.weightedBones);
	int32_t skinIndex = _createSkin(gltfDoc, header.weightedBones, (int)header.boneTree.size(), "Skeleton", rootBoneNodeIndex, inverseMatricesIndex);

	_createSceneWithModelNodes(gltfDoc, meshIndices, skinIndex);
	gltfDoc.scenes[0].nodes.push_back(rootBoneNodeIndex);
}

void
ModelExporterGltf::_addModelDataStatic(Document& gltfDoc, const ModelHeaderInternal& header, const ModelBodyInternal& body)
{
	_modifyAsset(gltfDoc);

	Attributes vertsAttrs = _addVertexDataStatic(gltfDoc, body, header.vertexCount);
	int32_t indicesBufViewIndex = _addIndexData(gltfDoc, body.indices);
	std::vector<int32_t> meshIndices = _createMeshes(gltfDoc, header.meshInfos, vertsAttrs, indicesBufViewIndex);

	_createSceneWithModelNodes(gltfDoc, meshIndices, -1);
}

void
ModelExporterGltf::save(Document& gltfDoc, const std::filesystem::path& outPath)
{
	try
	{
		Save(gltfDoc, outPath, true);
	}
	catch (std::exception& ex)
	{
		_print_exception(ex);
	}
}

void
ModelExporterGltf::_print_exception(const std::exception& e, int level)
{
	WarframeExporter::Logger::getInstance().error(std::string(level, ' ') + e.what());
	try {
		std::rethrow_if_nested(e);
	}
	catch (const std::exception& nestedException) {
		_print_exception(nestedException, level + 1);
	}
	catch (...) {}
}

void
ModelExporterGltf::_createSceneWithModelNodes(Document& gltfDoc, const std::vector<int32_t>& meshes, int32_t skinIndex)
{
	if (gltfDoc.scenes.size() == 0)
	{
		Scene scene;
		gltfDoc.scenes.resize(1);
		gltfDoc.scene = 0;
	}
	Scene& scene = gltfDoc.scenes[0];

	for (size_t x = 0; x < meshes.size(); x++)
	{
		Node curNode;
		curNode.mesh = meshes[x];
		curNode.skin = skinIndex;
		int curNodeIndex = static_cast<int>(gltfDoc.nodes.size());
		gltfDoc.nodes.push_back(curNode);
		scene.nodes.push_back(curNodeIndex);
	}
}

int32_t
ModelExporterGltf::_createBones(Document& gltfDoc, const std::vector<BoneTreeNodeInternal>& boneTree)
{
	int32_t rootIndex = -1;
	if (boneTree.size() > 0)
		rootIndex = static_cast<int32_t>(gltfDoc.nodes.size());
	for (uint16_t x = 0; x < static_cast<uint16_t>(boneTree.size()); x++)
	{
		Node boneNode;
		boneNode.name = boneTree[x].name;
		_findChildrenOfBone(boneTree, x, boneNode.children);

		std::memcpy(&boneNode.rotation[0], &boneTree[x].rotation[0], sizeof(float) * 4);
		std::memcpy(&boneNode.translation[0], &boneTree[x].position[0], sizeof(float) * 3);

		gltfDoc.nodes.push_back(boneNode);
	}
	return rootIndex;
}

int32_t
ModelExporterGltf::_createSkin(Document& gltfDoc, const std::vector<int32_t>& weightedIndices, int totalBoneCount, const std::string& skinName, int32_t rootBoneIndex, int32_t inverseBindMatricesIndex)
{
	Skin skin;
	int32_t skinIndex = static_cast<int32_t>(gltfDoc.skins.size());
	skin.name = skinName;
	skin.skeleton = rootBoneIndex;
	std::vector<bool> addedBones(totalBoneCount, false);
	for (int x = 0; x < (int)weightedIndices.size(); x++)
	{
		skin.joints.push_back(static_cast<int>(weightedIndices[x]) + rootBoneIndex);
		addedBones[weightedIndices[x]] = true;
	}
	for (int x = 0; x < (int)totalBoneCount; x++)
	{
		if (!addedBones[x])
			skin.joints.push_back(x + rootBoneIndex);
	}

	skin.inverseBindMatrices = inverseBindMatricesIndex;
	gltfDoc.skins.push_back(skin);
	return skinIndex;
}

int32_t
ModelExporterGltf::_addInverseBindMatrices(Document& gltfDoc, const std::vector<BoneTreeNodeInternal>& boneTree, const std::vector<int32_t>& weightedIndices)
{
	if (gltfDoc.buffers.size() == 0)
		_createBuffer(gltfDoc);

	Buffer& buf = gltfDoc.buffers[0];
	uint32_t matSize = static_cast<uint32_t>(sizeof(boneTree[0].reverseBind));
	uint32_t byteLen = static_cast<uint32_t>(boneTree.size()) * matSize;
	byteLen += (byteLen % 4);
	uint32_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	buf.byteLength += byteLen;
	uint8_t* curPos = buf.data.data() + startOffset;

	std::vector<bool> addedBones(boneTree.size(), false);
	for (uint32_t x = 0; x < weightedIndices.size(); x++)
	{
		int boneTreeIndex = weightedIndices[x];
		std::memcpy(curPos, glm::value_ptr(boneTree[boneTreeIndex].reverseBind), matSize);
		curPos += matSize;
		addedBones[boneTreeIndex] = true;
	}
	for (uint32_t x = 0; x < boneTree.size(); x++)
	{
		if (!addedBones[x])
		{
			std::memcpy(curPos, glm::value_ptr(boneTree[x].reverseBind), matSize);
			curPos += matSize;
		}
	}

	BufferView bufView;
	int32_t bufViewIndex = static_cast<int32_t>(gltfDoc.bufferViews.size());
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.byteStride = 0;
	gltfDoc.bufferViews.push_back(bufView);

	Accessor matAcc;
	int32_t matAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
	matAcc.bufferView = bufViewIndex;
	matAcc.byteOffset = 0;
	matAcc.count = (uint32_t)boneTree.size();
	matAcc.type = Accessor::Type::Mat4;
	matAcc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(matAcc);

	return matAccIndex;
}

std::vector<int32_t>
ModelExporterGltf::_createMeshes(Document& gltfDoc, const std::vector<MeshInfoInternal>& meshInfos, Attributes attrs, int32_t indicesBuffViewIndex)
{
	Mesh mesh;
	std::vector<int32_t> meshIndices;

	for (size_t x = 0; x < meshInfos.size(); x++)
	{
		Accessor curAcc;
		int32_t curAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
		curAcc.bufferView = indicesBuffViewIndex;
		curAcc.byteOffset = meshInfos[x].faceLODOffsets[0] * 2;
		curAcc.count = meshInfos[x].faceLODCounts[0];
		curAcc.type = Accessor::Type::Scalar;
		curAcc.componentType = Accessor::ComponentType::UnsignedShort;
		gltfDoc.accessors.push_back(curAcc);

		Primitive curPrim;
		curPrim.indices = curAccIndex;
		curPrim.material = _findOrCreateMaterial(gltfDoc, meshInfos[x].matName);
		curPrim.mode = Primitive::Mode::Triangles;
		curPrim.attributes = attrs;

		Mesh curMesh;
		int32_t curMeshIndex = static_cast<int32_t>(gltfDoc.meshes.size());
		curMesh.name = meshInfos[x].name;
		curMesh.primitives.push_back(curPrim);
		gltfDoc.meshes.push_back(curMesh);
		
		meshIndices.push_back(curMeshIndex);
	}

	return meshIndices;
}

int32_t
ModelExporterGltf::_findOrCreateMaterial(Document& gltfDoc, const std::string& materialPath)
{
	std::string materialName = materialPath;
	size_t lastSlashOffset = materialPath.find_last_of("/");
	if (lastSlashOffset != std::string::npos)
		materialName = materialPath.substr(lastSlashOffset + 1);

	for (int32_t x = 0; x < (int32_t)gltfDoc.materials.size(); x++)
	{
		if (gltfDoc.materials[x].name.compare(materialName) == 0)
			return x;
	}
	
	int32_t matIndex = static_cast<int32_t>(gltfDoc.materials.size());
	Material mat;
	mat.name = materialName;
	mat.doubleSided = true;
	mat.extensionsAndExtras["extras"]["FullPath"] = materialPath;
	gltfDoc.materials.push_back(mat);

	return matIndex;
}

// Color must be in the format VEC4
// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes-overview
Attributes
ModelExporterGltf::_addVertexDataRigged(Document& gltfDoc, const ModelBodyInternal& body, int vertCount)
{
	if (gltfDoc.buffers.size() == 0)
		_createBuffer(gltfDoc);

	Buffer& buf = gltfDoc.buffers[0];
	// 1 colorLen is already factored into this vertex size
	// Add the other 2
	uint32_t byteLen = vertCount * body.vertexSizeRigged();
	byteLen += (byteLen % 4);
	uint32_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	buf.byteLength += byteLen;
	uint8_t* curPos = buf.data.data() + startOffset;

	for (int iVert = 0; iVert < vertCount; iVert++)
	{
		memcpy(curPos, &body.positions[iVert].x, ModelBodyInternal::positionLen);
		curPos += ModelBodyInternal::positionLen;
		memcpy(curPos, &body.UV1[iVert].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
		memcpy(curPos, &body.UV2[iVert].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
		for (size_t x = 0; x < body.colors.size(); x++)
		{
			memcpy(curPos, &body.colors[x][iVert], ModelBodyInternal::colorLen);
			curPos += ModelBodyInternal::colorLen;
		}
		memcpy(curPos, &body.boneIndices[iVert].x, ModelBodyInternal::boneIndexLen);
		curPos += ModelBodyInternal::boneIndexLen;
		memcpy(curPos, &body.boneWeights[iVert].x, ModelBodyInternal::boneWeightLen);
		curPos += ModelBodyInternal::boneWeightLen;
	}
	
	BufferView bufView;
	int32_t bufViewIndex = static_cast<int32_t>(gltfDoc.bufferViews.size());
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.byteStride = body.vertexSizeRigged();
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);
	
	uint32_t curByteOffset = 0;

	Accessor posAcc;
	int32_t posAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = curByteOffset;
	posAcc.count = vertCount;
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	posAcc.max = _findMaxVec3(body.positions);
	posAcc.min = _findMinVec3(body.positions);
	gltfDoc.accessors.push_back(posAcc);
	curByteOffset += ModelBodyInternal::positionLen;
	
	Accessor uv1Acc;
	int32_t uv1AccIndex = (int32_t)gltfDoc.accessors.size();
	uv1Acc.bufferView = bufViewIndex;
	uv1Acc.byteOffset = curByteOffset;
	uv1Acc.count = vertCount;
	uv1Acc.type = Accessor::Type::Vec2;
	uv1Acc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(uv1Acc);
	curByteOffset += ModelBodyInternal::UVLen;

	Accessor uv2Acc;
	int32_t uv2AccIndex = (int32_t)gltfDoc.accessors.size();
	uv2Acc.bufferView = bufViewIndex;
	uv2Acc.byteOffset = curByteOffset;
	uv2Acc.count = vertCount;
	uv2Acc.type = Accessor::Type::Vec2;
	uv2Acc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(uv2Acc);
	curByteOffset += ModelBodyInternal::UVLen;

	std::vector<int32_t> vertexColAccIndices;
	for (size_t x = 0; x < body.colors.size(); x++)
	{
		Accessor colAcc;
		vertexColAccIndices.push_back((int32_t)gltfDoc.accessors.size());
		colAcc.bufferView = bufViewIndex;
		colAcc.byteOffset = curByteOffset;
		colAcc.count = vertCount;
		colAcc.type = Accessor::Type::Vec4;
		colAcc.componentType = Accessor::ComponentType::UnsignedByte;
		colAcc.normalized = true;
		gltfDoc.accessors.push_back(colAcc);
		curByteOffset += ModelBodyInternal::colorLen;
	}

	Accessor boneIndexAcc;
	int32_t boneIndexAccIndex = (int32_t)gltfDoc.accessors.size();
	boneIndexAcc.bufferView = bufViewIndex;
	boneIndexAcc.byteOffset = curByteOffset;
	boneIndexAcc.count = vertCount;
	boneIndexAcc.type = Accessor::Type::Vec4;
	boneIndexAcc.componentType = Accessor::ComponentType::UnsignedShort;
	gltfDoc.accessors.push_back(boneIndexAcc);
	curByteOffset += ModelBodyInternal::boneIndexLen;

	Accessor boneWeightAcc;
	int32_t boneWeightAccIndex = (int32_t)gltfDoc.accessors.size();
	boneWeightAcc.bufferView = bufViewIndex;
	boneWeightAcc.byteOffset = curByteOffset;
	boneWeightAcc.count = vertCount;
	boneWeightAcc.type = Accessor::Type::Vec4;
	boneWeightAcc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(boneWeightAcc);

	Attributes attrs;
	attrs["POSITION"] = posAccIndex;
	attrs["TEXCOORD_0"] = uv1AccIndex;
	attrs["TEXCOORD_1"] = uv2AccIndex;
	for (size_t x = 0; x < vertexColAccIndices.size(); x++)
		attrs["COLOR_" + std::to_string(x)] = vertexColAccIndices[x];
	attrs["JOINTS_0"] = boneIndexAccIndex;
	attrs["WEIGHTS_0"] = boneWeightAccIndex;

	return attrs;
}

// Color must be in the format VEC4
// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes-overview
Attributes
ModelExporterGltf::_addVertexDataStatic(Document& gltfDoc, const ModelBodyInternal& body, int vertCount)
{
	if (gltfDoc.buffers.size() == 0)
		_createBuffer(gltfDoc);

	Buffer& buf = gltfDoc.buffers[0];
	uint32_t byteLen = vertCount * body.vertexSizeStatic();
	uint32_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	buf.byteLength += byteLen;
	uint8_t* curPos = buf.data.data() + startOffset;

	for (int iVert = 0; iVert < vertCount; iVert++)
	{
		memcpy(curPos, &body.positions[iVert].x, ModelBodyInternal::positionLen);
		curPos += ModelBodyInternal::positionLen;
		memcpy(curPos, &body.UV1[iVert].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
		memcpy(curPos, &body.UV2[iVert].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
		for (size_t x = 0; x < body.colors.size(); x++)
		{
			memcpy(curPos, &body.colors[x][iVert], ModelBodyInternal::colorLen);
			curPos += ModelBodyInternal::colorLen;
		}
	}

	BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.byteStride = body.vertexSizeStatic();
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

	uint32_t curByteOffset = 0;

	Accessor posAcc;
	int32_t posAccIndex = (int32_t)gltfDoc.accessors.size();
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = curByteOffset;
	posAcc.count = vertCount;
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	posAcc.max = _findMaxVec3(body.positions);
	posAcc.min = _findMinVec3(body.positions);
	gltfDoc.accessors.push_back(posAcc);
	curByteOffset += ModelBodyInternal::positionLen;
	
	Accessor uv1Acc;
	int32_t uv1AccIndex = (int32_t)gltfDoc.accessors.size();
	uv1Acc.bufferView = bufViewIndex;
	uv1Acc.byteOffset = curByteOffset;
	uv1Acc.count = vertCount;
	uv1Acc.type = Accessor::Type::Vec2;
	uv1Acc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(uv1Acc);
	curByteOffset += ModelBodyInternal::UVLen;

	Accessor uv2Acc;
	int32_t uv2AccIndex = (int32_t)gltfDoc.accessors.size();
	uv2Acc.bufferView = bufViewIndex;
	uv2Acc.byteOffset = curByteOffset;
	uv2Acc.count = vertCount;
	uv2Acc.type = Accessor::Type::Vec2;
	uv2Acc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(uv2Acc);
	curByteOffset += ModelBodyInternal::UVLen;

	std::vector<int32_t> vertexColAccIndices;
	for (size_t x = 0; x < body.colors.size(); x++)
	{
		Accessor colAcc;
		vertexColAccIndices.push_back((int32_t)gltfDoc.accessors.size());
		colAcc.bufferView = bufViewIndex;
		colAcc.byteOffset = curByteOffset;
		colAcc.count = vertCount;
		colAcc.type = Accessor::Type::Vec4;
		colAcc.componentType = Accessor::ComponentType::UnsignedByte;
		colAcc.normalized = true;
		gltfDoc.accessors.push_back(colAcc);
		curByteOffset += ModelBodyInternal::colorLen;
	}

	Attributes attrs;
	attrs["POSITION"] = posAccIndex;
	attrs["TEXCOORD_0"] = uv1AccIndex;
	attrs["TEXCOORD_1"] = uv2AccIndex;
	for (size_t x = 0; x < vertexColAccIndices.size(); x++)
		attrs["COLOR_" + std::to_string(x)] = vertexColAccIndices[x];

	return attrs;
}

int32_t
ModelExporterGltf::_addIndexData(Document& gltfDoc, const std::vector<uint16_t>& body)
{
	if (gltfDoc.buffers.size() == 0)
		_createBuffer(gltfDoc);
	
	Buffer& buf = gltfDoc.buffers[0];

	uint32_t indexSize = (uint32_t)body.size() * (uint32_t)sizeof(body[0]);
	uint32_t byteLen = indexSize + (indexSize % 4);
	uint32_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	std::memcpy(&buf.data[startOffset], body.data(), indexSize);
	buf.byteLength += byteLen;

	BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.target = BufferView::TargetType::ElementArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

	return bufViewIndex;
}

void
ModelExporterGltf::_modifyAsset(Document& gltfDoc)
{
	gltfDoc.asset.generator = m_generatorName;
	gltfDoc.asset.copyright = m_copyright;
}

void
ModelExporterGltf::_createBuffer(Document& gltfDoc)
{
	assert(gltfDoc.buffers.size() == 0);

	Buffer newBuf;
	newBuf.name = "Model Data";
	newBuf.byteLength = 0;
	gltfDoc.buffers.push_back(newBuf);
}

void
ModelExporterGltf::_checkAndFixBufferAllignment(Document& gltfDoc)
{
	Buffer& buf = gltfDoc.buffers[0];
	int mod = buf.byteLength % 4;
	if (mod != 0)
	{
		buf.data.resize(buf.data.size() + mod);
		buf.byteLength += mod;
	}
}


std::vector<float>
ModelExporterGltf::_findMaxVec3(const std::vector<glm::vec3>& body)
{
	std::vector<float> max = { -100.0F, -100.0F, -100.0F };
	for (size_t x = 0; x < body.size(); x++)
	{
		if (body[x][0] > max[0])
			max[0] = body[x][0];
		if (body[x][1] > max[1])
			max[1] = body[x][1];
		if (body[x][2] > max[2])
			max[2] = body[x][2];
	}
	return max;
}

std::vector<float>
ModelExporterGltf::_findMinVec3(const std::vector<glm::vec3>& body)
{
	std::vector<float> min = { 100.0F, 100.0F, 100.0F };
	for (size_t x = 0; x < body.size(); x++)
	{
		if (body[x][0] < min[0])
			min[0] = body[x][0];
		if (body[x][1] < min[1])
			min[1] = body[x][1];
		if (body[x][2] < min[2])
			min[2] = body[x][2];
	}
	return min;
}

void
ModelExporterGltf::_findChildrenOfBone(const std::vector<BoneTreeNodeInternal>& boneTree, uint16_t boneIndex, std::vector<int32_t>& out)
{
	for (uint16_t x = 0; x < boneTree.size(); x++)
	{
		if (boneTree[x].parentIndex == boneIndex)
			out.push_back(x);
	}
}
