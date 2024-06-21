# Build Instructions
Build instructions are designed for Windows with MSVC. They should be similar for other platforms (see current state section in the README) and compilers, but some adjustments may be needed.

## Setup
1. Windows only: [Install Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/). You may need to add `CMAKE_WINDOWS_KITS_10_DIR` to your path if it is not being found ([more info](https://cmake.org/cmake/help/latest/variable/CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION.html)).
1. [Install Qt6](https://www.qt.io/product/qt6) if you haven't already.
1. Clone the project with submodules. Either use ```git clone https://github.com/KaranveerB/Open-Smart-Shutdown.git --recurse-submodules``` or if you've already cloned the project, use ```git submodule update --init --recursive```
1. If your build system cannot automatically find Qt6, set `QT6_INSTALL_PATH` in `src/CmakeLists.txt` to the library directory (the one with the `bin` folder in it). The specific directory may depend on your compiler. If your build system can automatically find Qt6 in your path, skip this step.

## Building
```
cd Open-Smart-Shutdown # cd into the directory if you haven't already
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../src # CMAKE_BUILD_TYPE can be Debug, Release, RelWithDebInfo, or MinSizeRel
cmake --build .
```

## Run the program
The executable will be in the subdirectory corresponding to the `CMAKE_BUILD_TYPE` used. Ensure the `.dll` and `plugins` folder is kept with the executable for Windows releases.
