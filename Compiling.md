# Linux

1. Install Dependencies
    * Archlinux: `pacman -s git make cmake gcc zlib glu qt6-base`
    * Debian/Ubuntu/Mint: `apt install git make cmake g++ zlib1g-dev libglu1-mesa-dev qt6-base-dev`
    * Fedora: `yum install git make cmake gcc zlib-devel mesa-libGLU-devel qt6-qtbase-devel`
1. [Initilize the repository](#initilize-the-repository)
1. [Install oodle](#obtain-a-copy-of-oodle)
1. Compile
    1. Create a folder to compile (can be anywhere), then `cd` into it.
    1. `cmake <path_to_repo> -DCMAKE_BUILD_TYPE=Release`
    1. `cmake --build . -j8`
        - The `-j8` flag adds multi-threading

# Windows

1. Install vcpkg and Packages
    1. Open terminal in a directory to clone/install vcpkg. Binaries will be installed here.
    1. `git clone https://github.com/microsoft/vcpkg.git` and `cd vcpkg`
    1. Initilize vcpkg with `.\bootstrap-vcpkg.bat`
    1. Install `zlib` with `.\vcpkg.exe install zlib:x64-windows-static`
    1. Install Qt with `.\vcpkg.exe install qtbase qtmultimedia` (This takes a **while**)
1. [Initilize the repository](#initilize-the-repository)
1. [Install oodle](#obtain-a-copy-of-oodle)
1. Compile
    1. Create a folder to compile (can be anywhere), then `cd` into it.
    1. `cmake <path_to_repo> -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=<vcpkg_install_dir>\scripts\buildsystems\vcpkg.cmake`
    1. `cmake --build . -j8`
        - The `-j8` flag adds multi-threading

# Generic steps (hyperlinked above)

## Initilize the repository
1. `git clone https://github.com/Puxtril/Warframe-Exporter && cd Warframe-Exporter`
1. Initilize the submodules
	1. `cd lib`
	1. `git submodule update --init --depth 1 Binary-Reader LotusLib QtOpenGLViewer bcdec ddspp fx-gltf glm json libspng spdlog PhysxParser`
    1. `git submodule update --init tclap-code`
    1. `cd LotusLib/lib`
    1. `git submodule update --init --depth 1`


## Obtain a copy of Oodle

### Option 1: Download from Github

1. Browse to `https://github.com/WorkingRobot/OodleUE/tree/main/Engine/Source/Runtime/OodleDataCompression/Sdks`
1. Click on the latest version
1. Download `lib/Linux/liboo2corelinux64.a` or `lib/Win64/oo2core_win64.lib`
    - For the lazy, [Linux](https://github.com/WorkingRobot/OodleUE/raw/refs/heads/main/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.12/lib/Linux/liboo2corelinux64.a) and [Windows](https://github.com/WorkingRobot/OodleUE/raw/refs/heads/main/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.12/lib/Win64/oo2core_win64.lib).
1. If you wish to compile this project in Debug mode, download the debug binaries `lib/Linux/liboo2corelinux64_dbg.a` or `lib/Win64/oo2core_win64_debug.lib`
    1. For the lazy, [Linux](https://github.com/WorkingRobot/OodleUE/raw/refs/heads/main/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.12/lib/Linux/liboo2corelinux64_dbg.a) and and [Windows](https://github.com/WorkingRobot/OodleUE/raw/refs/heads/main/Engine/Source/Runtime/OodleDataCompression/Sdks/2.9.12/lib/Win64/oo2core_win64_debug.lib)

### Option 2: Obtain directly from Unreal Engine

1. Download and install [Epic Launcher](https://store.epicgames.com)
1. Once downloaded find the SDK folder `Engine/Source/Runtime/OodleDataCompression/Sdks/X.X.X/lib`

### Move Oodle into the repository

1. In the root of this repository, create a folder `bin`
1. Inside `bin`, create a folder `Linux` or `Win64`
1. Put the above binary inside.

Your folder structure should look like this:

```
├──assets
├──bin
|  ├──Linux
|  |  └──liboo2corelinux64.a
|  └──Win64
|     └──oo2core_win64.lib
├──include
├──...
```