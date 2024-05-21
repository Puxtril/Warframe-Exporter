# Warframe Extractor

A cross-platform program/library for exporting files from Warframe. Provides a Command-line and Graphical interface. "Easily" extensible to support new formats and upgrades to existing formats.

<hr>

![Preview](/assets/Preview.png)

# Table of Contents

1. [How to use](#how-to-use)
1. [External Libraries](#external-libraries)
1. [ImHex Patterns](#imhex-patterns)
1. [Contributions](#contributions)
1. [Building](#building)
1. [Library Overview](#overview-of-this-library)
    1. [High level](#high-level-overview)
    1. [Medium level](#medium-level-overview)
    1. [Low level](#low-level-overview)

# How to use

Download the [latest release](https://github.com/Puxtril/Warframe-Exporter/releases/latest).
- `Warframe-Exporter` is the easy-to-use program with a Graphical User Interface.
- `Warframe-Exporter-CLI` is a Command-Line interface. This is useful for writing scripts to export files.
- `Warframe-Exporter-CLI-Advanced` is a Command-Line interface with extra features. You probably don't need this unless you're familiar with the inner workings of Warframe.

# External Libraries

* [spdlog](https://github.com/gabime/spdlog): Logging
* [Binary-Reader](https://github.com/Puxtril/Binary-Reader) Read binary data into primitive data types.
* [LotusLib](https://github.com/Puxtril/LotusLib): Provides an interface for the Warframe files. [See requirements for Oodle](https://github.com/Puxtril/LotusLib#how-to-install).
* [ddspp](https://github.com/puxtril/ddspp): For exporting textures as DDS
* [fx-gltf](https://github.com/jessey-git/fx-gltf): For exprting models as [glTF](https://en.wikipedia.org/wiki/GlTF).
  * [nlohmann-json](https://github.com/nlohmann/json): Required by fx-gltf.
* [glm](https://github.com/g-truc/glm): A helper library for models. Provides matrix and vector math operations.
* [TCLAP](https://sourceforge.net/projects/tclap/): Command-line interface library
* [Qt6](https://doc.qt.io/): GUI framework.
* [QtOpenGLViewer](https://github.com/Puxtril/QtOpenGLViewer): View 2D/3D scenes with mouse rotation/pan/zoom
* [bcdec](https://github.com/iOrange/bcdec): Decode [block-compressed](https://en.wikipedia.org/wiki/S3_Texture_Compression) DDS images.
* [libspng](https://github.com/randy408/libspng): Effieciently encode images as PNG.

# ImHex Patterns

Inside the `utils` folder, you'll find a collection of `.hexpat` files. These are patterns for [ImHex](https://github.com/WerWolv/ImHex). If you're interested in the file formats analyzed in this repo, I highly recommend using these.

1. Download and install ImHex
1. Copy all `.hexpat` files into `C:\Users\<Username>\AppData\Local\imhex\patterns`
1. Load a raw file into Imhex for analysis
    1. (Optional) Using the advanced tool, find specific formats using the `--print-enums` flag
    1. Write raw files from the advanced tool using the `--write-raw` flag
1. Within ImHex, go to *File -> Import -> Pattern File* and select the appropriate pattern

# Contributions

## Adding New Extractor

If you indend to add a new format such as Animation, Maps, Audio, etc.

1. Create a new branch (if it doesn't already exist).
1. Create a new folder in *include* and *src*. Store all subsequent files in here.
1. Create a new class that inherits from the Extractor class (in `Extractor.h`).
1. Register the class inside `EnumMapExtractor.h`.
1. Add documentation to the README
    1. Add instructions to "Contributions"
    1. Add the format to "Formats currently supported"
1. Add the CLI commands (Yes, I know it's a pain. At least it's documented!)
    1. Debug
        1. Add the new SwitchArg command in `CLI-Debug.h`
        1. Initilize that member variable in the constructor inside `CLI-Debug.cpp`
        1. Add that member variable to the CLI inside `addMainCmds`
        1. Add the member variable check to the `if` statement inside `processCmd` AND `debug`
        1. Add a package name to `getPkgsNames`
    1. Extract
        1. Add the new SwitchArg command in `CLI-Extract.h`
        1. Initilize that member variable in the constructor inside `CLI-Extract.cpp`
        1. Add that member variable to the CLI inside `addMainCmds`
        1. Add the member variable check to the `if` statement inside `addMiscCommands` AND `processCmd`
        1. Add a package name to `getPkgsNames`
1. Add the UI features
    1. Add UI Buttons
        1. Download and open Qt6 Designer
        1. Open Picker.ui
        1. Add Checkboxes to an appropriate location
        1. Update the checkbox names in the "Object Inspector" window on Designer
    1. Add checkbox value to `pickerDone` signal
        1. Open `src/ui/UIPicker.cpp`
        1. Inside the function `parsePickerOptions`, add `if` statements for both checkboxes
    1. Add load/save settings
        1. Inside `include/ui/Settings.h`, add `static const QString` values for view/export
        1. Inside `include/ui/Settings.h`, Add getter-methods for view/export
        1. Implement above methods inside `src/ui/Settings.cpp`
        1. Add save commands inside `src/ui/Settings.cpp` method `setSettings`
        1. Add calls to these functions inside `src/ui/UIPicker.cpp` method `loadSettings`
    1. Add new package loads
        1. If needed, add new package loading to `src/ui/UIExporter.cpp` method `getPackageNames`

## Adding a new 3D Model/Texture/Material/etc type

*Warframe will often add slightly altered file formats. You can tell by the new enumeration value inside the CommonHeader. New formats need manual inspection and addition to the extractor. You can see all the existing file foramts inside each respective asset type's folder named `types`.*

**\<type\> refers to the asset type. Ex: model, texture, etc.**

1. Add the new enum value inside `<type>Types.h`
1. Add the enum value to `getEnumMapKeys()` (inside `<type>Extractor.h`)
1. Add a new reader class in the directory `types`. You should copy an existing class - the new format will likely be very similar to a previous one.
1. Register the class to `g_enumMap<type>` (inside `<type>EnumMap.h`)

## Adding a new Texture Compression Format
This is very unlikely to occur. They recently started using BC6 and BC7, so there's no more compression formats they could use. Unless they add another uncompressed format.

1. Add the new enum value to `WarframeExporter::Texture::TextureCompression` (inside `texture/TextureInfo.h`)
1. Add a new class inside `texture/TextureInfos.h`.
1. Register the new class to `WarframeExporter::Texture::g_enumMapTexture` (inside `texture/TextureEnumMap.h`)

# Building

CMake is very nice and I love it. See how easy this is to compile?
**EXCEPT THE UI**

## Requirements

- Packages 
    - Windows: CMake, git, Visual Studio 2022
    - Other OSs: CMake, git, any C++ compiler
- A copy of the Oodle SDK (Provided by Unreal Engine)
    - Download the files.
        - Download from the engine
            - Download Unreal Engine from the official website (You will need an account and the Epic launcher)
            - Once downloaded find the SDK folder `Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.5/lib`
        - OR download [from here.](https://github.com/WorkingRobot/OodleUE/tree/main/Engine/Source/Runtime/OodleDataCompression/Sdks)
    - Create a folder in the root of this repository named `bin`
    - Copy folders `Linux` and `Win64` into `bin`.
    

## Build Commands

1. `git clone https://github.com/Puxtril/Warframe-Exporter.git && cd Warframe-Exporter`
1. Edit the first few lines of CMakeLists.txt to build what you want.
1. Initilize the submodules
	1. `cd lib`
	1. `git submodule update --init Binary-Reader ddspp fx-gltf glm json LotusLib spdlog tclap-code`
1. If you're on Windows and haven't installed the Qt6 SDK
    1. Initilize the repositories
        1. `git submodule update --init qt5`
        1. `cd qt5`
        1. `git submodule update --init qtbase qttools qtdeclarative`
        1. `cd qttools`
        1. `git submodule update --init`
        1. `cd ../../..`
    1. Build Qt
        1. `mkdir build-qt && cd build-qt`
        1. Run the rest of these commands from the MSVC shell (x64 Native Tools Command Prompt)
        1. `../lib/qt5/configure -release -optimize-size -prefix "../lib/qt-install" -confirm-license`
        1. `cmake --build . --parallel 4`
        1. `cmake --install .`
1. Install zlib
    1. Linux: Install using your package manager.
    1. Windows: *sigh* follow these steps.
        1. `cd` into an empty directory. We will build zlib here.
        1. `git clone https://github.com/madler/zlib.git`
        1. `mkdir zlib-build`
        1. `cd zlib-build`
        1. `cmake ..\zlib -DCMAKE_INSTALL_PREFIX=..\zlib-install`
        1. `cmake --build . --config Release`
        1. `cmake --install .`
1. Build Warframe-Exporter
	1. `mkdir build && cd build`
	1. Linux: `cmake ..` Windows: `cmake .. -DZLIB_ROOT=<path-to>/zlib-build`
	1. Linux: `make` Windows: `cmake --build . --config Release`
1. Copy dependencies (Windows)
	1. `../lib/qt-install/bin/windeployqt Warframe-Exporter.exe`

# Overview of this library

I will split this up into 3 sections, each subsequent section being more verbose. For more information on the cache file structure, read the [LotusLib documentation](https://github.com/Puxtril/LotusLib#documentation).

## High-level overview

This project converts the data from the Warframe cache files into usable formats. A command-line interface is provided (break out your CMD skills), though programmers may compile this as a library for use in other projects. It also supports debugging because let's be real, there will be new versions of exsting formats added, and that needs to be debuggable.

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