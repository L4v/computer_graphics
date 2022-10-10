# Computer Graphics
Computer Graphics course material for the Faculty of Technical Sciences.

## Table of Contents
  - [Branches](#branches)
  - [Installation and setup](#installation-and-setup)
    - [Windows](#windows)
      - [GiM starter project](#gim-starter-project)
      - [Creating your own project](#creating-your-own-project)

## Branches
- base: Contains the base code which can be expanded upon for projects and learning
- week-x: Contains x week's material

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
- For GLAD go to [this](https://glad.dav1d.de/) page
- Select API gl: "Version 3.3"
- Change Compatibility to "Core" 
- Click on "Generate" at bottom of page and download the glad.zip
	- ![GLAD](/img/glad0.png)
	- ![GLAD](/img/glad1.png)

- Unzip glad.zip
- Place the "include" and "src" directories in your program's code directory (where the main.c or main.cpp are located)
- In the Solution Explorer right click on your project -> "Add" and select "Existing item..."
	- ![Project external item](/img/project0.png)
- Add glad.c from the "src" folder
- Right click on your project again and select "Properties"
	- ![Project properties](/img/project1.png)
- Under "Configuration Properties"->"C/C++" select the dropdown button for "Additional Include Directories" (if it's invisible click on the text field) and select "<Edit...>"
	- ![Project configuration](/img/config0.png)
- Click on the "New Line" button
	- ![Project configuration](/img/config1.png)
- Click on "..."
	- ![Project configuration](/img/config2.png)
- Select the "include" directory you pasted into your project for glad
