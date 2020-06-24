# OpenGL Demo

"OpenGL Demo" is a demonstration of showing object with OpenGL.
The goal of this project is to show full object image.  

Here's the goal.  

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/sample.gif)

## DEMO

The project was developed step by step.  
  
### 1st stage :::
Firstly I showed the image without normal/texture data.  This image is mapped only eith the vertices information.  

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug.gif) 

### 2nd stage :::
Next step is the shading.  By using normal vector data,  the shades of the object are added.  

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug_shade.gif)

### 3rd stage :::
Finally, texture has been added to the object by using texture coordinates and the material information in "bug.mtl" file.

![demo](https://github.com/goto-c/OpenGL/blob/master/03_ShowObj/gif/bug_full.gif)


## Features

This project only uses the OpenGL with fixed function pipeline, and [GLFW](https://github.com/glfw/glfw).

## Requirement

* GLFW(3.3.2)
* Xcode(11.5)

## Installation

Besides github, you can also install GLFW in [this page](https://www.glfw.org).

## Usage

Cloning the repository with  
`git clone https://github.com/goto-c/OpenGL.git`  
  
Make build directory in the ShowObj directory.  
`cd OpenGL`  
`cd ShowObj`  
`mkdir build`  
`cd build`  
  
Do cmake  
`cmake -G Xcode ..`

Then, open the Xcode project file named " ShowObj ".  

## Note

I tested environments under Mac, not under Linux and Windows.

## License

https://poly.google.com/view/3WS86lkqYFy

  
  
## references

https://tokoik.github.io/GLFWdraft.pdf  
https://tokoik.github.io/opengl/libglut.html "GLUTによる「手抜き」OpenGL入門"  
