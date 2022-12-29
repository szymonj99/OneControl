<h1 align="center">
<img src="./images/Logo.png">
<br/>

# OneControl
Easily control all your additional devices seamlessly with one keyboard and mouse.

# How-To

```zsh
git clone https://github.com/szymonj99/OneControl
cd OneControl
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

To change the build type, replace `Release` with one of the available CMake options available [here](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html).

Then run `./src/OneControl` as an administrator.

# Linux Permissions
On Linux, the user has the choice to either:
- Run the program as root,
- Add the current user to the group which owns `/dev/input/mice` and `/dev/input/eventX` files.

This is needed as the files by default, prevent the user accounts from sniffing and simulating inputs, which is what this program does.

# License
[GPL-3.0 License](https://github.com/szymonj99/OneControl/blob/main/LICENSE)

# Project Structure
Currently, the project has the following structure:

```
./src	- The source files for the OneControl binary
./lib	- The source files for the OneControl library
./tests - The test files
```

- The files generated from the `./lib` directory are tested with the Google Tests framework. This library is then included in the binary. This allows us to test some of the functionality of the program.