#pragma once

#include "FileType.hpp"
#include "texture/ExtractorTexture.h"
#include "model/ExtractorModel.h"
#include "material/ExtractorMaterial.h"

#include <unordered_map>

namespace WarframeExporter
{
	class FileTypeMap
	{
		std::unordered_map<FileTypeExternal, Extractor&> m_map;

		FileTypeMap()
		{
			for (auto x : Texture::ExtractorTexture::getInstance().externalTypes())
				m_map.insert(std::pair<FileTypeExternal, Extractor&>{x, Texture::ExtractorTexture::getInstance()});
			for (auto x : Model::ExtractorModel::getInstance().externalTypes())
				m_map.insert(std::pair<FileTypeExternal, Extractor&>{x, Model::ExtractorModel::getInstance()});
			for (auto x : Material::ExtractorMaterial::getInstance().externalTypes())
				m_map.insert(std::pair<FileTypeExternal, Extractor&>{x, Material::ExtractorMaterial::getInstance()});
		}

	public:
		inline static FileTypeMap& getInstance()
		{
			static FileTypeMap instance;
			return instance;
		}
		inline Extractor& getExtractor(FileTypeExternal type)
		{
			return m_map.at(type);
		}
	};
}
