#include "animation/AnimationExporterGLTF.h"

using namespace WarframeExporter::Animation;

gltfAnimation::gltfAnimation()
	: m_document()
{
}

void
gltfAnimation::save(const std::filesystem::path& outPath)
{
	gltf::Save(m_document, outPath, true);
}

void
gltfAnimation::addAnimationData(const AnimationData& animData)
{
	if (animData.skeletons.size() > 1 || animData.skeletons[0].actions.size() > 1)
		throw std::runtime_error("More than 1 action/skeleton, skipping!");

	createBufferAndScene();
	
	int32_t rootBoneIndex = createBones(animData.skeletons[0].bones, animData.skeletons[0].actions[0]);
	addSkeletonToScene(rootBoneIndex, (int32_t)animData.skeletons[0].bones.size());
	addActionData(animData.skeletons[0].actions[0], rootBoneIndex);
}

void
gltfAnimation::addSkeletonToScene(int32_t rootBoneIndex, int32_t boneCount)
{
	gltf::Skin skin;
	int32_t skinIndex = m_document.skins.size();
	skin.name = "skeleton";
	skin.skeleton = rootBoneIndex;
	for (int32_t x = rootBoneIndex; x < rootBoneIndex + boneCount; x++)
		skin.joints.push_back(x);
	m_document.skins.push_back(skin);
	
	gltf::Node curNode;
	curNode.skin = skinIndex;
	int32_t curNodeIndex = m_document.nodes.size();
	m_document.nodes.push_back(curNode);
	m_document.scenes[0].nodes.push_back(curNodeIndex);
}

int32_t
gltfAnimation::createBones(const std::vector<BoneTreeNodeExternal>& boneTree, const AnimationActionInternal& action)
{
	int32_t rootIndex = -1;
	if (boneTree.size() > 0)
		rootIndex = (int32_t)m_document.nodes.size();
	for (size_t x = 0; x < boneTree.size(); x++)
	{
		gltf::Node boneNode;
		boneNode.name = boneTree[x].name;
		findChildrenOfBone(boneTree, x, boneNode.children);

		//std::memcpy(&boneNode.rotation[0], &action.initialTransform.rot[x][0], sizeof(float) * 4);
		//std::memcpy(&boneNode.translation[0], &action.initialTransform.pos[x][0], sizeof(float) * 3);
		//std::memcpy(&boneNode.scale[0], &action.initialTransform.scale[x][0], sizeof(float) * 3);

		m_document.nodes.push_back(boneNode);
	}

	return rootIndex;
}

void
gltfAnimation::addActionData(const AnimationActionInternal& action, uint32_t rootNodeOffset)
{
	int32_t timescaleIndex = addBufferData(action.timeScale, -1);
	m_document.accessors[timescaleIndex].min = { 0.0F };
	m_document.accessors[timescaleIndex].max = { 1.0F };
	
	int32_t curAnimIndex = m_document.animations.size();
	m_document.animations.push_back(gltf::Animation());
	m_document.animations[curAnimIndex].name = action.name;

	for (size_t x = 0; x < action.transforms.size(); x++)
	{
		const BoneTransform& curTransform = action.transforms[x];

		int32_t posAccIndex = addBufferData(curTransform.pos, -1);
		addSamplerandChannel(curAnimIndex, posAccIndex, timescaleIndex, rootNodeOffset + x, "translation");

		int32_t rotAccIndex = addBufferData(curTransform.rot, -1);
		addSamplerandChannel(curAnimIndex, rotAccIndex, timescaleIndex, rootNodeOffset + x, "rotation");

		//int32_t scaleAccIndex = addBufferData(curTransform.scale, curBufViewIndex);
		//addSamplerandChannel(curAnimIndex, scaleAccIndex, timescaleIndex, rootNodeOffset + x, "scale");
	}
}

void
gltfAnimation::addSamplerandChannel(uint32_t animationIndex, int32_t accessorIndex, uint32_t timescaleAccessorIndex, int32_t boneNodeIndex, std::string targetPath)
{
	gltf::Animation& curAnim = m_document.animations[animationIndex];

	gltf::Animation::Sampler sampler;
	int32_t posSampelerIndex = curAnim.samplers.size();
	sampler.input = timescaleAccessorIndex;
	sampler.output = accessorIndex;
	curAnim.samplers.push_back(sampler);

	gltf::Animation::Channel channel;
	channel.sampler = posSampelerIndex;
	channel.target.node = boneNodeIndex;
	channel.target.path = targetPath;
	curAnim.channels.push_back(channel);
}

int32_t
gltfAnimation::addBufferData(const std::vector<float>& inData, int32_t bufferViewIndex)
{
	gltf::Buffer& buf = m_document.buffers[0];

	size_t byteLen = 4 * inData.size();
	size_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	std::memcpy(&buf.data[startOffset], inData.data(), byteLen);
	buf.byteLength += byteLen;

	int32_t curBufViewLen;
	if (bufferViewIndex == -1)
	{
		gltf::BufferView bufView;
		bufView.buffer = 0;
		bufView.byteOffset = startOffset;
		bufView.byteLength = byteLen;
		bufferViewIndex = m_document.bufferViews.size();
		m_document.bufferViews.push_back(bufView);
		curBufViewLen = 0;
	}
	else
	{
		gltf::BufferView& curBufView = m_document.bufferViews[bufferViewIndex];
		curBufViewLen = m_document.bufferViews[bufferViewIndex].byteLength;
		curBufView.byteLength += byteLen;
	}

	gltf::Accessor accessor;
	size_t posAccIndex = m_document.accessors.size();
	accessor.bufferView = bufferViewIndex;
	accessor.byteOffset = curBufViewLen;
	accessor.count = inData.size();
	accessor.type = gltf::Accessor::Type::Scalar;
	accessor.componentType = gltf::Accessor::ComponentType::Float;
	m_document.accessors.push_back(accessor);

	return posAccIndex;
}

int32_t
gltfAnimation::addBufferData(const std::vector<glm::vec3>& inData, int32_t bufferViewIndex)
{
	gltf::Buffer& buf = m_document.buffers[0];

	size_t stride = 4 * 3;
	size_t byteLen = stride * inData.size();
	size_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	for (size_t x = 0; x < inData.size(); x++)
	{
		std::memcpy(&buf.data[startOffset + (stride * x)], &inData[x].x, stride);
	}
	buf.byteLength += byteLen;

	int32_t curBufViewLen;
	if (bufferViewIndex == -1)
	{
		gltf::BufferView bufView;
		bufView.buffer = 0;
		bufView.byteOffset = startOffset;
		bufView.byteLength = byteLen;
		bufferViewIndex = m_document.bufferViews.size();
		m_document.bufferViews.push_back(bufView);
		curBufViewLen = 0;
	}
	else
	{
		gltf::BufferView& curBufView = m_document.bufferViews[bufferViewIndex];
		curBufViewLen = m_document.bufferViews[bufferViewIndex].byteLength;
		curBufView.byteLength += byteLen;
	}

	gltf::Accessor accessor;
	size_t posAccIndex = m_document.accessors.size();
	accessor.bufferView = bufferViewIndex;
	accessor.byteOffset = curBufViewLen;
	accessor.count = inData.size();
	accessor.type = gltf::Accessor::Type::Vec3;
	accessor.componentType = gltf::Accessor::ComponentType::Float;
	m_document.accessors.push_back(accessor);

	return posAccIndex;
}

int32_t
gltfAnimation::addBufferData(const std::vector<glm::quat>& inData, int32_t bufferViewIndex)
{
	gltf::Buffer& buf = m_document.buffers[0];

	size_t stride = 4 * 4;
	size_t byteLen = stride * inData.size();
	size_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + byteLen);
	//std::memcpy(&buf.data[startOffset], &inData[0][0], byteLen);
	for (size_t x = 0; x < inData.size(); x++)
	{
		size_t destPos = startOffset + (stride * x);
		std::memcpy(&buf.data[destPos], &inData[x].w, 4);
		std::memcpy(&buf.data[destPos+4], &inData[x].x, 4);
		std::memcpy(&buf.data[destPos+8], &inData[x].y, 4);
		std::memcpy(&buf.data[destPos+12], &inData[x].z, 4);
	}
	buf.byteLength += byteLen;

	int32_t curBufViewLen;
	if (bufferViewIndex == -1)
	{
		gltf::BufferView bufView;
		bufView.buffer = 0;
		bufView.byteOffset = startOffset;
		bufView.byteLength = byteLen;
		bufferViewIndex = m_document.bufferViews.size();
		m_document.bufferViews.push_back(bufView);
		curBufViewLen = 0;
	}
	else
	{
		gltf::BufferView& curBufView = m_document.bufferViews[bufferViewIndex];
		curBufViewLen = m_document.bufferViews[bufferViewIndex].byteLength;
		curBufView.byteLength += byteLen;
	}

	gltf::Accessor accessor;
	size_t posAccIndex = m_document.accessors.size();
	accessor.bufferView = bufferViewIndex;
	accessor.byteOffset = curBufViewLen;
	accessor.count = inData.size();
	accessor.type = gltf::Accessor::Type::Vec4;
	accessor.componentType = gltf::Accessor::ComponentType::Float;
	m_document.accessors.push_back(accessor);

	return posAccIndex;
}

void
gltfAnimation::createBufferAndScene()
{
	assert(m_document.buffers.size() == 0);

	gltf::Buffer newBuf;
	newBuf.name = "Animation Data";
	//newBuf.byteLength = 9;
	//newBuf.data.resize(9);
	m_document.buffers.push_back(newBuf);

	gltf::Scene scene;
	scene.name = "Scene";
	m_document.scenes.push_back(scene);
	m_document.scene = 0;
}

void
gltfAnimation::findChildrenOfBone(const std::vector<BoneTreeNodeExternal>& boneTree, uint16_t boneIndex, std::vector<int32_t>& out)
{
	for (uint16_t x = 0; x < boneTree.size(); x++)
	{
		if (boneTree[x].parentIndex == boneIndex)
			out.push_back(x);
	}
}

void
gltfAnimation::modifyAsset()
{
	m_document.asset.generator = m_generatorName;
	m_document.asset.copyright = m_copyright;
}