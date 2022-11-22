# Glew 2.2.0 Library

![Glew Logo](http://glew.sourceforge.net/glew.png)

This package contains the headers and the library files needed to run glew loader in your project, just simply add this package and it automatically will load the header **glew.h**, the **glew32.lib** and the **glew32.dll** files.

- If you want to run a static environment just go to:

  ​	Project > Project properties >  Referenced Packages > glew > Linkage and select **static**
  
- If there is conflicts with **NOT** including **glew.h** just use the #include statement where you need it, this doesn't conflicts with this package.

## Licensing
The content of this package is property of [nigels-com](https://github.com/nigels-com) and the only original code made by me it's the NuGet packaging. 
You can find the original source at: [glew](https://github.com/nigels-com/glew)