#include "model/ModelConverter.h"

using namespace WarframeExporter::Model;

void
ModelConverter::convertToInternal(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, std::vector<std::vector<glm::u8vec4>> vertexColors, ModelHeaderInternal& outHeader, ModelBodyInternal& outBody, ScaleType scaleType, const std::string& internalPath)

{
    if (extHeader.boneTree.size() > 1)
        ModelConverter::convertInternalHeaderRigged(extHeader, extBody, outHeader);
    ModelConverter::convertInternalHeaderStaticOrRigged(extHeader, attributes, outHeader, internalPath);

    getModelScale(extHeader.meshInfos, extHeader.ensmallening1, extHeader.ensmallening2, scaleType, outHeader.modelScale);
    ModelConverter::convertInternalBodyStaticOrRigged(extHeader, extBody, outBody, vertexColors, outHeader.modelScale);
}

void
ModelConverter::convertInternalHeaderRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelHeaderInternal& outHeader)
{
    // Create fast way to index bone in BoneTree using its name
    std::unordered_map<std::string, int> boneNameMap;
    for (int x = 0; x < (int)extHeader.boneTree.size(); x++)
    {
        boneNameMap[extHeader.boneTree[x].name] = x;
    }

    // Re-create WeightedBoneNames as indices
    outHeader.weightedBones.resize(extHeader.weightedBoneNames.size());
    for (int x = 0; x < (int)extHeader.weightedBoneNames.size(); x++)
    {
        const std::string& boneName = extHeader.weightedBoneNames[x];
        outHeader.weightedBones[x] = (boneNameMap[boneName]);
    }

    std::vector<BoneTreeNodeInternal> intNodes;
    for (size_t x = 0; x < extHeader.boneTree.size(); x++)
    {
        BoneTreeNodeExternal extNode = extHeader.boneTree[x];
        BoneTreeNodeInternal newNode;

        newNode.name = extNode.name;
        newNode.parentIndex = extNode.parentIndex;
        newNode.rotation = extBody.boneRotations[x];
        newNode.position = extBody.bonePositions[x];
        
        intNodes.push_back(newNode);
    }
    // Prevent invinite loop, by default the top-most node has parent index of 0
    if (intNodes.size() > 1 && intNodes[0].parentIndex == 0)
        intNodes[0].parentIndex = -1;
    
    std::vector<bool> bonesSetInverseBinds(intNodes.size(), false);
    // Set Inverse Binds for Weighted Bones
    for (size_t x = 0; x < outHeader.weightedBones.size(); x++)
    {
        glm::mat4& matrix = extBody.reverseBinds[x];
        int boneTreeIndex = outHeader.weightedBones[x];
        intNodes[boneTreeIndex].reverseBind = matrix;
        bonesSetInverseBinds[boneTreeIndex] = true;
    }

    static const glm::mat4 identity = {
        1.0F, 0.0F, 0.0F, 0.0F,
        0.0F, 1.0F, 0.0F, 0.0F,
        0.0F, 0.0F, 1.0F, 0.0F,
        0.0F, 0.0F, 0.0F, 1.0F,
    };
    // Set Inverse Binds for Non-weighted bones
    for (size_t x = 0; x < intNodes.size(); x++)
    {
        if (!bonesSetInverseBinds[x])
        {
            glm::mat4 indentityCopy = glm::mat4(identity);
            intNodes[x].reverseBind = indentityCopy;
        }
    }
    outHeader.boneTree = intNodes;
}

void
ModelConverter::convertInternalHeaderStaticOrRigged(ModelHeaderExternal& extHeader, const std::string& attributes, ModelHeaderInternal& outHeader, const std::string& internalPath)
{
    outHeader.vertexCount = extHeader.vertexCount;
    outHeader.boneCount = extHeader.boneCount;
    outHeader.faceCount = extHeader.faceCount;
    outHeader.morphCount = extHeader.morphCount;

    std::vector<std::string> materialNames = extHeader.materialPaths.size() > 0 ? extHeader.materialPaths : ModelConverter::extractMaterialNames(attributes);

    std::vector<MeshInfoInternal> intMeshInfos;
    for (size_t x = 0; x < extHeader.meshInfos.size(); x++)
    {
        MeshInfoExternal extMeshInfo = extHeader.meshInfos[x];
        MeshInfoInternal newMeshInfo;

        newMeshInfo.name = extMeshInfo.name;
        memcpy(newMeshInfo.faceLODOffsets.data(), extMeshInfo.faceLODOffsets.data(), 5 * 4);
        memcpy(newMeshInfo.faceLODCounts.data(), extMeshInfo.faceLODCounts.data(), 5 * 4);

        std::string matName = x < materialNames.size() ? materialNames[x] : "None";
        if (matName[0] != '/' && matName != "None")
            newMeshInfo.matName = (std::filesystem::path(internalPath).parent_path() / matName).string();
        else
            newMeshInfo.matName = matName;

        intMeshInfos.push_back(newMeshInfo);
    }
    outHeader.meshInfos = intMeshInfos;

    outHeader.errorMsgs = extHeader.errorMsgs;
}

void
ModelConverter::convertInternalBodyStaticOrRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, std::vector<std::vector<glm::u8vec4>> vertexColors, const glm::vec4& modelScale)
{
    outBody.indices = extBody.indices;

    // Apply global model scale
    std::vector<glm::vec3> newPositions;
    newPositions.resize(extBody.positions.size());
    for (size_t x = 0; x < extBody.positions.size(); x++)
    {
        newPositions[x][0] = extBody.positions[x][0] * modelScale[0];
        newPositions[x][1] = extBody.positions[x][1] * modelScale[1];
        newPositions[x][2] = extBody.positions[x][2] * modelScale[2];
    }
    outBody.positions = std::move(newPositions);
    outBody.UV1 = std::move(extBody.UV1);
    outBody.UV2 = std::move(extBody.UV2);
    outBody.boneWeights = std::move(extBody.boneWeights);

    outBody.colors = std::move(extBody.colors);
    for (auto& col : vertexColors)
        outBody.colors.push_back(col);
    outBody.AO = std::move(extBody.AO);

    // Convert local bone indices to global
    std::vector<glm::u16vec4>& newIndices = outBody.boneIndices;
    newIndices.resize(extBody.boneIndices.size());
    for (size_t x = 0; x < extHeader.boneMaps.size(); x++)
    {
        MeshInfoExternal curMeshInfo = extHeader.meshInfos[x];
        std::vector<uint32_t> curBoneMap = extHeader.boneMaps[x];
        for (uint32_t iface = curMeshInfo.faceLODOffsets[0]; iface < curMeshInfo.faceLODOffsets[0] + curMeshInfo.faceLODCounts[0]; iface++)
        {
            uint32_t iVertex = outBody.indices[iface];
            for (uint32_t iCurBone = 0; iCurBone < 4; iCurBone++)
            {
                if (outBody.boneWeights[iVertex][iCurBone] != 0.0F)
                {
                    uint8_t localBoneIndex = extBody.boneIndices[iVertex][iCurBone];
                    // Usually the packager is good about keping locals within the range of the first meshInfo bonemap.
                    // On the rare case it's out of range, the next meshinfo should assign the correct global index.
                    if (localBoneIndex >= curBoneMap.size())
                        continue;
                    newIndices[iVertex][iCurBone] = curBoneMap[localBoneIndex];
                }
            }
        }
    }
}

void
ModelConverter::mirrorX(ModelHeaderInternal& intHeader, ModelBodyInternal& intBody)
{
    // Rigged models
    for (auto& curBone : intHeader.boneTree)
    {
        curBone.reverseBind = glm::transpose(glm::scale(glm::transpose(curBone.reverseBind), { -1, 1, 1 }));
        curBone.rotation.x *= -1;
        curBone.rotation.y *= -1;
        curBone.position.x *= -1;
    }
    
    // Static models
    if (intHeader.boneTree.empty())
    {
        for (glm::vec3& curPos : intBody.positions)
        {
            curPos.z *= -1.0f;
        }
    }
}

std::vector<std::string>
ModelConverter::extractMaterialNames(const std::string& attributes)
{
    std::smatch matches;
    std::regex firstPattern(R"((?:Materials=\{\n)(?:    ([a-zA-Z0-9/]+),?\n))");
    std::regex pattern(R"(    ([a-zA-Z0-9/]+),?\n)");

    std::vector<std::string> matNames;
    std::string::const_iterator searchCursor(attributes.cbegin());
    std::regex curPattern = firstPattern;
    while (std::regex_search(searchCursor, attributes.cend(), matches, curPattern))
    {
        if (matches[1].matched)
            matNames.push_back(matches[1].str());
        searchCursor = matches.suffix().first;
        curPattern = pattern;
    }
    
    return matNames;
}

void
ModelConverter::getModelScale(const std::vector<MeshInfoExternal>& meshInfos, glm::vec4 ensmall1, glm::vec4 ensmall2, ScaleType scaleType, glm::vec4& outScale)
{
    outScale.x = 0.0;
    outScale.y = 0.0;
    outScale.z = 0.0;
    outScale.w = 0.0;

    for (int i = 0; i < 4; i++)
    {
        for (MeshInfoExternal curMeshInfo : meshInfos)
        {
            if (scaleType == ScaleType::XYZ)
            {
                float largerOf2 = std::max(std::abs(ensmall1[i]), std::abs(ensmall2[i]));
                outScale[i] = std::max(outScale[i], largerOf2);
            }
            else if (scaleType == ScaleType::XZ)
            {
                // Skip Y axis
                if (i == 1)
                {
                    outScale[i] = 1.0;
                    continue;
                }

                float largerOf2 = std::max(std::abs(curMeshInfo.vector1[i]), std::abs(curMeshInfo.vector2[i]));
                outScale[i] = largerOf2 * 2;
            }
        }
    }
	
	// Because the scale might be 0...
	outScale.x = (outScale.x == 0.0f) ? 1.0f : outScale.x;
	outScale.y = (outScale.y == 0.0f) ? 1.0f : outScale.y;
	outScale.z = (outScale.z == 0.0f) ? 1.0f : outScale.z;
    // Intentionally commented out
    //outScale.w = (outScale.w == 0.0f) ? 1.0f : outScale.w;
}