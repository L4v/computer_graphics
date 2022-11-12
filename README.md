# Computer Graphics
Computer Graphics course material for the Faculty of Technical Sciences.

## Table of Contents
- [Computer Graphics](#computer-graphics)
	- [Table of Contents](#table-of-contents)
	- [Branches](#branches)
	- [Environment](#environment)
	- [Installation and setup](#installation-and-setup)
		- [Windows](#windows)
			- [Setting up Visual Studio](#setting-up-visual-studio)
			- [Base starter project](#base-starter-project)
			- [Creating your own project](#creating-your-own-project)
		- [Linux](#linux)
			- [Arch based](#arch-based)
			- [Debian based](#debian-based)
			- [Compiling and running](#compiling-and-running)

## Branches
- base: Contains the base code which can be expanded upon for projects and learning
- demo: Contains a demo project showing off what's possible with OpenGL
- week-x: Contains x week's material

## Environment
> Note: If you wish to use other libraries, consult with Jovan Ivošević at ivosevic.jovan@uns.ac.rs.
- GLFW
- GLEW
- GLM

## Installation and setup
### Windows
If you want to use a project with preinstalled packages, you can use the starter project with  all the required libraries already setup.
#### Setting up Visual Studio
Make sure to select "Desktop development with C++" in Visual Studio Installer
![Desktop development with C++](/img/desktop_development_cpp.png)
#### Base starter project
- From the "base" branch, download the starter project for your course
- Open it in Visual Studio and check your NuGet packages, you might have to restore them if you're getting build errors
	- ![NuGet](/img/nuget0.png)
	- ![NuGet Restore](/img/nuget_restore.png)
- After that you're set to go! No further setup required. If there is an issue, please notify us so we could address it
#### Creating your own project
- Create a new C++ console app in Visual Studio
- With your project's .cpp file open, click on "Project" in toolbar
- Select Manage "NuGet Packages..."
	- ![NuGet](/img/nuget0.png)
- Select the "Browse" tab and enter the package name in the search field
- For GLFW enter "glfw" and install the package by Marcus Geelnard and Camilia Berglund
	- ![NuGet](/img/nuget1.png)
- In the same manner install the following NuGet packages:
	- glew-2.2.0 by NerdTronik
	- glm by GLM contributors (versions 0.9.9.800 and higher)
### Linux
Packages might differ depending on your distro, if not listed, contact ivosevic.jovan@uns.ac.rs to update.
- Create a folder for your code (you can use the .cpp and .hpp files from the repo)
#### Arch based
> Manjaro, EndeavourOS, etc...

- To install GLM, GLFW and GLEW:
```bash
sudo pacman -S glm glfw glew
```
#### Debian based
> Ubuntu, Pop, Deepin, Kali

```bash
sudo apt install libglfw3 libglfw3-dev libglm-dev libglew-dev
```

#### Compiling and running
You can use make, cmake or any other build tool, but for most purposes this command will suffice:
```bash
# To compile (run this command inside your code dir, where all the .cpp and .hpp files are). NOTE: pkg-config is surrounded by backticks `, not by quotes '
g++ -o <your_program_name> *.cpp `pkg-config --libs glfw3 glew` -ldl
# To run
./<your_program_name>
```
You can paste this code into a .sh file, e.g.: build.sh and run `chmod +x build.sh` only once in order to make it runnable, then to run it just type: `./build.sh` 
