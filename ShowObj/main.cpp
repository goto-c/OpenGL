#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION

std::vector<double> v;
std::vector<double> vn;
std::vector<double> vt;
std::vector<unsigned int> f_v;
std::vector<unsigned int> f_t;
std::vector<unsigned int> f_n;





static void error_callback(int error, const char* description){
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}





std::vector<unsigned int> strip_slash(std::string s){
    
    std::vector<unsigned int> vtn_values;
    vtn_values.clear();
    
    std::vector<int> digits_v;
    std::vector<int> digits_t;
    std::vector<int> digits_n;
    digits_v.clear();
    digits_n.clear();
    digits_t.clear();
    int v_value = 0;
    int t_value = 0;
    int n_value = 0;
    int idx = 0;
    
    // "***/***/***"の状態の文字列から、v,t,nの各インデックス文字列に分離する
    while (s[idx]!='/'){
        digits_v.push_back(s[idx]);
        idx++;
    }
    idx++;
    while (s[idx]!='/'){
        digits_t.push_back(s[idx]);
        idx++;
    }
    idx++;
    while (idx < s.size()){
        digits_n.push_back(s[idx]);
        idx++;
    }
    
    // インデックスの文字列を10進数に変換
    for (int i=0; i<digits_v.size(); i++){
        v_value +=
            std::pow(10, i)
            * (digits_v[digits_v.size() - 1 - i] - '0');
    }
    for (int i=0; i<digits_t.size(); i++){
        t_value +=
            std::pow(10, i)
            * (digits_t[digits_t.size() - 1 - i] - '0');
    }
    for (int i=0; i<digits_n.size(); i++){
        n_value +=
            std::pow(10, i)
            * (digits_n[digits_n.size() - 1 - i] - '0');
    }
    
    vtn_values.push_back(v_value);
    vtn_values.push_back(t_value);
    vtn_values.push_back(n_value);
    
    /*
    for (int i=0; i<s.size(); i++){
        std::cout << s[i];
    }
    std::cout << std::endl;
    std::cout << "s.size(): " << s.size() << std::endl;
    std::cout << "v: " << v_value << std::endl;
    std::cout << "t: " << t_value << std::endl;
    std::cout << "n: " << n_value << std::endl;
    std::cout << std::endl;
     */
    
    return vtn_values;
}

void read_obj(std::string objfile_name){
    
    std::ifstream fin;
    std::string fname = objfile_name;
    fin.open(fname);
    if (fin == NULL) std::cout << "File Open Failed" << std::endl;
    
    v.clear();
    vn.clear();
    vt.clear();
    f_v.clear();
    f_t.clear();
    f_n.clear();
    
    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    
    while(!fin.eof()){
        fin.getline(buff, BUFF_SIZE);
        
        // 先頭が'v'の場合
        if (buff[0]=='v' && buff[1]==' '){
            char str[256];
            double x, y, z;
            sscanf(buff, "%s %lf %lf %lf", str, &x, &y, &z);
            v.push_back(x);
            v.push_back(y);
            v.push_back(z);
        }
        // 先頭が'vn'の場合
        if (buff[0]=='v' && buff[1]=='n'){
            char str[256];
            double x, y, z;
            sscanf(buff, "%s %lf %lf %lf", str, &x, &y, &z);
            vn.push_back(x);
            vn.push_back(y);
            vn.push_back(z);
        }
        // 先頭が'vt'の場合
        if (buff[0]=='v' && buff[1]=='t'){
            char str[256];
            double x, y, z;
            sscanf(buff, "%s %lf %lf %lf", str, &x, &y, &z);
            vt.push_back(x);
            vt.push_back(y);
            vt.push_back(z);
        }
        // 先頭が'f'の場合
        if (buff[0]=='f'){
            char str[256];
            char i0[256], i1[256], i2[256], i3[256]={'\0'};
            sscanf(buff, "%s %s %s %s %s", str, i0, i1, i2, i3);
            std::vector<unsigned int> vertex0, vertex1, vertex2;
            vertex0.clear();
            vertex1.clear();
            vertex2.clear();
            vertex0 = strip_slash(i0);
            vertex1 = strip_slash(i1);
            vertex2 = strip_slash(i2);
            f_v.push_back(vertex0[0]-1);
            f_v.push_back(vertex1[0]-1);
            f_v.push_back(vertex2[0]-1);
            f_t.push_back(vertex0[1]-1);
            f_t.push_back(vertex1[1]-1);
            f_t.push_back(vertex2[1]-1);
            f_n.push_back(vertex0[2]-1);
            f_n.push_back(vertex1[2]-1);
            f_n.push_back(vertex2[2]-1);
            // 四角形メッシュを加える場合、三角形に分割
            if (i3[0]!='\0'){
                std::vector<unsigned int> vertex3;
                vertex3.clear();
                vertex3 = strip_slash(i3);
                f_v.push_back(vertex0[0]-1);
                f_v.push_back(vertex2[0]-1);
                f_v.push_back(vertex3[0]-1);
                f_t.push_back(vertex0[1]-1);
                f_t.push_back(vertex2[1]-1);
                f_t.push_back(vertex3[1]-1);
                f_n.push_back(vertex0[2]-1);
                f_n.push_back(vertex2[2]-1);
                f_n.push_back(vertex3[2]-1);
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
    
  for (int i = 0; i < f_v.size(); i++) {
      
    double n_vec[3] = {
    vn[f_n[i] * 3 + 0],
    vn[f_n[i] * 3 + 1],
    vn[f_n[i] * 3 + 2]};
    glNormal3dv(n_vec);
      
    float x, y, z;
    x = v[f_v[i] * 3 + 0];
    y = v[f_v[i] * 3 + 1];
    z = v[f_v[i] * 3 + 2];
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
    
  /*for (int i=0; i<vt.size()/3; i++){
      std::cout << "vt: " << vt[3*i] << ", " << vt[3*i+1] << ", " << vt[3*i+2] << std::endl;
  }*/
    
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
      
    // テクスチャの設定
      /*
    #define TEXWIDTH  256
    #define TEXHEIGHT 256
    static const char texture1[] = "bug.jpg";
    GLubyte texture[TEXHEIGHT][TEXWIDTH][3];
    FILE *fp;
    
    // テクスチャ画像の読み込み
    if ((fp = fopen(texture1, "rb")) != NULL){
        fread(texture, sizeof(texture), 1, fp);
        fclose(fp);
    }
    else {
        std::cout << "Texture file not opened" << std::endl;
    }
      
    // メモリ上に画素情報がどのように格納されているかをGPUに伝える(1->画素境界のバイト数)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // テクスチャの割り当て
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXWIDTH, TEXHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
      
    // テクスチャを拡大・縮小する方法を指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      
    glEnable(GL_TEXTURE_2D);

    int TEXSIZE = 4;
    glTexImage2D(
        GL_TEXTURE_2D , 0 , 3 , TEXSIZE , TEXSIZE ,
        0 , GL_RGB , GL_UNSIGNED_BYTE , texture
    );
    glBindTexture(GL_TEXTURE_2D, 0);
      */
    // 光源の初期設定 (光源の位置, 直接光強度, 環境光強度)
    const GLfloat lightPos[] = {3, 0, -2, 1};
    const GLfloat lightCol[] = {1, 1, 0, 1};
    const GLfloat lightAmb[] = {0.1, 0.1, 0.1, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_DEPTH_TEST);
    display();
    // カラーバッファを入れ替える
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

