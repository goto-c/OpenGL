# OpenGL Demo
<img src="sample.gif" width=850px>

"OpenGL Demo" is a demonstration of showing objects with OpenGL.  
This projects uses the model downloaded [here](https://poly.google.com/view/3WS86lkqYFy).  

<a href="http://doge.mit-license.org"><img src="http://img.shields.io/:license-mit-blue.svg"></a>  

[![C/C++ CI](https://github.com/goto-c/OpenGL/workflows/C/C++%20CI/badge.svg)](https://github.com/goto-c/OpenGL/actions)

## Main project 【03_ShowObj】

The main demo is 03_ShowObj. This project shows the full image of object wrriten in OBJ files.
  
![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug.gif)
#### ■1st : Vertex  
Firstly the image without normal/texture data is shown.  This image is mapped only with vertices' information.  

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug_shade.gif)
#### ■2nd : Normal  
The next step is shading.  By using normal vectors' data,  the object's shades is added.  

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug_full.gif)
#### ■3rd : Texture  
Finally, the object is covered with texture, using texture coordinates and the material information in MTL file.
  
## Quick Overview
  
### 00_HelloWorld 
This project just prints "Hello World"

### 01_openwin
<img src="01_openwin/thumbnail.gif" width=220px>

### 01_openwinNewGlad
<img src="01_openwinNewGlad/thumbnail.png" width=200px>

### 02_ShowTexture
<img src="02_ShowTexture/thumbnail.png" width=200px>

### 03_ShowObj
<img src="03_ShowObj/gif/bug_full.gif" width=220px>

### 04_ShowObjWithShader
<img src="04_ShowObjWithShader/gif/normal.gif" width=220px>

### 05_ScreenShot
<img src="05_ScreenShot/output/Screen_0.png" width=200px>


## Features

These projects use the OpenGL, [GLFW](https://github.com/glfw/glfw), [GLAD](https://github.com/Dav1dde/glad), [stb_image](https://github.com/nothings/stb).

## Requirement

* GLFW(Version 3.3.2)
* GLAD(Version 3.3)
* Xcode(Version 11.5)

## Installation

You can also install GLFW [here](https://www.glfw.org), GLAD [here](https://glad.dav1d.de/), but it will be easier to use `git submodule update --init --recursive` to use these libraries.

## Usage

For Mac  
  
Clone OpenGL repository and update submodules with  
```
git clone https://github.com/goto-c/OpenGL.git
cd OpenGL 
git submodule update --init --recursive
```
  
Compile glfw with  
```
cd 3rd_party/glfw
cmake .  
make
```
  
Make build directory in OpenGL.  
```
cd ../../  
mkdir build  
cd build
``` 
 
Use cmake  
`cmake -G Xcode ..`  

You will find Xcode project file in OpenGL/build directory.
Open the project file, then you can try every project.

## Note

I tested environments under Mac, not under Linux and Windows.  
If you are a windows or Linux user, following scripts will help you.  
For [Windows](https://github.com/goto-c/OpenGL/blob/master/.github/workflows/c-cpp-windows.yml) / For [Linux](https://github.com/goto-c/OpenGL/blob/master/.github/workflows/c-cpp-linux.yml)  

## License

https://poly.google.com/view/3WS86lkqYFy

  
  
## references

https://tokoik.github.io/GLFWdraft.pdf  
https://tokoik.github.io/opengl/libglut.html "GLUTによる「手抜き」OpenGL入門"  
