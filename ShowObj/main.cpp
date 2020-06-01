#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

std::vector<double> v;
std::vector<unsigned int> f;




static void error_callback(int error, const char* description){
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}





int strip_slash(std::string s){
    
    std::vector<int> digits;
    digits.clear();
    int v_num = 0;
    
    for (int i=0; i<s.size(); i++){
        if (s[i] != '/'){
            digits.push_back(s[i]);
        }
        else break;
    }
    for (int i=0; i<digits.size(); i++){
        v_num +=
            std::pow(10, i)
            * (digits[digits.size() - 1 - i] - '0');
    }
    return v_num;
}

void read_obj(std::string objfile_name){
    
    std::ifstream fin;
    std::string fname = objfile_name;
    fin.open(fname);
    if (fin == NULL) std::cout << "File Open Failed" << std::endl;
    
    v.clear();
    f.clear();
    
    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    
    while(!fin.eof()){
        fin.getline(buff, BUFF_SIZE);
        
        if (buff[0]=='v' && buff[1]==' '){
            char str[256];
            double x, y, z;
            sscanf(buff, "%s %lf %lf %lf", str, &x, &y, &z);
            v.push_back(x);
            v.push_back(y);
            v.push_back(z);
        }
        if (buff[0]=='f'){
            char str[256];
            char i0[256], i1[256], i2[256], i3[256]={'\0'};
            sscanf(buff, "%s %s %s %s %s", str, i0, i1, i2, i3);
            int v0, v1, v2, v3;
            v0 = strip_slash(i0);
            v1 = strip_slash(i1);
            v2 = strip_slash(i2);
            f.push_back(v0-1);
            f.push_back(v1-1);
            f.push_back(v2-1);
            if (i3[0]!='\0'){
                v3 = strip_slash(i3);
                f.push_back(v0-1);
                f.push_back(v2-1);
                f.push_back(v3-1);
            }
        }
    }
    
    fin.close();
    
}



void resize(int w, int h){
    std::cout << w << ":" << h << std::endl;
    glViewport(0, 0, w, h);
}


// 配列に格納した図形データを描画する
void display(void){
    
  glColor3f(0.5f, 0.5f, 0.0f);
  glBegin(GL_TRIANGLES);
    
  for (int i = 0; i < f.size(); i++) {
    float x, y, z;
    x = v[f[i] * 3 + 0];
    y = v[f[i] * 3 + 1];
    z = v[f[i] * 3 + 2];
    glVertex3f(x, y, z);
  }
  glEnd();
  glFlush();
}








int main(void)
{
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
    
    
  // 使用するobjファイルを指定し、データを読み込む
  std::string objfile_name;
  std::cout << "Put Object filename" << std::endl;
  std::cout << " >>> ";
  std::cin >> objfile_name;
  read_obj(objfile_name);
    
  // GLFWを初期化し、ウィンドウを作成
  if (!glfwInit())
    exit(EXIT_FAILURE);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    
  // ウィンドウ作成に失敗 -> 終了
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
    
  // 作成したウィンドウをOpenGLの処理対象にする
  glfwMakeContextCurrent(window);
    
  glfwSetKeyCallback(window, key_callback);
    
  // ウィンドウが表示されている間、繰り返す
  while (!glfwWindowShouldClose(window))
  {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
      
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
    glTranslatef(0.0f, -0.5f, 0.0f);
      
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    display();
    // カラーバッファを入れ替える
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

