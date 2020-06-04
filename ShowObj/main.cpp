#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string objfile_name;
std::vector<double> v; // 頂点の空間座標
std::vector<double> vn; // 頂点の法線座標
std::vector<double> vt; // 頂点のテクスチャ座標
std::vector<unsigned int> f_v; // 面の頂点座標
std::vector<unsigned int> f_t; // 面のテクスチャ座標
std::vector<unsigned int> f_n; // 面の法線座標

// mtlファイルのマテリアル情報を格納した構造体
struct material_info {
    std::string name;
    double Ka[3];
    double Kd[3];
    double Ks[3];
    int d;
    std::string map_Ka;
    std::string map_Kd;
    std::string map_d;
    int f_start;
};

// マテリアル構造体を出現順に格納したベクトル
material_info materials[1024];



/* --------------- コールバック関数 ------------------*/

static void error_callback(int error, const char* description){
  fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize(int w, int h){
    std::cout << w << ":" << h << std::endl;
    glViewport(0, 0, w, h);
}


/* --------------- obj ファイル読み込み用の関数 --------------*/

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
    
    return vtn_values;
}

// objファイルを読み込み、データをvectorに格納する
void read_obj(std::string file_obj){
    
    std::ifstream fin;
    std::string fname = file_obj;
    fin.open(fname);
    if (fin == NULL) std::cout << "OBJ file open failed" << std::endl;
    //else             std::cout << "OBJ file opened !" << std::endl;
    
    v.clear();
    vn.clear();
    vt.clear();
    f_v.clear();
    f_t.clear();
    f_n.clear();
    
    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    int f_count = 0;
    int mtl_count = 0;
    
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
        // 先頭が'us'の場合
        if (buff[0]=='u' && buff[1]=='s'){
            char str[256], mtl[1024];
            sscanf(buff, "%s %s", str, mtl);
            materials[mtl_count].name = mtl;
            materials[mtl_count].f_start = mtl_count;
            mtl_count++;
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
                f_count++;
            }
            f_count++;
        }
    }
    fin.close();
}


/* ----------------- mtl ファイル読み込み用の関数 ---------------*/

void read_mtl(std::string file_mtl){
    
    std::ifstream fin;
    std::string fname = file_mtl;
    fin.open(fname);
    if (fin == NULL) std::cout << "MTL file open failed" << std::endl;
    //else             std::cout << "MTL file opened !" << std::endl;
    
    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    std::vector<unsigned int> m_idx;
    
    while (!fin.eof()){
        fin.getline(buff, BUFF_SIZE);
        
        // コメント行は飛ばす
        if (buff[0]=='#') continue;
        
        // マテリアル名が一致する構造体配列の要素を検索
        if (buff[0]=='n' && buff[1]=='e' && buff[7]=='{'){
            char str[256], name[1024];
            sscanf(buff, "%s %s", str, name);
            std::cout << "name: " << name << std::endl;
            m_idx.clear();
            int idx = 0;
            
            while (materials[idx].name[0] == '{'){
                // マテリアル名が一致すれば、そのインデックスをm_idxに記憶
                if (materials[idx].name == name){
                    m_idx.push_back(idx);
                }
                idx++;
            }
        }
        // 対応するマテリアルの構造体に、各種の情報を追加していく
        for (int k=0; k<m_idx.size(); k++){
            
                // 環境光・拡散光・鏡面光のRGB値を構造体のメンバに追加
                if (buff[1]=='K' && buff[2]=='a'){
                    char str[256];
                    double r, g, b;
                    sscanf(buff, "%s %lf %lf %lf", str, &r, &g, &b);
                    materials[m_idx[k]].Ka[0] = r;
                    materials[m_idx[k]].Ka[1] = g;
                    materials[m_idx[k]].Ka[2] = b;
                }
                if (buff[1]=='K' && buff[2]=='d'){
                    char str[256];
                    double r, g, b;
                    sscanf(buff, "%s %lf %lf %lf", str, &r, &g, &b);
                    materials[m_idx[k]].Kd[0] = r;
                    materials[m_idx[k]].Kd[1] = g;
                    materials[m_idx[k]].Kd[2] = b;
                }
                if (buff[1]=='K' && buff[2]=='s'){
                    char str[256];
                    double r, g, b;
                    sscanf(buff, "%s %lf %lf %lf", str, &r, &g, &b);
                    materials[m_idx[k]].Ks[0] = r;
                    materials[m_idx[k]].Ks[1] = g;
                    materials[m_idx[k]].Ks[2] = b;
                }
                // テクスチャ画像ファイル名を構造体メンバに追加
                if (buff[1]=='m' && buff[5]=='K' && buff[6]=='a'){
                    char str[256], map[1024];
                    sscanf(buff, "%s %s", str, map);
                    materials[m_idx[k]].map_Ka = map;
                }
                if (buff[1]=='m' && buff[5]=='K' && buff[6]=='d'){
                    char str[256], map[1024];
                    sscanf(buff, "%s %s", str, map);
                    materials[m_idx[k]].map_Kd = map;
                }
                if (buff[1]=='m' && buff[5]=='d'){
                    char str[256], map[1024];
                    sscanf(buff, "%s %s", str, map);
                    materials[m_idx[k]].map_d = map;
                }
            }
        }
        
}

/* ------------------ 初期化関数 ----------------------*/

static void InitText(const char *file_path){
    
    int width, height;
    int channels;
    unsigned char *img = stbi_load(file_path, &width, &height, &channels, 3);
    
    if (img == NULL) std::cout << "Failed to get image" << std::endl;
    else {
        std::cout << "texture -> " << file_path << std::endl;
        std::cout << "           " << width << " , " << height << std::endl;
    }
    
    /* テクスチャ画像はバイト単位に詰め込まれている */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      
    /* テクスチャの割り当て */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, img);
        
    /* テクスチャを拡大・縮小する方法の指定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      
    /* 初期設定 */
    glClearColor(0.7, 0.5, 0.3, 0.7); // 背景の色を指定
    glDisable(GL_CULL_FACE); // ポリゴンの表面のみ表示
}


static void InitLight(GLfloat *lightamb){
    
    GLfloat lightpos[] = { 1.0, 1.0, 1.0, 0.5 }; /* 位置　　　 */
    GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* 直接光強度 */
      
    /* 光源の初期設定 */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightpos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
}


/* ------------------ ディスプレイ関数 -----------------*/

void display(void){
    
  struct material_info *pt = &materials[0];
    
  for (int i = 0; i < f_v.size(); i++) {
    
    // 異なるマテリアルの描画を始める場合、テクスチャと光源を初期化
    if (i == pt -> f_start){
        
        // テクスチャ画像が変更されている場合
        if (pt -> map_Kd != (pt-1) -> map_Kd && pt -> map_Kd[0] != '\0'){
            std::string tex_file = pt -> map_Kd;
            std::string tex_file_path = "./data/" + objfile_name + "/" + tex_file;
            InitText(tex_file_path.c_str());
            //std::cout << " texture -> " << tex_file << std::endl;
        }
        
        // 光源を設定
        GLfloat lightamb[3] = {pt -> Ka[0],
                               pt -> Ka[1],
                               pt -> Ka[2]};
        InitLight(lightamb);
        
        pt++;
    }
      
    static const GLfloat color[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glEnable(GL_TEXTURE_2D);
      
    glColor3f(1.0f, 1.0f, 0.5f);
    glBegin(GL_TRIANGLES);
    
    // f_vtnを抜き出す
    double t1, t2;
    t1 =     vt[f_t[i] * 3 + 0];
    t2 = 1 - vt[f_t[i] * 3 + 1];
      
    double n_vec[3] = {
    vn[f_n[i] * 3 + 0],
    vn[f_n[i] * 3 + 1],
    vn[f_n[i] * 3 + 2]};
      
    double x, y, z;
    x = v[f_v[i] * 3 + 0];
    y = v[f_v[i] * 3 + 1];
    z = v[f_v[i] * 3 + 2];
      
    glTexCoord2d(t1, t2);
    glNormal3dv(n_vec);
    glVertex3f(x, y, z);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glFlush();
}


/* ------------------ main 関数 --------------------*/

int main(void)
{
  // 使用するobjファイルを指定し、データを読み込む
  std::cout << "Put Object filename without the extension" << std::endl;
  std::cout << " >>> ";
  std::cin >> objfile_name;
    
  std::string file_obj = "./data/" + objfile_name + "/" + objfile_name + ".obj";
  std::string file_mtl = "./data/" + objfile_name + "/" + objfile_name + ".mtl";
    
  read_obj(file_obj);
  std::cout << "OBJ file loading succeeded !" << std::endl;
  read_mtl(file_mtl);
  std::cout << "MTL file loading succeeded !" << std::endl;
    
  /*
  for (int i=0; i<3; i++){
      std::cout << materials[i].f_start << ": " << materials[i].map_Kd << std::endl;
  }
    
  for (int i=0; i<3; i++){
      std::cout << materials[i].name << ", " << std::endl;
      std::cout << i << "Ka: " << materials[i].Ka[0] << ", " << materials[i].Ka[1] << ", " << materials[i].Ka[2] << std::endl;
      std::cout << i << "Kd: " << materials[i].Kd[0] << ", " << materials[i].Kd[1] << ", " << materials[i].Kd[2] << std::endl;
      std::cout << i << "Ks: " << materials[i].Ks[0] << ", " << materials[i].Ks[1] << ", " << materials[i].Ks[2] << std::endl;
      std::cout << i << "map_Ka: " << materials[i].map_Ka << std::endl;
      std::cout << i << "map_Kd: " << materials[i].map_Kd << std::endl;
      std::cout << i << "map_d:  " << materials[i].map_d << std::endl;
  }*/
    
  // GLFWを初期化し、ウィンドウを作成
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
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

