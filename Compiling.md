This guide is only for compiling on Linux. A pre-compiled version is already available for Windows, and the file inside `.github/workflows` should provide enough instructions for Windows compilation.

# Install Dependencies

* Archlinux: `pacman -s git make cmake g++ zlib glu qt6-base`
* Debian/Ubuntu/Mint: `apt install git make cmake g++ zlib1g-dev libglu1-mesa-dev qt6-base-dev`
* Fedora: `yum install git make cmake g++ zlib-devel mesa-libGLU-devel qt6-qtbase-devel`

# Initilize the repository

1. `git clone https://github.com/Puxtril/Warframe-Exporter && cd Warframe-Exporter`
1. Initilize the submodules
	1. `cd lib`
	1. `git submodule update --init --depth 1 Binary-Reader LotusLib QtOpenGLViewer bcdec ddspp fx-gltf glm json libspng spdlog PhysxParser`
    1. `git submodule update --init tclap-code`
    1. `cd LotusLib/lib`
    1. `git submodule update --init --depth 1`


# Obtain a copy of Oodle

## Option 1: Download from Github

1. Browse to `https://github.com/WorkingRobot/OodleUE/tree/main/Engine/Source/Runtime/OodleDataCompression/Sdks`
1. Click on the latest version
1. Browse to `lib/Linux`
1. Download `liboo2corelinux64.a`

## Option 2: Obtain directly from Unreal Engine

1. Download and install [Epic Launcher](https://store.epicgames.com)
1. Once downloaded find the SDK folder `Engine/Source/Runtime/OodleDataCompression/Sdks/X.X.X/lib/Linux`

## Move Oodle into the repository

1. In the root of this repository, create a folder `bin`
1. Inside `bin`, create a folder `Linux`
1. Copy `liboo2corelinux64.a` inside

Your folder structure should look like this:

```
├──assets
├──bin
|  └──Linux
|     └──liboo2corelinux64.a
├──include
├──...
```

# Compile 

1. Create a folder to compile (can be anywhere), then `cd` into it.
1. `cmake <path_to_repo> -DCMAKE_BUILD_TYPE=Release`
1. `cmake --build . -j8`
    - The `-j8` flag adds multi-threading
