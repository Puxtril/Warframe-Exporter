#include "model/ModelExporterGltf.h"

using namespace WarframeExporter::Model;

void
ModelExporterGltf::addModelData(Document& gltfDoc, const ModelHeaderInternal& header, const ModelBodyInternal& bodyInt, const ModelBodyExternal& bodyExt)
{
	_modifyAsset(gltfDoc);

	Attributes vertsAttrs = _addVertexData(gltfDoc, bodyInt, bodyExt, header.vertexCount);
	std::vector<int32_t> indicesAccessors = _addIndexData(gltfDoc, bodyInt.indices, header.meshInfos);

	std::vector<int32_t> meshIndices;
	for (size_t meshIndex = 0; meshIndex < indicesAccessors.size(); meshIndex++)
	{
		int32_t index = _createMesh(gltfDoc, vertsAttrs, indicesAccessors[meshIndex], header.meshInfos[meshIndex].matName, header.meshInfos[meshIndex].name);
		_addModelExtraInformation(gltfDoc, index, header);
		meshIndices.push_back(index);
	}

	if (header.boneTree.size() > 0)
	{
		int32_t rootBoneNodeIndex = _createBones(gltfDoc, header.boneTree);
		int32_t inverseMatricesIndex = _addInverseBindMatrices(gltfDoc, header.boneTree, header.weightedBones);
		int32_t skinIndex = _createSkin(gltfDoc, header.weightedBones, (int)header.boneTree.size(), "Skeleton", rootBoneNodeIndex, inverseMatricesIndex);

		_addModelsToScene(gltfDoc, meshIndices, skinIndex);
		
		gltfDoc.scenes[0].nodes.push_back(rootBoneNodeIndex);
	}
	else
	{
		_addModelsToScene(gltfDoc, meshIndices);
	}
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
ModelExporterGltf::_addModelExtraInformation(Document& gltfDoc, int32_t meshIndex, const ModelHeaderInternal& header)
{
	Mesh& curMesh = gltfDoc.meshes[meshIndex];
	curMesh.extensionsAndExtras["extras"]["EnsmalleningScale"] = std::array<float, 4>{header.modelScale.x,header.modelScale.y, header.modelScale.z, header.modelScale.w};
}

void
ModelExporterGltf::_addModelsToScene(Document& gltfDoc, const std::vector<int32_t>& meshes, int32_t skinIndex)
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
	Buffer& buf = _getBuffer(gltfDoc);
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
	bufView.buffer = gltfDoc.buffers.size() - 1;
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

int32_t
ModelExporterGltf::_createMesh(Document& gltfDoc, Attributes attrs, int32_t indicies, const std::string& materialName, const std::string& modelName)
{
	Primitive curPrim;
	curPrim.indices = indicies;
	curPrim.material = _findOrCreateMaterial(gltfDoc, materialName);
	curPrim.mode = Primitive::Mode::Triangles;
	curPrim.attributes = attrs;

	Mesh curMesh;
	int32_t curMeshIndex = static_cast<int32_t>(gltfDoc.meshes.size());
	curMesh.name = modelName;
	curMesh.primitives.push_back(curPrim);
	gltfDoc.meshes.push_back(curMesh);

	return curMeshIndex;
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

Attributes
ModelExporterGltf::_addVertexData(Document& gltfDoc, const ModelBodyInternal& bodyInt, const ModelBodyExternal& bodyExt, int vertCount)
{
	Buffer& buf = _getBuffer(gltfDoc);

	bool 
		addPosition = true,
		addUV1 = bodyInt.UV1.size() > 0 ? true : false,
		addUV2 = bodyInt.UV2.size() > 0 ? true : false,
		addAO = bodyInt.AO.size() > 0 ? true : false,
		addBoneIndex = bodyInt.boneIndices.size() > 0 ? true : false,
		addBoneWeight = bodyInt.boneWeights.size() > 0 ? true : false,
		addRawPosition = bodyExt.positions.size() > 0 ? true : false,
		addRawNormal = bodyExt.normals.size() > 0 ? true : false,
		addRawTangent = bodyExt.tangents.size() > 0 ? true : false;

	// Custom attributes/colors as Uint8 must be exported as Vec4
	uint32_t modelTotalByteLen = (
		(addPosition ? bodyInt.positionTypeSize() : 0) +
		(addUV1 ? bodyInt.UVTypeSize() : 0) +
		(addUV2 ? bodyInt.UVTypeSize() : 0) +
		(addAO ? bodyInt.AOTypeSize() * 4 : 0) + // 1 -> 4
		(bodyInt.colorTypeSize() * bodyInt.colors.size()) +
		(addBoneIndex ? bodyInt.boneIndexTypeSize() : 0) +
		(addBoneWeight ? bodyInt.boneWeightTypeSize() : 0) + 
		(addRawPosition ? sizeof(bodyExt.positions[0]) : 0) +
		(addRawNormal ? sizeof(bodyExt.normals[0]) : 0) +
		(addRawTangent ? 4 : 0) // 3 -> 4
	) * vertCount;

	// Assert 32-bit allignment
	modelTotalByteLen += (modelTotalByteLen % 4);

	// Resize buffer
	uint32_t startOffset = buf.byteLength;
	buf.data.resize(startOffset + modelTotalByteLen);
	buf.byteLength += modelTotalByteLen;

	// Cursors for inserting into Buffer (and thus, BufferView)
	uint8_t* bufferCursor = buf.data.data() + startOffset;
	uint32_t bufferViewCursor = 0;

	// Create glTF BufferView
	// Resize to current model byte length, then don't touch again
	BufferView bufView;
	int32_t bufViewIndex = static_cast<int32_t>(gltfDoc.bufferViews.size());
	bufView.buffer = gltfDoc.buffers.size() - 1;
	bufView.byteOffset = startOffset;
	bufView.byteLength = modelTotalByteLen;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

	Attributes attrs;

	if (addPosition)
	{
		uint32_t byteSize = bodyInt.positionTypeSize() * bodyInt.positions.size();
		memcpy(bufferCursor, &bodyInt.positions[0], byteSize);
		bufferCursor += byteSize;

		Accessor posAcc;
		int32_t posAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
		posAcc.bufferView = bufViewIndex;
		posAcc.byteOffset = bufferViewCursor;
		posAcc.count = vertCount;
		posAcc.type = Accessor::Type::Vec3;
		posAcc.componentType = Accessor::ComponentType::Float;
		posAcc.max = _findMaxVec3(bodyInt.positions);
		posAcc.min = _findMinVec3(bodyInt.positions);
		gltfDoc.accessors.push_back(posAcc);
		bufferViewCursor += byteSize;

		attrs["POSITION"] = posAccIndex;
	}

	if (addUV1)
	{
		uint32_t byteSize = bodyInt.UVTypeSize() * bodyInt.UV1.size();
		memcpy(bufferCursor, &bodyInt.UV1[0], byteSize);
		bufferCursor += byteSize;

		Accessor uv1Acc;
		int32_t uv1AccIndex = (int32_t)gltfDoc.accessors.size();
		uv1Acc.bufferView = bufViewIndex;
		uv1Acc.byteOffset = bufferViewCursor;
		uv1Acc.count = vertCount;
		uv1Acc.type = Accessor::Type::Vec2;
		uv1Acc.componentType = Accessor::ComponentType::Float;
		gltfDoc.accessors.push_back(uv1Acc);
		bufferViewCursor += byteSize;

		attrs["TEXCOORD_0"] = uv1AccIndex;
	}

	if (addUV2)
	{
		uint32_t byteSize = bodyInt.UVTypeSize() * bodyInt.UV2.size();
		memcpy(bufferCursor, &bodyInt.UV2[0], byteSize);
		bufferCursor += byteSize;

		Accessor uv2Acc;
		int32_t uv2AccIndex = (int32_t)gltfDoc.accessors.size();
		uv2Acc.bufferView = bufViewIndex;
		uv2Acc.byteOffset = bufferViewCursor;
		uv2Acc.count = vertCount;
		uv2Acc.type = Accessor::Type::Vec2;
		uv2Acc.componentType = Accessor::ComponentType::Float;
		gltfDoc.accessors.push_back(uv2Acc);
		bufferViewCursor += byteSize;

		attrs["TEXCOORD_1"] = uv2AccIndex;
	}

	if (addAO)
	{
		uint32_t byteSize = bodyInt.AOTypeSize() * bodyInt.AO.size();
		for (const uint8_t& curAO : bodyInt.AO)
		{
			memcpy(bufferCursor++, &curAO, 1);
			memcpy(bufferCursor++, &curAO, 1);
			memcpy(bufferCursor++, &curAO, 1);
			memcpy(bufferCursor++, &curAO, 1);
		}

		Accessor aoAcc;
		int32_t aoAccIndex = (int32_t)gltfDoc.accessors.size();
		aoAcc.bufferView = bufViewIndex;
		aoAcc.byteOffset = bufferViewCursor;
		aoAcc.count = vertCount;
		aoAcc.type = Accessor::Type::Vec4;
		aoAcc.componentType = Accessor::ComponentType::UnsignedByte;
		aoAcc.normalized = true;
		gltfDoc.accessors.push_back(aoAcc);
		bufferViewCursor += (byteSize * 4);

		attrs["_AmbientOcclusion"] = aoAccIndex;
	}

	for (size_t x = 0; x < bodyInt.colors.size(); x++)
	{
		uint32_t byteSize = bodyInt.colorTypeSize() * bodyInt.colors[x].size();
		memcpy(bufferCursor, &bodyInt.colors[x][0], byteSize);
		bufferCursor += byteSize;

		Accessor colAcc;
		int32_t vertColAcc = (int32_t)gltfDoc.accessors.size();
		colAcc.bufferView = bufViewIndex;
		colAcc.byteOffset = bufferViewCursor;
		colAcc.count = vertCount;
		colAcc.type = Accessor::Type::Vec4;
		colAcc.componentType = Accessor::ComponentType::UnsignedByte;
		colAcc.normalized = true;
		gltfDoc.accessors.push_back(colAcc);
		bufferViewCursor += byteSize;

		attrs["COLOR_" + std::to_string(x)] = vertColAcc;
	}

	if (addBoneIndex)
	{
		uint32_t byteSize = bodyInt.boneIndexTypeSize() * bodyInt.boneIndices.size();
		memcpy(bufferCursor, &bodyInt.boneIndices[0], byteSize);
		bufferCursor += byteSize;

		Accessor boneIndexAcc;
		int32_t boneIndexAccIndex = (int32_t)gltfDoc.accessors.size();
		boneIndexAcc.bufferView = bufViewIndex;
		boneIndexAcc.byteOffset = bufferViewCursor;
		boneIndexAcc.count = vertCount;
		boneIndexAcc.type = Accessor::Type::Vec4;
		boneIndexAcc.componentType = Accessor::ComponentType::UnsignedShort;
		gltfDoc.accessors.push_back(boneIndexAcc);
		bufferViewCursor += byteSize;

		attrs["JOINTS_0"] = boneIndexAccIndex;
	}

	if (addBoneWeight)
	{
		uint32_t byteSize = bodyInt.boneWeightTypeSize() * bodyInt.boneWeights.size();
		memcpy(bufferCursor, &bodyInt.boneWeights[0], byteSize);
		bufferCursor += byteSize;

		Accessor boneWeightAcc;
		int32_t boneWeightAccIndex = (int32_t)gltfDoc.accessors.size();
		boneWeightAcc.bufferView = bufViewIndex;
		boneWeightAcc.byteOffset = bufferViewCursor;
		boneWeightAcc.count = vertCount;
		boneWeightAcc.type = Accessor::Type::Vec4;
		boneWeightAcc.componentType = Accessor::ComponentType::Float;
		gltfDoc.accessors.push_back(boneWeightAcc);
		bufferViewCursor += byteSize;

		attrs["WEIGHTS_0"] = boneWeightAccIndex;
	}

	if (addRawPosition)
	{
		uint32_t byteSize = sizeof(bodyExt.positions[0]) * bodyExt.positions.size();
		memcpy(bufferCursor, &bodyExt.positions[0], byteSize);
		bufferCursor += byteSize;

		Accessor posAcc;
		int32_t posAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
		posAcc.bufferView = bufViewIndex;
		posAcc.byteOffset = bufferViewCursor;
		posAcc.count = vertCount;
		posAcc.type = Accessor::Type::Vec4;
		posAcc.componentType = Accessor::ComponentType::Float;
		posAcc.normalized = true;
		gltfDoc.accessors.push_back(posAcc);
		bufferViewCursor += byteSize;

		attrs["_RawPosition"] = posAccIndex;
	}

	if (addRawNormal)
	{
		uint32_t byteSize = sizeof(bodyExt.normals[0]) * bodyExt.normals.size();
		memcpy(bufferCursor, &bodyExt.normals[0], byteSize);
		bufferCursor += byteSize;

		Accessor normalAcc;
		int32_t normalAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
		normalAcc.bufferView = bufViewIndex;
		normalAcc.byteOffset = bufferViewCursor;
		normalAcc.count = vertCount;
		normalAcc.type = Accessor::Type::Vec4;
		normalAcc.componentType = Accessor::ComponentType::UnsignedByte;
		normalAcc.normalized = true;
		gltfDoc.accessors.push_back(normalAcc);
		bufferViewCursor += byteSize;

		attrs["_RawNormal"] = normalAccIndex;
	}

	if (addRawTangent)
	{
		uint32_t byteSize = 4 * bodyExt.tangents.size();
		for (size_t i = 0; i < bodyExt.tangents.size(); i++)
		{
			memcpy(bufferCursor, &bodyExt.tangents[i][0], 3);
			memcpy(bufferCursor + 3, &bodyInt.AO[i], 1);
			bufferCursor += 4;
		}

		Accessor tangentAcc;
		int32_t tangentAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
		tangentAcc.bufferView = bufViewIndex;
		tangentAcc.byteOffset = bufferViewCursor;
		tangentAcc.count = vertCount;
		tangentAcc.type = Accessor::Type::Vec4;
		tangentAcc.componentType = Accessor::ComponentType::UnsignedByte;
		tangentAcc.normalized = true;
		gltfDoc.accessors.push_back(tangentAcc);
		bufferViewCursor += byteSize;

		attrs["_RawTangent"] = tangentAccIndex;
	}

	return attrs;
}

std::vector<int32_t>
ModelExporterGltf::_addIndexData(Document& gltfDoc, const std::vector<uint16_t>& body, const std::vector<MeshInfoInternal>& meshInfos)
{
	Buffer& buf = _getBuffer(gltfDoc);

	uint32_t indexSize = (uint32_t)body.size() * (uint32_t)sizeof(body[0]);
	uint32_t byteLen = indexSize + (indexSize % 4);
	uint32_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	std::memcpy(&buf.data[startOffset], body.data(), indexSize);
	buf.byteLength += byteLen;

	BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = gltfDoc.buffers.size() - 1;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.target = BufferView::TargetType::ElementArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

	std::vector<int32_t> accessorIndicies;
	for (size_t x = 0; x < meshInfos.size(); x++)
	{
		Accessor curAcc;
		int32_t curAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
		curAcc.bufferView = bufViewIndex;
		curAcc.byteOffset = meshInfos[x].faceLODOffsets[0] * 2;
		curAcc.count = meshInfos[x].faceLODCounts[0];
		curAcc.type = Accessor::Type::Scalar;
		curAcc.componentType = Accessor::ComponentType::UnsignedShort;
		gltfDoc.accessors.push_back(curAcc);
		
		accessorIndicies.push_back(curAccIndex);
	}

	return accessorIndicies;
}

void
ModelExporterGltf::_modifyAsset(Document& gltfDoc)
{
	gltfDoc.asset.generator = m_generatorName;
	gltfDoc.asset.copyright = m_copyright;
}

Buffer&
ModelExporterGltf::_getBuffer(Document& gltfDoc)
{
	if (gltfDoc.buffers.size() == 0)
	{
		Buffer newBuf;
		newBuf.name = "Model Data 1";
		newBuf.byteLength = 0;
		gltfDoc.buffers.push_back(newBuf);
	}

	return gltfDoc.buffers.back();
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
