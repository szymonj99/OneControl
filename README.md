<img src="./images/Logo.png" alt="OneControl Logo">

# OneControl
Easily control all your additional devices seamlessly with one keyboard and mouse.

# How-To
_To use `vcpkg`, you need an environment variable `VCPKG_ROOT` set to the root directory of `vcpkg`._
```zsh
git clone https://github.com/szymonj99/OneControl
cd OneControl
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
The executable should be output to `build/Release/OneControl.exe`.

# IDE Support
OneControl supports CLion and Visual Studio.
To use CLion, you may need to create the following toolchains: `MSVC - x64` and `MSVC - x86`.
In the future, I would like these to be included in the repository.

## Additional build options
OneControl provides multiple CMake `option`s. These are prefixed with `ONECONTROL_` and can be changed in `CMakeLists.txt`.

For example, wanting to use `CPM` instead of `vcpkg`, change or set `ONECONTROL_USE_VCPKG` to `FALSE`.

For local development, you can also set `ONECONTROL_USE_LOCAL_ONELIBRARY` to `TRUE`. This can speed up rebuilding of the project and allows for local changes to OneLibrary to be implemented.

# License
[GPL-3.0 License](https://github.com/szymonj99/OneControl/blob/main/LICENSE)
Commercial licenses are also available.

# Project Structure
Currently, the project has the following structure:

```
./include   - The header files
./src	    - The source files
./tests     - The test files
```

If Git notifies you of changes to, for example, `.idea/vcs.xml`, you can do:
```
git update-index --assume-unchanged .idea/vcs.xml
```
This is so that you do not commit changes to that file which could include local directories.

# Code Style
- To Do