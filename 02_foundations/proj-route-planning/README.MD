You can find more about this project [here](https://github.com/udacity/CppND-Route-Planning-Project).

However, if you want to build this project on your local `Windows` machine, 
I'd highly recommend the following steps for installing the `io2d` library
instead of the steps mentioned on the project's homepage.

### Prerequisites
* [Visual Studio Community 2019](https://visualstudio.microsoft.com/) (older versions like 15/17 and professional should work, too)
* [Cmake](https://cmake.org/download)
* [Vcpkg](https://github.com/Microsoft/vcpkg)
* [Git](https://git-scm.com/downloads)
* [Ubuntu bash for Windows](https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10)

### Compile Project
* Install Visual Studio and CMake, then follow the `vcpkg` instructions on Github to install it system-wide
* Install io2d and dependencies by executing in the cmd (for 64-bit): 
```
vcpkg install io2d:x64-windows
```
* Clone the project repo (recursively)
* Go to the project repo, then open the cmd there and create a build folder and cd into it. Execute: 
```
cmake -DCMAKE_TOOLCHAIN_FILE=vcpkg_root/scripts/buildsystems/vcpkg.cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..
```
* This should generate a Visual Studio project in the build folder which you can open by double clicking on
the `CppND-Route-Planning-Project.sln` file.
* In Visual Studio, select your build config. You can delete all projects in the solution,
except for the `CppND-Route-Planning-Project` and `plugixml` project.
* Right click on the `CppND-Route-Planning-Project` project and go to Settings -> C/C++ -> Preprocessor and add: `_UNICODE;UNICODE;`
* Close settings, then first build the `plugixml` project and afterwards the `CppND-Route-Planning-Project` project.
* Under project settings for the CppND project -> Debugging, add the filepath of the OpenStreetMap: 
```
-f C:/Repos/CppND-Route-Planning-Project/map.osm
```
You can now start the project directly from Visual Studio.

### Use Unit Tests:
In order to submit your project, it needs to pass the unit tests from Google Test.
To execute them on Windows, we are using the Ubuntu Bash for Windows 10.
First, we need to install the dependency packages inside Ubuntu Bash:

* Refresh apt: `sudo apt update`
* Install GCC: `sudo apt install build-essential`
* Build latest version of CMake: 
```
sudo apt-get remove -y cmake
git clone https://github.com/Kitware/CMake.git 
cd CMake
./bootstrap
make
sudo make install
```
* Install Cairo: `sudo apt install libcairo2-dev`
* Install graphicsmagick: `sudo apt install libgraphicsmagick1-dev`
* Install libpng: `sudo apt install libpng-dev`
* Next, we need to compile and install io2d:
```
git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl
cd P0267_RefImpl
mkdir Debug 
cd Debug
cmake .. 
make
make install
```

Finally, we can go to our project repo and build and execute the unit tests there:
```
cd /Project_Repo/ 
mkdir build 
cd build
cmake -DTESTING="AStarSearch" .. 
make
cd ../build/
./test
```
When you have finished implementing the project, all tests should pass!

The information are from `knowledge`'s udacity forum.
