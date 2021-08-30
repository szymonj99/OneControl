<h1 align="center">
<img src="./images/Logo.png">
<br/>

# OneControl
Easily control all your additional devices seamlessly with one keyboard and mouse.

# How-To
```
git clone https://github.com/szymonj99/OneControl
cd OneControl
cmake .
cmake --build .
```
Then run `./src/OneControl` as an administrator.

# Linux Permissions
On Linux, the user has the choice to either:
- Run the program as root,
- Add the current user to the group which owns `/dev/input/mice` and `/dev/input/eventX` files.

This is needed as the files by default, prevent the user accounts from sniffing and simulating inputs, which is what this program does.

# License
[GPL-3.0 License](https://github.com/szymonj99/OneControl/blob/main/LICENSE)