# Computer Graphics
Computer Graphics course material for the Faculty of Technical Sciences.

## Table of Contents
- [Computer Graphics](#computer-graphics)
	- [Table of Contents](#table-of-contents)
	- [Branches](#branches)
	- [Environment](#environment)
	- [Installation and setup](#installation-and-setup)
		- [Windows](#windows)
			- [GiM starter project](#gim-starter-project)
			- [Creating your own project](#creating-your-own-project)

## Branches
- base: Contains the base code which can be expanded upon for projects and learning
- week-x: Contains x week's material

## Environment
> Note: If you wish to use other libraries, consult with Jovan Ivošević at ivosevic.jovan@uns.ac.rs.
- GLFW
- GLEW
- GLM

## Installation and setup
### Windows
If you want to use a project with preinstalled packages, you can use the starter project with  all the required libraries already setup.
#### GiM starter project
- From [GiM](http://gim.ftn.uns.ac.rs) download the starter project for your course
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