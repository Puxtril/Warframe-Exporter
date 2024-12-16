#include "ui/FormatPreview1.h"

FormatPreview::FormatPreview()
    : m_parentWidget(nullptr), m_layout(nullptr)
{ }

void
FormatPreview::setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    m_parentWidget = parentWidget;
    m_layout = parentLayout;
}

void
FormatPreview::setData(LotusLib::PackagesReader* pkgs, const std::string& pkgName, const LotusLib::LotusPath& internalPath)
{
    LotusLib::FileEntry fileEntry = pkgs->getPackage(pkgName).value().getFile(internalPath, LotusLib::READ_COMMON_HEADER | LotusLib::READ_H_CACHE | LotusLib::READ_F_CACHE | LotusLib::READ_B_CACHE);
    WarframeExporter::Extractor* extractor = WarframeExporter::g_enumMapExtractor[fileEntry.commonHeader.type];
    
    if (extractor == nullptr)
        return;

    QPlainTextEdit* textEdit = new QPlainTextEdit(m_parentWidget);
    m_layout->addWidget(textEdit);
    std::stringstream outStr;

    try
    {
        switch(extractor->getExtractorType())
        {
            case WarframeExporter::ExtractorType::Model:
                setupModel(outStr, pkgs, fileEntry);
                break;
            case WarframeExporter::ExtractorType::Texture:
                setupTexture(outStr, pkgs, fileEntry);
                break;
            case WarframeExporter::ExtractorType::Level:
                setupLevel(outStr, fileEntry);
                break;
            case WarframeExporter::ExtractorType::Audio:
                setupAudio(outStr, fileEntry);
                break;
            case WarframeExporter::ExtractorType::Shader:
                setupShader(outStr, fileEntry);
                break;
            case WarframeExporter::ExtractorType::Landscape:
                setupLandscape(outStr, fileEntry);
                break;
            default:
                break;
        }
    }
    catch (std::exception& ex)
    {
        outStr.clear();
        outStr << "Error: " << ex.what() << std::endl;
    }

    std::string outStrTmp = outStr.str();
    textEdit->setPlainText(QString(outStrTmp.c_str()));
}

void
FormatPreview::clearPreview()
{
    for (QWidget* x : m_parentWidget->findChildren<QWidget*>(Qt::FindDirectChildrenOnly))
        delete x;
}

void
FormatPreview::setupModel(std::stringstream& outStr, LotusLib::PackagesReader* pkgs, LotusLib::FileEntry& fileEntry)
{
    WarframeExporter::Model::ModelHeaderExternal headerExt;
    WarframeExporter::Model::ModelBodyExternal bodyExt;
    WarframeExporter::Model::ModelExtractor::getInstance()->extractExternal(fileEntry, {true, true, true}, headerExt, bodyExt);

    if (headerExt.meshInfos.size() == 0)
    {
        outStr << "Error: Model has 0 meshinfos" << std::endl;
    }
    else
    {
        std::optional<LotusLib::PackageReader> pkg = pkgs->getPackage("Misc");
        auto vertexColors = WarframeExporter::Model::ModelExtractor::getInstance()->getVertexColors(fileEntry.internalPath, pkg.value());

        // Convert body/header data into internal format
        WarframeExporter::Model::ModelHeaderInternal headerInt;
        WarframeExporter::Model::ModelBodyInternal bodyInt;
        WarframeExporter::Model::ModelConverter::convertToInternal(headerExt, bodyExt, fileEntry.commonHeader.attributes, vertexColors, headerInt, bodyInt, WarframeExporter::Model::g_enumMapModel[fileEntry.commonHeader.type]->ensmalleningScale());
    
        outStr << "Vertices: " << headerInt.vertexCount << std::endl;
        outStr << "Total faces: " << headerInt.faceCount / 3 << std::endl;
        outStr << "Bones: " << headerInt.boneCount << std::endl;
        outStr << "Ensmallening scale: (" << headerInt.modelScale[0] << ", " << headerInt.modelScale[1] << ", " << headerInt.modelScale[2] << ")" << std::endl;

        outStr << std::endl;
        outStr << "Submeshes" << std::endl;
        outStr << "-----------" << std::endl;
        for (const WarframeExporter::Model::MeshInfoExternal& submesh : headerExt.meshInfos)
            outStr << submesh.name << " | " << "Faces: " << submesh.faceLODCounts[0] / 3 << std::endl;
        
        if (headerExt.physXMeshes.size() > 0)
        {
            outStr << std::endl;
            outStr << "PhysX Meshes " << std::endl;
            outStr << "-----------" << std::endl;
            for (const WarframeExporter::Model::PhysXMesh& physx : headerExt.physXMeshes)
                outStr << "Type: " << physx.typeEnum << ", Length: " << physx.dataLength << std::endl;
        }

        if (headerInt.errorMsgs.size() > 0)
        {
            outStr << std::endl;
            outStr << "Errors " << std::endl;
            outStr << "-------" << std::endl;
            for (const std::string& errMsg : headerInt.errorMsgs)
                outStr << errMsg;
        }
    }
}

void
FormatPreview::setupTexture(std::stringstream& outStr, LotusLib::PackagesReader* pkgs, LotusLib::FileEntry& fileEntry)
{
    BinaryReader::BinaryReaderBuffered& entry = fileEntry.fData.getLength() != 0 ? fileEntry.fData : fileEntry.bData;
    WarframeExporter::Texture::TextureHeaderExternal extHeader = WarframeExporter::Texture::TextureReader::readHeader(&fileEntry.headerData, fileEntry.commonHeader, {true, true, true});

    bool knownFormat = WarframeExporter::Texture::internalFormatToDdsFormat.count(static_cast<WarframeExporter::Texture::TextureCompression>(extHeader.format)) != 0;
    WarframeExporter::Texture::TextureInternal intTexture;
    if (knownFormat)
    {
        intTexture.header = WarframeExporter::Texture::TextureConverter::convertHeader(extHeader, static_cast<int32_t>(entry.getLength()), fileEntry.commonHeader.type);
        intTexture.body = WarframeExporter::Texture::TextureReader::readBody(&entry, extHeader);
    }
    
    std::string formatName = "";
    switch(intTexture.header.formatEnum)
    {
        case WarframeExporter::Texture::TextureCompression::Default:
        case WarframeExporter::Texture::TextureCompression::BC1:
            formatName = "BC1";
            break;
        case WarframeExporter::Texture::TextureCompression::BC2:
            formatName = "BC2";
            break;
        case WarframeExporter::Texture::TextureCompression::BC3:
            formatName = "BC3";
            break;
        case WarframeExporter::Texture::TextureCompression::BC4:
            formatName = "BC4";
            break;
        case WarframeExporter::Texture::TextureCompression::BC5:
            formatName = "BC5";
            break;
        case WarframeExporter::Texture::TextureCompression::BC6:
            formatName = "BC7";
            break;
        case WarframeExporter::Texture::TextureCompression::Uncompressed:
            formatName = "R8G8B8A8";
            break;
        case WarframeExporter::Texture::TextureCompression::A8:
            formatName = "A8";
            break;
        case WarframeExporter::Texture::TextureCompression::R16:
            formatName = "R16";
            break;
        default:
            formatName = "Unknown";
            break;
    }  

    outStr << "Compression: " << (int)extHeader.format << " (" << formatName << ")" << std::endl;
    outStr << "Resolution (Raw): " << extHeader.widthBase << "x" << extHeader.heightBase << std::endl;
    if (knownFormat)
        outStr << "Resolution (Adjusted): " << intTexture.header.width << "x" << intTexture.header.height << std::endl;

    if (knownFormat && intTexture.header.textureNames.size() > 0)
    {
        outStr << std::endl;
        outStr << "Subtextures" << std::endl;
        outStr << "------------" << std::endl;
        for (const std::string& name : intTexture.header.textureNames)
            outStr << "  " << name << std::endl;
    }
}

void
FormatPreview::setupLevel(std::stringstream& outStr, LotusLib::FileEntry& fileEntry)
{
    WarframeExporter::Level::LevelExternal extLevel = WarframeExporter::Level::LevelExtractor::getInstance()->getLevelExternal(fileEntry);

    outStr << "Has Landscape: " << (extLevel.landscapeIndex > 0) << std::endl;
    outStr << "Meshes: " << extLevel.header.levelObjs.size() << std::endl;
    outStr << "Paths: " << extLevel.header.paths.size() << std::endl;

    if (extLevel.header.actionPaths.size() > 0)
    {
        outStr << std::endl;
        outStr << "Actions" << std::endl;
        outStr << "(Unused by extractor)" << std::endl;
        outStr << "-------" << std::endl;
        for (const std::string& action : extLevel.header.actionPaths)
            outStr << "  " << action << std::endl;
    }

    if (extLevel.header.assetPaths.size() > 0)
    {
        outStr << std::endl;
        outStr << "Assets" << std::endl;
        outStr << "(Unused by extractor)" << std::endl;
        outStr << "------" << std::endl;
        for (const std::string& action : extLevel.header.assetPaths)
            outStr << "  " << action << std::endl;
    }
}

void
FormatPreview::setupAudio(std::stringstream& outStr, LotusLib::FileEntry& fileEntry)
{
    WarframeExporter::Audio::AudioCompression compression = WarframeExporter::Audio::AudioExtractorProxy::getInstance()->peekCompressionFormat(&fileEntry.headerData);
    
    WarframeExporter::Audio::AudioHeader audioHeader;
    std::string compressionName = "";
    switch(compression)
    {
        case WarframeExporter::Audio::AudioCompression::ADPCM:
        {
            compressionName = "ADPCM";
            WarframeExporter::Audio::AudioReader* reader = WarframeExporter::Audio::g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
            reader->readHeader(&fileEntry.headerData, {true, true, true}, fileEntry.commonHeader, audioHeader);
            break;
        }
        case WarframeExporter::Audio::AudioCompression::PCM:
        {
            compressionName = "PCM";
            WarframeExporter::Audio::AudioReader* reader = WarframeExporter::Audio::g_enumMapAudioPCMReader[fileEntry.commonHeader.type];
            reader->readHeader(&fileEntry.headerData, {true, true, true}, fileEntry.commonHeader, audioHeader);
            break;
        }
        case WarframeExporter::Audio::AudioCompression::OPUS:
        {
            compressionName = "Opus";
            WarframeExporter::Audio::AudioReader* reader = WarframeExporter::Audio::g_enumMapAudioOpusReader[fileEntry.commonHeader.type];
            reader->readHeader(&fileEntry.headerData, {true, true, true}, fileEntry.commonHeader, audioHeader);
            break;
        }
        default:
            compressionName = "Unknown";
            break;
    }
    
    outStr << "Compression: " << (int)compression << " (" << compressionName << ")" << std::endl;
    outStr << "Stream Serial: " << audioHeader.streamSerialNumber << std::endl;
    outStr << "Channels: " << audioHeader.channelCount << std::endl;
    outStr << "Samples per second: " << audioHeader.samplesPerSec << std::endl;
    outStr << "Bits per sample: " << audioHeader.bitsPerSample << std::endl;
    outStr << "Average bytes per second: " << audioHeader.averageBytesPerSecond << std::endl;
    outStr << "Block allignment: " << audioHeader.blockAllign << std::endl;
    outStr << "Samples per block: " << audioHeader.samplesPerBlock << std::endl;
}

void
FormatPreview::setupShader(std::stringstream& outStr, LotusLib::FileEntry& fileEntry)
{
    WarframeExporter::Shader::ShaderReader* shaderReader = WarframeExporter::Shader::g_enumMapShader[fileEntry.commonHeader.type];
    WarframeExporter::Shader::ShaderHeaderExternal shaderHeader = shaderReader->readHeader(&fileEntry.headerData, fileEntry.commonHeader.type);

    outStr << "Shader count: " << shaderHeader.shaderCount << std::endl;
}

void
FormatPreview::setupLandscape(std::stringstream& outStr, LotusLib::FileEntry& fileEntry)
{
    WarframeExporter::Landscape::LandscapeReader* reader = WarframeExporter::Landscape::g_enumMapLandscape[fileEntry.commonHeader.type];
    WarframeExporter::Landscape::LandscapeHeaderExternal external = reader->readHeader(&fileEntry.headerData);

    outStr << "Grid size: " << external.rowCount << "x" << external.columnCount << std::endl;
    outStr << "Chunk count: " << external.chunks.size() << std::endl;
}

