#include "model/ModelConverter.h"

using namespace WarframeExporter::Model;

void
ModelConverter::convertToInternal(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, std::vector<VertexColorBody> vColors, ModelHeaderInternal& outHeader, ModelBodyInternal& outBody)
{
    ModelConverter::flipXAxis(extBody);
    ModelConverter::overwriteColors(extBody.getColorPtr());

    if (extHeader.getBoneTree().size() > 1)
        ModelConverter::convertInternalHeaderRigged(extHeader, extBody, attributes, outHeader);
    ModelConverter::convertInternalHeaderStaticOrRigged(extHeader, extBody, attributes, outHeader);

    if (extHeader.getBoneTree().size() > 1)
        ModelConverter::convertInternalBodyRigged(extHeader, extBody, outBody, outHeader.getModelScale());
    ModelConverter::convertInternalBodyStaticOrRigged(extHeader, extBody, outBody, outHeader.getModelScale(), vColors);
}

void
ModelConverter::convertInternalHeaderRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader)
{
    // Create fast way to index bone in BoneTree using its name
    std::unordered_map<std::string, size_t> boneNameMap;
    for (size_t x = 0; x < extHeader.getBoneTree().size(); x++)
    {
        boneNameMap[extHeader.getBoneTree()[x].getName()] = x;
    }
    // Re-create WeightedBoneNames as indices
    std::vector<size_t> weightedBoneIndices(0, extHeader.getWeightedBoneNames().size());
    for (size_t x = 0; x < extHeader.getWeightedBoneNames().size(); x++)
    {
        const std::string& boneName = extHeader.getWeightedBoneNames()[x];
        weightedBoneIndices.push_back(boneNameMap[boneName]);
    }
    outHeader.setWeightedBones(weightedBoneIndices);

    std::vector<BoneTreeNodeInternal> intNodes;
    for (size_t x = 0; x < extHeader.getBoneTree().size(); x++)
    {
        BoneTreeNodeExternal extNode = extHeader.getBoneTree()[x];
        BoneTreeNodeInternal newNode;

        newNode.setName(extNode.getName());
        newNode.setParentIndex(extNode.getParentIndex());
        newNode.setRotation(extBody.getBoneRotationsPtr()[x]);
        newNode.setPosition(extBody.getBonePositionsPtr()[x]);
        
        intNodes.push_back(newNode);
    }
    // Prevent invinite loop, by default the top-most node has parent index of 0
    if (intNodes.size() > 1 && intNodes[0].getParentIndex() == 0)
        intNodes[0].setParentIndex(-1);
    
    std::vector<bool> bonesSetInverseBinds(intNodes.size(), false);
    // Set Inverse Binds for Weighted Bones
    for (size_t x = 0; x < outHeader.getWeightedBones().size(); x++)
    {
        glm::mat4& matrix = extBody.getReverseBindsPtr()[x];
        size_t boneTreeIndex = outHeader.getWeightedBones()[x];
        intNodes[boneTreeIndex].setReverseBind(matrix);
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
            intNodes[x].setReverseBind(indentityCopy);
        }
    }
    outHeader.setBoneTree(intNodes);
}

void
ModelConverter::convertInternalHeaderStaticOrRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader)
{
    outHeader.setVertexCount(extHeader.getVertexCount());
    outHeader.setBoneCount(extHeader.getBoneCount());
    outHeader.setFaceCount(extHeader.getFaceCount());
    outHeader.setMorphCount(extHeader.getMorphCount());

    std::vector<std::string> materialNames = ModelConverter::extractMaterialNames(attributes);
    std::vector<MeshInfoInternal> intMeshInfos;
    for (size_t x = 0; x < extHeader.getMeshInfos().size(); x++)
    {
        MeshInfoExternal extMeshInfo = extHeader.getMeshInfos()[x];
        MeshInfoInternal newMeshInfo;

        newMeshInfo.setName(extMeshInfo.getName());
        memcpy(newMeshInfo.getLODOffsetsPtr().data(), extMeshInfo.getLODOffsets().data(), 5 * 4);
        memcpy(newMeshInfo.getLODCountsPtr().data(), extMeshInfo.getLODCounts().data(), 5 * 4);

        std::string matName = x < materialNames.size() ? materialNames[x] : "None";
        newMeshInfo.setMaterialName(matName);

        intMeshInfos.push_back(newMeshInfo);
    }
    outHeader.setMeshInfos(intMeshInfos);

    outHeader.setErrorMsgs(extHeader.getErrorMsgs());
    getModelScale(extHeader.getMeshInfos(), outHeader.getModelScalePtr());
}

void
ModelConverter::convertInternalBodyRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, const glm::vec3& modelScale)
{
    //outBody.setRevserseBinds(extBody.getReverseBindsPtr());
    /*
    std::vector<glm::mat4> flippedMatrices = std::move(extBody.getReverseBindsPtr());
    for (size_t x = 0; x < flippedMatrices.size(); x++)
    {
        //flippedMatrices[x] = glm::scale(flippedMatrices[x], glm::vec3(1, -1, 1));
        flippedMatrices[x][3] = { 0.0F, 0.0F, 0.0F, 1.0F };
    }
    outBody.setRevserseBinds(flippedMatrices);
    */
}

void
ModelConverter::convertInternalBodyStaticOrRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, const glm::vec3& modelScale, std::vector<VertexColorBody> vColors)
{
    outBody.setIndices(extBody.getIndexPtr());

    // Apply global model scale
    std::vector<glm::vec3> newPositions;
    newPositions.resize(extBody.getPositions().size());
    for (size_t x = 0; x < extBody.getPositions().size(); x++)
    {
        newPositions[x] = extBody.getPositions()[x] * modelScale;
    }
    outBody.setPositions(newPositions);

    std::vector<std::vector<glm::u8vec4>> vertColors(1 + vColors.size());
    vertColors[0] = std::move(extBody.getColorPtr());
    for (int x = 0; x < vColors.size(); x++)
        vertColors[x + 1] = std::move(vColors[x].getColorsPtr());
    outBody.setColors(vertColors);

    outBody.setUV1(extBody.getUV1Ptr());
    outBody.setUV2(extBody.getUV2Ptr());
    outBody.setBoneWeights(extBody.getBoneWeightsPtr());

    // Convert local bone indices to global
    std::vector<glm::u16vec4>& newIndices = outBody.getBoneIndexPtr();
    newIndices.resize(extBody.getBoneIndices().size());
    for (size_t x = 0; x < extHeader.getBoneMaps().size(); x++)
    {
        MeshInfoExternal curMeshInfo = extHeader.getMeshInfos()[x];
        std::vector<uint32_t> curBoneMap = extHeader.getBoneMaps()[x];
        for (uint32_t iface = curMeshInfo.getLODOffsets()[0]; iface < curMeshInfo.getLODOffsets()[0] + curMeshInfo.getLODCounts()[0]; iface++)
        {
            uint32_t iVertex = outBody.getIndices()[iface];
            for (uint32_t iCurBone = 0; iCurBone < 4; iCurBone++)
            {
                if (outBody.getBoneWeights()[iVertex][iCurBone] != 0.0F)
                {
                    uint8_t localBoneIndex = extBody.getBoneIndices()[iVertex][iCurBone];
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
ModelConverter::overwriteColors(std::vector<glm::u8vec4>& colors)
{
    for (auto& x : colors)
    {
        x.r = x.a;
        x.g = x.a;
        x.b = x.a;
    }
}

void
ModelConverter::flipXAxis(ModelBodyExternal& extBody)
{
    for (auto& x : extBody.getReverseBindsPtr())
    {
        x = glm::transpose(glm::scale(glm::transpose(x), { -1, 1, 1 }));
    }
    for (auto& x : extBody.getBoneRotationsPtr())
    {
        x.x *= -1;
        x.y *= -1;
    }
    for (auto& x : extBody.getBonePositionsPtr())
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
            float largerOf2 = std::max(std::abs(curMeshInfo.getVec1()[i]), std::abs(curMeshInfo.getVec2()[i]));
            outScale[i] = std::max(outScale[i], largerOf2);
        }
    }
}