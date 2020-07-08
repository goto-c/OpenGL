# OpenGL Demo

"OpenGL Demo" is a demonstration of showing object with OpenGL.
Here is the used model throughout this project. (See License)
![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/sample.gif)

## DEMO

03_ShowObj is the main project. This project shows the full image of obj file.
3rd stage demonstrated below is the latest version.
  
#### 1st stage ::: vertex
Firstly I showed the image without normal/texture data.  This image is mapped only eith the vertices information.  

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug.gif) 

#### 2nd stage ::: normal
Next step is the shading.  By using normal vector data,  object was added its shades.  

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug_shade.gif)

#### 3rd stage ::: texture
Finally, texture has been added to the object by using texture coordinates and the material information in "bug.mtl" file.

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug_full.gif)

### 00_HelloWorld 
Just says "Hello World"

### 01_openwin
<img src="01_openwin/thumbnail.gif" width=200px>

### 01_openwinNewGlad
<img src="01_openwinNewGlad/thumbnail.png" width=200px>

### 02_ShowTexture
<img src="02_ShowTexture/thumbnail.png" width=200px>

### 03_ShowObj
<img src="03_ShowObj/gif/bug_full.gif" width=200px>

### 04_ShowObjWithShader
<img src="04_ShowObjWithShader/gif/normal.gif" width=200px>

### 05_ScreenShot
<img src="05_ScreenShot/output/Screen_0.png" width=200px>


## Features

These projects use the OpenGL, [GLFW](https://github.com/glfw/glfw), GLAD, stb_image.h, and stb_image_write.h.

## Requirement

* GLFW(3.3.2)
* GLAD(3.3)
* Xcode(11.5)

## Installation

You can install GLFW in [this page](https://www.glfw.org), GLAD in [this page](https://glad.dav1d.de/).

## Usage

Clone OpenGL repository by  
`git clone https://github.com/goto-c/OpenGL.git`  
  
Make build directory in OpenGL.  
`cd OpenGL`  
`mkdir build`  
`cd build`    
  
Use cmake  
`cmake -G Xcode ..`  

You will find Xcode project file in OpenGL/build directory.
Open the project file, then you can try every project.

## Note

I tested environments under Mac, not under Linux and Windows.

## License

https://poly.google.com/view/3WS86lkqYFy

  
  
## references

https://tokoik.github.io/GLFWdraft.pdf  
https://tokoik.github.io/opengl/libglut.html "GLUTによる「手抜き」OpenGL入門"  
