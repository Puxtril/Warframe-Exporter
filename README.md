# Level Branch

Unfortunately not much progress has been made here. I saw a bunch of integers listed in the body file for levels and could not find what they reference.

# Warframe Extractor

A cross-platform program/library for exporting files from Warframe. Provides a command-line interface for users, can be compiled into a library for programmers. "Easily" extensible to support new formats and upgrades to existing formats.

Formats currently supported:
- Textures -> DDS
- 3D Models (mostly) -> glTF

Formats with started progress:
- Animation -> glTF
- Levels -> glTF (presumably)

# How to use

Download the latest from the Releases section.
#TODO

# External Libraries

* [spdlog](https://github.com/gabime/spdlog): Logging
* [Binary-Reader](https://github.com/Protinon/Binary-Reader) Read binary data into primitive data types.
* [LotusLib](https://github.com/Puxtril/LotusLib): Provides an interface for the Warframe files. [See requirements for Oodle](https://github.com/Puxtril/LotusLib#how-to-install).
* [ddspp](https://github.com/redorav/ddspp): For exporting textures as DDS
* [fx-gltf](https://github.com/jessey-git/fx-gltf): For exprting models as [glTF](https://en.wikipedia.org/wiki/GlTF).
  * [nlohmann-json](https://github.com/nlohmann/json): Required by fx-gltf.
* [glm](https://github.com/g-truc/glm): A helper library for models. Provides matrix and vector math operations.
* [TCLAP](https://sourceforge.net/projects/tclap/): Command-line interface library

# Overview of this library

I will split this up into 3 sections, each subsequent section being more verbose. For more information on the cache file structure, read the [LotusLib documentation](https://github.com/Puxtril/LotusLib#documentation).

## High-level overview

This project converts the data from the Warframe cache files into usable formats. A command-line interface is provided (break out your CMD skills), though programmers may compile this into a library for use in other projects. It also supports debugging because let's be real, there will be new versions of exsting formats added, and that needs to be detectable.

## Medium-level overview

This is best explained by going over the data flow. To iterate over a group of files within the cache, you can choose between exporting and debugging the files. When a file is reached that is supported by an extractor, it begins the extraction process. Each extractor follows a similar flow.

Extraction process:
1. Parse the file contents into usable data (integer, floats, strings, etc).
1. Convert that data into a compatable format for later processing. This is an intermediate stage between reading and exporting into another format. We want both stages to be independant of each other.
1. Export the data into a standard format that can be read by other programs like Blender. Highly encouraged to use an external library for this.

During the exporting process, log all errors to a file.

Debug process: Parse the file contents and set certian checkpoints. If a checkpoint is reached and the data is not expected, assume the file is not fully supported. Log this information to a file. During this process, log additional information as files are iterated over.

## Low-level overview

This is best explained by orgaizing the project into separate sections.

* Iterator: The base class `BatchIterator` is sublassed by `BatchIteratorDebug` and `BatchIteratorExport`. Both are self-explanitory. The main difference is `BatchIteratorDebug` will not write extracted files to disk.
* Enum Map: File types are identified by an enumeration. The goal of this section is to map an enumeration to an object that processes the data. Because some formats like textures may have multiple enumerations that can be processed the exact same way. `EnumMap` is the mapping between an enumeration and the object processor; must be a sublass of `EnumMapValue`. `EnumMapValue` is the abstract class for objects to process enumertaions; these subclasses should also be abstract. This class has only 1 required method: return the array of associated enumerations. `Extractor` is an abtract sublcass of `EnumMapValue` that provides an interface for all extractors. `EnumMapExtractor` is the instanciated `EnumMap` using class `Extractor`. Because each Iterator needs access to this, it made sense to keep this separate. There may be other enum maps in this project, but the Extactor map was used as an example.
* File Properties: Files stored in the cache have a filetime (DOS time) associated with them. To make extraction easier between subsequent runs, only write files with different modification times. Unforunately reading/writing files from disk is OS-specific, thus this class.
* Ensmallening: Provides a simple interface for extractors to modfiy behavior based on the ensmallening change.

I will also gloss over the previously mentioned "Extraction process". Each extractor should have their own folder in `include` and `src`. Within those folders I've named files in a specific pattern.

* The main extractor file that inherits from `Extractor` should be named `<Name>Extractor`, ex. `TextureExtractor`.
* The first stage is reading from the cache: `TextureReader`.
* Next is converting the data into a compatable format: `TextureConverter`.
* The structs to store the previously mentioned data is defined in `TextureStructs`.
* Finally, the data is exported into a standard format: `TextureExporterDDS`.

# Contributions

## Adding New Extractor

If you indend to add a new format such as Animation, Maps, Audio, etc.

1. Create a new branch (if it doesn't already exist).
1. Create a new folder in *include* and *src*. Store all subsequent files in here.
1. Create a new class that inherits from the Extractor class (in `Extractor.h`).
1. Register the class inside `EnumMapExtractor.h`.
1. Add documentation below and at the beginning of this README.

## Adding a new 3D Model format

1. Add the new enum value to `WarframeExporter::Model::ModelType` (inside `model/ModelReader.h`)
1. Add the enum value to `WarframeExporter::Model::ModelExtractor::getEnumMapKeys()` (inside `model/ModelExtractor.h`)
1. Add a new reader class in the directory `model/types`. You should copy an existing class - the new format will likely be very similar to a previous one.
1. Register the class to `WarframeExporter::Model::g_enumMapModel` (inside `model/ModelEnumMap.h`)

## Adding a new Texture Format

1. Add the new enum value to `WarframeExporter::Texture::TextureType` (inside `texture/TextureExtractor.h`)
1. Add the enum value to `WarframeExporter::Texture::TextureExtractor::getEnumMapKeys()` (inside `texture/TextureExtractor.h`)

## Adding a new Texture Compression Format
This is very unlikely to occur. They recently started using BC6 and BC7, so there's no more compression formats they could use. Unless they add another uncompressed format.

1. Add the new enum value to `WarframeExporter::Texture::TextureCompression` (inside `texture/TextureInfo.h`)
1. Add a new class inside `texture/TextureInfos.h`.
1. Register the new class to `WarframeExporter::Texture::g_enumMapTexture` (inside `texture/TextureEnumMap.h`)

## Adding a new Material format

1. Add the new enum value to `WarframeExporter::Material::MaterialType` (inside `material/MaterialExtractor.h`)
1. Add the enum value to `WarframeExporter::Material::MaterialExtractor::getEnumMapKeys()` (inside `material/MaterialExtractor.h`)

## Adding a new Level format

1. Add the new enum value to `WarframeExporter::Level::LevelType` (inside `level/LevelReader.h`)
1. Add the enum value to `WarframeExporter::Level::LevelExtractor::getEnumMapKeys()` (inside `level/LevelExtractor.h`)
1. Add a new reader class in the directory `level/types`. You should copy an existing class - the new format will likely be very similar to a previous one.
1. Register the class to `WarframeExporter::Level::g_enumMapLevel` (inside `level/LevelEnumMap.h`)