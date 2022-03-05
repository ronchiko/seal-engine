# Seal Game Engine

A game engine written in C.

# Requirements

* CMake version 3.21 or higher
* Vcpkg

# Build

```powershell
# Install dependecies
vcpkg install "@.vcpkg_deps.txt"

# Build complete engine
cmake -B bin -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>\scripts\buildsystems\vcpkg.cmake
cmake --build bin

# Clean build
cmake --build bin --target clean

# If you are on windows run
./win-install.ps1	# This will add the library & binary files to $PATH 

# Build individual component (Once the project was built at least once)
cd <component-name>
cmake -B bin -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>\scripts\buildsystems\vcpkg.cmake
cmake --build bin

# For tests for individual component
ctest --test-dir bin --output-on-failure
```