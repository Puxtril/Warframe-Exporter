# Warframe Extractor

A cross-platform program/library for exporting files from Warframe. Provides a Command-line and Graphical interface. "Easily" extensible to support new formats and upgrades to existing formats.

<hr>

![Preview](/assets/Preview.png)

# How to use

Download the [latest release](https://github.com/Puxtril/Warframe-Exporter/releases/latest).
- `Warframe-Exporter` is the easy-to-use program with a Graphical User Interface.
- `Warframe-Exporter-CLI` is a Command-Line interface. This is useful for writing scripts to export files.
- `Warframe-Exporter-CLI-Advanced` is a Command-Line interface with extra features. You probably don't need this unless you're familiar with the inner workings of Warframe.

# External Libraries

* [LotusLib](https://github.com/Puxtril/LotusLib): Provides an interface for the Warframe files. [See requirements for Oodle](https://github.com/Puxtril/LotusLib#how-to-install).
* [Binary-Reader](https://github.com/Puxtril/Binary-Reader) Read binary data into primitive data types.
* [Qt6](https://doc.qt.io/): GUI framework.
* [TCLAP](https://sourceforge.net/projects/tclap/): Command-line interface library
* [spdlog](https://github.com/gabime/spdlog): Logging
* [ddspp](https://github.com/puxtril/ddspp): For exporting textures as DDS
* [fx-gltf](https://github.com/jessey-git/fx-gltf): For exprting models as [glTF](https://en.wikipedia.org/wiki/GlTF).
  * [nlohmann-json](https://github.com/nlohmann/json): Required by fx-gltf.
* [glm](https://github.com/g-truc/glm): A helper library for models. Provides matrix and vector math operations.
* [QtOpenGLViewer](https://github.com/Puxtril/QtOpenGLViewer): View 2D/3D scenes with mouse rotation/pan/zoom
* [bcdec](https://github.com/iOrange/bcdec): Decode [block-compressed](https://en.wikipedia.org/wiki/S3_Texture_Compression) DDS images.
* [libspng](https://github.com/randy408/libspng): Effieciently encode images as PNG.

# ImHex Patterns

Inside the `utils` folder, you'll find a collection of `.hexpat` files. These are patterns for [ImHex](https://github.com/WerWolv/ImHex). If you're interested in the file formats analyzed in this repo, I highly recommend using these.

1. Download and install ImHex
1. Copy all `.hexpat` files into `C:\Users\<Username>\AppData\Local\imhex\patterns` or `~/.local/share/imhex/patterns`
1. Load a raw file into Imhex for analysis
    1. Using the advanced tool, find specific formats using the `--print-enums` flag
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

CMake is very nice and I love it. ~~See how easy this is to compile?~~ I hate GUIs and Windows.

## Requirements

- Packages 
    - Windows: CMake, git, Visual Studio 2022
    - Other OSs: CMake, git, any C++ compiler
- A copy of the Oodle SDK (Provided by Unreal Engine)
    - Download the files.
        - Easy: Download [from here.](https://github.com/WorkingRobot/OodleUE/tree/main/Engine/Source/Runtime/OodleDataCompression/Sdks)
        - Hard: Download from the engine
            - Download Unreal Engine from the official website (You will need an account and the Epic launcher)
            - Once downloaded find the SDK folder `Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.5/lib`
    - Create a folder in the root of this repository named `bin`
    - Copy folders `Linux` and `Win64` into `bin`. We want the *static* libraries here.
    

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

For more information on the cache file structure, read the [LotusLib documentation](https://github.com/Puxtril/LotusLib#documentation).

## High-level overview

Inside the Cache files Warframe stores on your computer (.toc and .cache), lie all the game's assets. The assets are not stored in standard formats like PNG images or FBX 3D models, they're stored in custom formats. This program reads these custom assets from the cache and converts them into sandard formats. Since it's only reading files from the cache, there is 0 risk of being banned from Warframe. Unless you run this while Warframe is running, then Warframe may detect something is reading the cache files.

## Low-level overview

Every virtual file inside the cache contains a Common Header structure. Inside this structure is an integer enumeration indicating the file type. This file type dictates the structure of data following the Common Header. This Extractor maintains a list of all the supported file type eumerations depending on which category it falls into (Ex. Model enumerations are inside the *model* folder, textures inside the *texture* folder, etc).

I've implemented my own flow to extract assets. The goal was to ensure new formats can easily be added (which has been proved true). Each folder (ex. Model, Texture, Material, etc) implements this basic structure:

1) **Read** the virtual file into an *external* data structure. The *external* structure should resemble the source file structure as closely as possible.
1) **Convert** the *external* structure into an *internal* structure. This stage removes any game-logic, engine transformations, or anything else that may interfere with basic assumptions about the structure. The *internal* structure should be compatable for any possible use case.
1) **Export** the *internal* structure into a standard format. These often require the use of external libraries to accomodate standard file formats (ex. glTF, PNG). 

You may also notice an *enum map* structure floating about. The goal here is to *link integer enumerations with C++ classes to process them.* To better explain, here are the steps to use this system:

1) Create a new abstract class that inherits from `EnumMapValue`. `EnumMapValue` has only 1 abstract method that returns an array of processable enumeration values. Example: `Extractor.h`
1) Create a new concrete class that implements this interface. Example: `model/ModelExtractor`
1) Create a *const static EnumMap* using the abstract class from step 1 and initilize all the concrete classes. Example: `EnumMapExtractor.h`
