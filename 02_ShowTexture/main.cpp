#include <iostream>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


static const GLfloat lightpos[] = { 1.0, 1.0, 1.0, 1.0 }; /* 位置　　　 */
static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* 直接光強度 */
static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* 環境光強度 */


static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}


static void init(){
    
    int width, height;
    int channels;
    unsigned char *img = stbi_load((std::string(PATH_ROOT_DIR)+"/data/pics/scene1.jpg").c_str(), &width, &height, &channels, 3);
    if (img==NULL) {
        std::cout << "Failed to get image" << std::endl;
        stbi_failure_reason();
    }
    
    std::cout << width << ", " << height << ", " << channels << std::endl;
    std::cout << std::endl;
    
    /* テクスチャ画像はバイト単位に詰め込まれている */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      
    /* テクスチャの割り当て */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, img);
        
    /* テクスチャを拡大・縮小する方法の指定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      
    /* 初期設定 */
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glDisable(GL_CULL_FACE);
      
    /* 光源の初期設定 */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightpos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
}


static void scene(void)
{
  static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };  /* 材質 */
  
  /* 材質の設定 */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  
  /* テクスチャマッピング開始 */
  glEnable(GL_TEXTURE_2D);
  
  /* １枚の４角形を描く */
  glNormal3d(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2d(0.0, 1.0);
  glVertex3d(-1.4, -1.0,  0.0);
  glTexCoord2d(1.0, 1.0);
  glVertex3d( 1.4, -1.0,  0.0);
  glTexCoord2d(1.0, 0.0);
  glVertex3d( 1.4,  1.0,  0.0);
  glTexCoord2d(0.0, 0.0);
  glVertex3d(-1.4,  1.0,  0.0);
  glEnd();
  
  /* テクスチャマッピング終了 */
  glDisable(GL_TEXTURE_2D);
}


int main(void)
{
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
    
  if (!glfwInit())
    exit(EXIT_FAILURE);
    
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
    
  init();
    
    
  while (!glfwWindowShouldClose(window))
  {
    float ratio; //~54 図形を描画
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height); //開いた窓の描画範囲(0,0からwidth,heightまで)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //GPUメモリを初期化
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION); //~45 座標変換(xyz座標とウィンドウ内位置の対応付の定義)
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    scene();
      
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
