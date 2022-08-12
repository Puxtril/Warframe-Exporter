#include "model/GltfExporter.h"

using namespace WarframeExporter::Model;

GltfModel::GltfModel()
	: m_document()
{
}

void
GltfModel::addModelData(const ModelHeaderInternal& header, const ModelBodyInternal& body)
{
	if (header.getBoneTree().size() > 0)
		addModelDataRigged(header, body);
	else
		addModelDataStatic(header, body);
}

void
GltfModel::addModelDataRigged(const ModelHeaderInternal& header, const ModelBodyInternal& body)
{
	modifyAsset();

	Attributes vertsAttrs = addVertexDataRigged(body, header.getVertexCount());
	unsigned int indicesBufViewIndex = addIndexData(body.getIndices());
	std::vector<int32_t> meshIndices = createMeshes(header.getMeshInfos(), vertsAttrs, indicesBufViewIndex);

	int32_t rootBoneNodeIndex = createBones(header.getBoneTree());
	int32_t inverseMatricesIndex = addInverseBindMatrices(header.getBoneTree(), header.getWeightedBones());
	int32_t skinIndex = createSkin(header.getWeightedBones(), header.getBoneTree().size(), "Skeleton", rootBoneNodeIndex, inverseMatricesIndex);

	createSceneWithModelNodes(meshIndices, skinIndex);
	m_document.scenes[0].nodes.push_back(rootBoneNodeIndex);
}

void
GltfModel::addModelDataStatic(const ModelHeaderInternal& header, const ModelBodyInternal& body)
{
	modifyAsset();

	Attributes vertsAttrs = addVertexDataStatic(body, header.getVertexCount());
	int32_t indicesBufViewIndex = addIndexData(body.getIndices());
	std::vector<int32_t> meshIndices = createMeshes(header.getMeshInfos(), vertsAttrs, indicesBufViewIndex);

	createSceneWithModelNodes(meshIndices, -1);
}

void
GltfModel::save(const std::string& outPathStr)
{
	std::filesystem::path outPath = { outPathStr };
	try
	{
		Save(m_document, outPath, true);
	}
	catch (std::exception& ex)
	{
		print_exception(ex);
	}
}

void
GltfModel::print_exception(const std::exception& e, int level)
{
	std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
	try {
		std::rethrow_if_nested(e);
	}
	catch (const std::exception& nestedException) {
		print_exception(nestedException, level + 1);
	}
	catch (...) {}
}

void
GltfModel::createSceneWithModelNodes(const std::vector<int32_t>& meshes, int32_t skinIndex)
{
	assert(m_document.scenes.size() == 0);
	Scene scene;
	scene.name = "Scene";

	for (size_t x = 0; x < meshes.size(); x++)
	{
		Node curNode;
		curNode.mesh = meshes[x];
		curNode.skin = skinIndex;
		size_t curNodeIndex = m_document.nodes.size();
		m_document.nodes.push_back(curNode);
		scene.nodes.push_back(curNodeIndex);
	}

	m_document.scenes.push_back(scene);
	m_document.scene = 0;

}

int32_t
GltfModel::createBones(const std::vector<BoneTreeNodeInternal>& boneTree)
{
	int32_t rootIndex = -1;
	if (boneTree.size() > 0)
		rootIndex = m_document.nodes.size();
	for (size_t x = 0; x < boneTree.size(); x++)
	{
		Node boneNode;
		boneNode.name = boneTree[x].getName();
		findChildrenOfBone(boneTree, x, boneNode.children);

		std::memcpy(&boneNode.rotation[0], &boneTree[x].getRotation()[0], sizeof(float) * 4);
		std::memcpy(&boneNode.translation[0], &boneTree[x].getPosition()[0], sizeof(float) * 3);

		m_document.nodes.push_back(boneNode);
	}
	return rootIndex;
}

int32_t
GltfModel::createSkin(const std::vector<size_t>& weightedIndices, size_t totalBoneCount, const std::string& skinName, int32_t rootBoneIndex, int32_t inverseBindMatricesIndex)
{
	Skin skin;
	size_t skinIndex = m_document.skins.size();
	skin.name = skinName;
	skin.skeleton = rootBoneIndex;
	std::vector<bool> addedBones(totalBoneCount, false);
	for (size_t x = 0; x < weightedIndices.size(); x++)
	{
		skin.joints.push_back(weightedIndices[x] + rootBoneIndex);
		addedBones[weightedIndices[x]] = true;
	}
	for (size_t x = 0; x < totalBoneCount; x++)
	{
		if (!addedBones[x])
			skin.joints.push_back(x + rootBoneIndex);
	}

	skin.inverseBindMatrices = inverseBindMatricesIndex;
	m_document.skins.push_back(skin);
	return skinIndex;
}

int32_t
GltfModel::addInverseBindMatrices(const std::vector<BoneTreeNodeInternal>& boneTree, const std::vector<size_t>& weightedIndices)
{
	if (m_document.buffers.size() == 0)
		createBuffer();
	checkAndFixBufferAllignment();

	Buffer& buf = m_document.buffers[0];
	size_t matSize = sizeof(boneTree[0].getReverseBind());
	size_t byteLen = boneTree.size() * matSize;
	size_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	buf.byteLength += byteLen;
	uint8_t* curPos = buf.data.data() + startOffset;

	std::vector<bool> addedBones(boneTree.size(), false);
	for (size_t x = 0; x < weightedIndices.size(); x++)
	{
		size_t boneTreeIndex = weightedIndices[x];
		std::memcpy(curPos, glm::value_ptr(boneTree[boneTreeIndex].getReverseBind()), matSize);
		curPos += matSize;
		addedBones[boneTreeIndex] = true;
	}
	for (size_t x = 0; x < boneTree.size(); x++)
	{
		if (!addedBones[x])
		{
			std::memcpy(curPos, glm::value_ptr(boneTree[x].getReverseBind()), matSize);
			curPos += matSize;
		}
	}

	BufferView bufView;
	int32_t bufViewIndex = m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.byteStride = 0;
	m_document.bufferViews.push_back(bufView);

	Accessor matAcc;
	int32_t matAccIndex = m_document.accessors.size();
	matAcc.bufferView = bufViewIndex;
	matAcc.byteOffset = 0;
	matAcc.count = boneTree.size();
	matAcc.type = Accessor::Type::Mat4;
	matAcc.componentType = Accessor::ComponentType::Float;
	m_document.accessors.push_back(matAcc);

	return matAccIndex;
}

std::vector<int32_t>
GltfModel::createMeshes(const std::vector<MeshInfoInternal>& meshInfos, Attributes attrs, int32_t indicesBuffViewIndex)
{
	Mesh mesh;
	std::vector<int32_t> meshIndices;

	for (size_t x = 0; x < meshInfos.size(); x++)
	{
		Accessor curAcc;
		int32_t curAccIndex = m_document.accessors.size();
		curAcc.bufferView = indicesBuffViewIndex;
		curAcc.byteOffset = meshInfos[x].getLODOffsets()[0] * 2;
		curAcc.count = meshInfos[x].getLODCounts()[0];
		curAcc.type = Accessor::Type::Scalar;
		curAcc.componentType = Accessor::ComponentType::UnsignedShort;
		m_document.accessors.push_back(curAcc);

		Primitive curPrim;
		curPrim.indices = curAccIndex;
		curPrim.material = findOrCreateMaterial(meshInfos[x].getMaterialName());
		curPrim.mode = Primitive::Mode::Triangles;
		curPrim.attributes = attrs;

		Mesh curMesh;
		int32_t curMeshIndex = m_document.meshes.size();
		curMesh.name = meshInfos[x].getName();
		curMesh.primitives.push_back(curPrim);
		m_document.meshes.push_back(curMesh);
		
		meshIndices.push_back(curMeshIndex);
	}

	return meshIndices;
}

int32_t
GltfModel::findOrCreateMaterial(const std::string& materialName)
{
	for (size_t x = 0; x < m_document.materials.size(); x++)
	{
		if (m_document.materials[x].name.compare(materialName) == 0)
			return x;
	}
	
	size_t matIndex = m_document.materials.size();
	Material mat;
	mat.name = materialName;
	mat.doubleSided = true;
	m_document.materials.push_back(mat);

	return matIndex;
}

Attributes
GltfModel::addVertexDataRigged(const ModelBodyInternal& body, size_t vertCount)
{
	if (m_document.buffers.size() == 0)
		createBuffer();

	Buffer& buf = m_document.buffers[0];
	size_t byteLen = vertCount * ModelBodyInternal::vertexSizeRigged;
	size_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	buf.byteLength += byteLen;
	uint8_t* curPos = buf.data.data() + startOffset;

	for (size_t x = 0; x < vertCount; x++)
	{
		memcpy(curPos, &body.getPositions()[x].x, ModelBodyInternal::positionLen);
		curPos += ModelBodyInternal::positionLen;
		memcpy(curPos, &body.getColors()[x].x, ModelBodyInternal::colorLen);
		curPos += ModelBodyInternal::colorLen;
		memcpy(curPos, &body.getUV1()[x].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
		memcpy(curPos, &body.getUV2()[x].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
		memcpy(curPos, &body.getBoneIndices()[x].x, ModelBodyInternal::boneIndexLen);
		curPos += ModelBodyInternal::boneIndexLen;
		memcpy(curPos, &body.getBoneWeights()[x].x, ModelBodyInternal::boneWeightLen);
		curPos += ModelBodyInternal::boneWeightLen;
	}

	BufferView bufView;
	int32_t bufViewIndex = m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.byteStride = ModelBodyInternal::vertexSizeRigged;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	m_document.bufferViews.push_back(bufView);
	
	Accessor posAcc;
	int32_t posAccIndex = m_document.accessors.size();
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = 0;
	posAcc.count = vertCount;
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	posAcc.max = findMaxVec3(body.getPositions());
	posAcc.min = findMinVec3(body.getPositions());
	m_document.accessors.push_back(posAcc);

	Accessor colAcc;
	int32_t colAccIndex = m_document.accessors.size();
	colAcc.bufferView = bufViewIndex;
	colAcc.byteOffset = ModelBodyInternal::positionLen;
	colAcc.count = vertCount;
	colAcc.type = Accessor::Type::Vec4;
	colAcc.componentType = Accessor::ComponentType::UnsignedByte;
	colAcc.normalized = true;
	m_document.accessors.push_back(colAcc);

	Accessor uv1Acc;
	int32_t uv1AccIndex = m_document.accessors.size();
	uv1Acc.bufferView = bufViewIndex;
	uv1Acc.byteOffset = ModelBodyInternal::positionLen + ModelBodyInternal::colorLen;
	uv1Acc.count = vertCount;
	uv1Acc.type = Accessor::Type::Vec2;
	uv1Acc.componentType = Accessor::ComponentType::Float;
	m_document.accessors.push_back(uv1Acc);

	Accessor uv2Acc;
	int32_t uv2AccIndex = m_document.accessors.size();
	uv2Acc.bufferView = bufViewIndex;
	uv2Acc.byteOffset = ModelBodyInternal::positionLen + ModelBodyInternal::colorLen + ModelBodyInternal::UVLen;
	uv2Acc.count = vertCount;
	uv2Acc.type = Accessor::Type::Vec2;
	uv2Acc.componentType = Accessor::ComponentType::Float;
	m_document.accessors.push_back(uv2Acc);

	Accessor boneIndexAcc;
	int32_t boneIndexAccIndex = m_document.accessors.size();
	boneIndexAcc.bufferView = bufViewIndex;
	boneIndexAcc.byteOffset = ModelBodyInternal::positionLen + ModelBodyInternal::colorLen + ModelBodyInternal::UVLen + ModelBodyInternal::UVLen;
	boneIndexAcc.count = vertCount;
	boneIndexAcc.type = Accessor::Type::Vec4;
	boneIndexAcc.componentType = Accessor::ComponentType::UnsignedShort;
	m_document.accessors.push_back(boneIndexAcc);

	Accessor boneWeightAcc;
	int32_t boneWeightAccIndex = m_document.accessors.size();
	boneWeightAcc.bufferView = bufViewIndex;
	boneWeightAcc.byteOffset = ModelBodyInternal::positionLen + ModelBodyInternal::colorLen + ModelBodyInternal::UVLen + ModelBodyInternal::UVLen + ModelBodyInternal::boneIndexLen;
	boneWeightAcc.count = vertCount;
	boneWeightAcc.type = Accessor::Type::Vec4;
	boneWeightAcc.componentType = Accessor::ComponentType::Float;
	//boneWeightAcc.normalized = true;
	m_document.accessors.push_back(boneWeightAcc);

	Attributes attrs;
	attrs["POSITION"] = posAccIndex;
	attrs["COLOR_0"] = colAccIndex;
	attrs["TEXCOORD_0"] = uv1AccIndex;
	attrs["TEXCOORD_1"] = uv2AccIndex;
	attrs["JOINTS_0"] = boneIndexAccIndex;
	attrs["WEIGHTS_0"] = boneWeightAccIndex;

	return attrs;
}

Attributes
GltfModel::addVertexDataStatic(const ModelBodyInternal& body, size_t vertCount)
{
	if (m_document.buffers.size() == 0)
		createBuffer();

	Buffer& buf = m_document.buffers[0];
	size_t byteLen = vertCount * ModelBodyInternal::vertexSizeStatic;
	size_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	buf.byteLength += byteLen;
	uint8_t* curPos = buf.data.data() + startOffset;

	for (size_t x = 0; x < vertCount; x++)
	{
		memcpy(curPos, &body.getPositions()[x].x, ModelBodyInternal::positionLen);
		curPos += ModelBodyInternal::positionLen;
		memcpy(curPos, &body.getColors()[x].x, ModelBodyInternal::colorLen);
		curPos += ModelBodyInternal::colorLen;
		memcpy(curPos, &body.getUV1()[x].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
		memcpy(curPos, &body.getUV2()[x].x, ModelBodyInternal::UVLen);
		curPos += ModelBodyInternal::UVLen;
	}

	BufferView bufView;
	int32_t bufViewIndex = m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.byteStride = ModelBodyInternal::vertexSizeStatic;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	m_document.bufferViews.push_back(bufView);

	Accessor posAcc;
	int32_t posAccIndex = m_document.accessors.size();
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = 0;
	posAcc.count = vertCount;
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	posAcc.max = findMaxVec3(body.getPositions());
	posAcc.min = findMinVec3(body.getPositions());
	m_document.accessors.push_back(posAcc);

	Accessor colAcc;
	int32_t colAccIndex = m_document.accessors.size();
	colAcc.bufferView = bufViewIndex;
	colAcc.byteOffset = ModelBodyInternal::positionLen;
	colAcc.count = vertCount;
	colAcc.type = Accessor::Type::Vec4;
	colAcc.componentType = Accessor::ComponentType::UnsignedByte;
	colAcc.normalized = true;
	m_document.accessors.push_back(colAcc);

	Accessor uv1Acc;
	int32_t uv1AccIndex = m_document.accessors.size();
	uv1Acc.bufferView = bufViewIndex;
	uv1Acc.byteOffset = ModelBodyInternal::positionLen + ModelBodyInternal::colorLen;
	uv1Acc.count = vertCount;
	uv1Acc.type = Accessor::Type::Vec2;
	uv1Acc.componentType = Accessor::ComponentType::Float;
	m_document.accessors.push_back(uv1Acc);

	Accessor uv2Acc;
	int32_t uv2AccIndex = m_document.accessors.size();
	uv2Acc.bufferView = bufViewIndex;
	uv2Acc.byteOffset = ModelBodyInternal::positionLen + ModelBodyInternal::colorLen + ModelBodyInternal::UVLen;
	uv2Acc.count = vertCount;
	uv2Acc.type = Accessor::Type::Vec2;
	uv2Acc.componentType = Accessor::ComponentType::Float;
	m_document.accessors.push_back(uv2Acc);

	Attributes attrs;
	attrs["POSITION"] = posAccIndex;
	attrs["COLOR_0"] = colAccIndex;
	attrs["TEXCOORD_0"] = uv1AccIndex;
	attrs["TEXCOORD_1"] = uv2AccIndex;

	return attrs;
}

int32_t
GltfModel::addIndexData(const std::vector<uint16_t>& body)
{
	if (m_document.buffers.size() == 0)
		createBuffer();
	
	Buffer& buf = m_document.buffers[0];

	size_t byteLen = body.size() * sizeof(body[0]);
	size_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	std::memcpy(&buf.data[startOffset], body.data(), byteLen);
	buf.byteLength += byteLen;

	BufferView bufView;
	size_t bufViewIndex = m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = startOffset;
	bufView.byteLength = byteLen;
	bufView.target = BufferView::TargetType::ElementArrayBuffer;
	m_document.bufferViews.push_back(bufView);

	return bufViewIndex;
}

void
GltfModel::modifyAsset()
{
	m_document.asset.generator = m_generatorName;
	m_document.asset.copyright = m_copyright;
}

void
GltfModel::createBuffer()
{
	assert(m_document.buffers.size() == 0);

	Buffer newBuf;
	newBuf.name = "Model Data";
	newBuf.byteLength = 0;
	m_document.buffers.push_back(newBuf);
}

void
GltfModel::checkAndFixBufferAllignment()
{
	Buffer& buf = m_document.buffers[0];
	int mod = buf.byteLength % 4;
	if (mod != 0)
	{
		buf.data.resize(buf.data.size() + mod);
		buf.byteLength += mod;
	}
}

std::vector<float>
GltfModel::findMaxVec3(const std::vector<glm::vec3>& body)
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
GltfModel::findMinVec3(const std::vector<glm::vec3>& body)
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
GltfModel::findChildrenOfBone(const std::vector<BoneTreeNodeInternal>& boneTree, uint16_t boneIndex, std::vector<int32_t>& out)
{
	for (uint16_t x = 0; x < boneTree.size(); x++)
	{
		if (boneTree[x].getParentIndex() == boneIndex)
			out.push_back(x);
	}
}