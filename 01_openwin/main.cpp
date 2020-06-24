#include <iostream>
#include <GLFW/glfw3.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  while (!glfwWindowShouldClose(window))
  {
    float ratio; //~54 図形を描画
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height); //開いた窓の描画範囲(0,0からwidth,heightまで)
    glClear(GL_COLOR_BUFFER_BIT); //GPUメモリを初期化
    glMatrixMode(GL_PROJECTION); //~45 座標変換(xyz座標とウィンドウ内位置の対応付の定義)
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES); //~54 図形を定義(三角形を書くというコマンド)
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f); //xyz座標をGPUに送る
    glColor3f(0.f, 1.f, 0.f); //次に続く頂点の色をRGBで指定(0~1)
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd(); //line 47の定義を終える
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
