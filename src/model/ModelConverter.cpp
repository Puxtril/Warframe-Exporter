#include "model/ModelConverter.h"

using namespace WarframeExporter::Model;

void
ModelConverter::convertToInternal(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader, ModelBodyInternal& outBody)
{
    ModelConverter::flipXAxis(extBody);

    if (extHeader.boneTree.size() > 1)
        ModelConverter::convertInternalHeaderRigged(extHeader, extBody, attributes, outHeader);
    ModelConverter::convertInternalHeaderStaticOrRigged(extHeader, extBody, attributes, outHeader);

    if (extHeader.boneTree.size() > 1)
        ModelConverter::convertInternalBodyRigged(extHeader, extBody, outBody, outHeader.modelScale);
    ModelConverter::convertInternalBodyStaticOrRigged(extHeader, extBody, outBody, outHeader.modelScale);
}

void
ModelConverter::convertInternalHeaderRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader)
{
    // Create fast way to index bone in BoneTree using its name
    std::unordered_map<std::string, size_t> boneNameMap;
    for (size_t x = 0; x < extHeader.boneTree.size(); x++)
    {
        boneNameMap[extHeader.boneTree[x].name] = x;
    }
    // Re-create WeightedBoneNames as indices
    std::vector<size_t> weightedBoneIndices(0, extHeader.weightedBoneNames.size());
    for (size_t x = 0; x < extHeader.weightedBoneNames.size(); x++)
    {
        const std::string& boneName = extHeader.weightedBoneNames[x];
        weightedBoneIndices.push_back(boneNameMap[boneName]);
    }
    outHeader.weightedBones = weightedBoneIndices;

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
        size_t boneTreeIndex = outHeader.weightedBones[x];
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
ModelConverter::convertInternalHeaderStaticOrRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader)
{
    outHeader.vertexCount = extHeader.vertexCount;
    outHeader.boneCount = extHeader.boneCount;
    outHeader.faceCount = extHeader.faceCount;
    outHeader.morphCount = extHeader.morphCount;

    std::vector<std::string> materialNames = ModelConverter::extractMaterialNames(attributes);
    std::vector<MeshInfoInternal> intMeshInfos;
    for (size_t x = 0; x < extHeader.meshInfos.size(); x++)
    {
        MeshInfoExternal extMeshInfo = extHeader.meshInfos[x];
        MeshInfoInternal newMeshInfo;

        newMeshInfo.name = extMeshInfo.name;
        memcpy(newMeshInfo.faceLODOffsets.data(), extMeshInfo.faceLODOffsets.data(), 5 * 4);
        memcpy(newMeshInfo.faceLODCounts.data(), extMeshInfo.faceLODCounts.data(), 5 * 4);

        std::string matName = x < materialNames.size() ? materialNames[x] : "None";
        newMeshInfo.matName = matName;

        intMeshInfos.push_back(newMeshInfo);
    }
    outHeader.meshInfos = intMeshInfos;

    outHeader.errorMsgs = extHeader.errorMsgs;
    getModelScale(extHeader.meshInfos, outHeader.modelScale);
}

void
ModelConverter::convertInternalBodyRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, const glm::vec3& modelScale)
{
    //outBody.setRevserseBinds(extBody.reverseBinds);
    /*
    std::vector<glm::mat4> flippedMatrices = std::move(extBody.reverseBinds);
    for (size_t x = 0; x < flippedMatrices.size(); x++)
    {
        //flippedMatrices[x] = glm::scale(flippedMatrices[x], glm::vec3(1, -1, 1));
        flippedMatrices[x][3] = { 0.0F, 0.0F, 0.0F, 1.0F };
    }
    outBody.setRevserseBinds(flippedMatrices);
    */
}

void
ModelConverter::convertInternalBodyStaticOrRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, const glm::vec3& modelScale)
{
    outBody.indices = extBody.indices;

    // Apply global model scale
    std::vector<glm::vec3> newPositions;
    newPositions.resize(extBody.positions.size());
    for (size_t x = 0; x < extBody.positions.size(); x++)
    {
        newPositions[x] = extBody.positions[x] * modelScale;
    }
    outBody.positions = newPositions;
    outBody.UV1 = extBody.UV1;
    outBody.UV2 = extBody.UV2;
    outBody.boneWeights = extBody.boneWeights;

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
ModelConverter::flipXAxis(ModelBodyExternal& extBody)
{
    for (auto& x : extBody.reverseBinds)
    {
        x = glm::transpose(glm::scale(glm::transpose(x), { -1, 1, 1 }));
    }
    for (auto& x : extBody.boneRotations)
    {
        x.x *= -1;
        x.y *= -1;
    }
    for (auto& x : extBody.bonePositions)
    {
        x.x *= -1;
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
ModelConverter::getModelScale(const std::vector<MeshInfoExternal>& meshInfos, glm::vec3& outScale)
{
    outScale.x = 0.0;
    outScale.y = 0.0;
    outScale.z = 0.0;

    for (int i = 0; i < 3; i++)
    {
        for (MeshInfoExternal curMeshInfo : meshInfos)
        {
            float largerOf2 = std::max(std::abs(curMeshInfo.vector1[i]), std::abs(curMeshInfo.vector2[i]));
            outScale[i] = std::max(outScale[i], largerOf2);
        }
    }
}
