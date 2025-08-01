# Warframe Extractor

A cross-platform program/library for exporting files from Warframe. Provides a Command-line and Graphical interface. "Easily" extensible to support new formats and upgrades to existing formats.

<hr>

![Preview](/assets/Preview.png)

# How to use

Download the [latest release](https://github.com/Puxtril/Warframe-Exporter/releases/latest).
- `Warframe-Exporter` is the easy-to-use program with a Graphical User Interface.
- `Warframe-Exporter-CLI` is a Command-Line interface. This is useful for writing scripts to export files.

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
    1. Using the CLI, find specific formats using the `--print-enums` flag
    1. Write raw files using the `--write-raw` flag
1. Within ImHex, go to *File -> Import -> Pattern File* and select the appropriate pattern

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
    
Rather than duplicate build commands here, please refer to the runners inside `.github/workflows`.

# Overview of this library

For more information on the cache file structure, read the [LotusLib documentation](https://github.com/Puxtril/LotusLib#documentation).

## High-level overview

Inside the Cache files Warframe stores on your computer (.toc and .cache), lie all the game's assets. The assets are not stored in standard formats like PNG images or FBX 3D models, they're stored in custom formats. This program reads these custom assets from the cache and converts them into sandard formats. Since it's only reading files from the cache, there is 0 risk of being banned from Warframe. Unless you run this while Warframe is running, then Warframe may detect something is reading the cache files.

## Low-level overview

Every virtual file inside the cache begins with the Common Header structure. Inside the Common Header is an integer enumeration indicating the file type. This file type indicates how the remaining data is formatted. This Extractor maintains a mapping of supported file types to an extractor (All model enumerations are processed by the model extractor, texture by the texture extractor, etc).

I've implemented my own flow to extract assets. The goal was to ensure new formats can easily be added. Each folder (ex. Model, Texture, Material, etc) implements this basic structure:

1) **Read** the virtual file into an *external* data structure. The *external* structure should resemble the source file structure as closely as possible.
1) **Convert** the *external* structure into an *internal* structure. This stage removes any game-logic, engine transformations, or anything else that may interfere with basic assumptions about the structure. The *internal* structure should be compatable for any possible use case.
1) **Export** the *internal* structure into a standard format. These often require the use of external libraries to accomodate standard file formats (ex. glTF, PNG). 

You may also notice an *enum map* structure floating about. The goal here is to *link integer enumerations with C++ classes which process them.* The file type enumeration (mentioned previously) is slightly more complex, in that the enumeration is specific to the game (Warframe/Soulframe) and the Package Category (Misc, Texture, AnimRetarget, etc). Extractors have their own Enum Map structure to handle game/package mappings, however there is also a basic Enum Map structure which simply links an integer to a processor class. This is used by other enumerations like texture compression and audio compression.
