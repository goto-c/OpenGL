#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// mtlファイルのmaterial情報を格納する
class CMaterialInfo {
public:
    std::string name;
    float Ka[4];
    float Kd[4];
    float Ks[4];
    int d;
    std::string map_Ka;
    std::string map_Kd;
    std::string map_d;
    GLuint idTex_Kd = 0;
};

// objファイルのmaterial使用場所を格納する
class CMaterialMap {
    public:
    std::string name;
    unsigned int itri_start;
    unsigned int itri_end;
    int iMaterialInfo;
};


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
void read_obj(std::vector<double>& aXYZ,
              std::vector<double>& aNrm,
              std::vector<double>& aTex,
              std::vector<unsigned int>& aTri_XYZ,
              std::vector<unsigned int>& aTri_Tex,
              std::vector<unsigned int>& aTri_Nrm,
              std::vector<CMaterialMap>& aMaterialMap,
              const std::string& file_obj)
{
    aMaterialMap.clear();
    
    std::ifstream fin;
    fin.open(file_obj);
    if (fin.fail())
        std::cout << "obj file open failed" << std::endl;
    
    aXYZ.clear();
    aNrm.clear();
    aTex.clear();
    aTri_XYZ.clear();
    aTri_Tex.clear();
    aTri_Nrm.clear();
    
    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    int icount_tri = 0;
    
    while(!fin.eof()){
        fin.getline(buff, BUFF_SIZE);
        
        // 先頭が'v'の場合
        if (buff[0]=='v' && buff[1]==' '){
            char str[256];
            double x, y, z;
            sscanf(buff, "%s %lf %lf %lf", str, &x, &y, &z);
            aXYZ.push_back(x);
            aXYZ.push_back(y);
            aXYZ.push_back(z);
        }
        // 先頭が'vn'の場合
        if (buff[0]=='v' && buff[1]=='n'){
            char str[256];
            double x, y, z;
            sscanf(buff, "%s %lf %lf %lf", str, &x, &y, &z);
            aNrm.push_back(x);
            aNrm.push_back(y);
            aNrm.push_back(z);
        }
        // 先頭が'vt'の場合
        if (buff[0]=='v' && buff[1]=='t'){
            char str[256];
            double s, t;
            sscanf(buff, "%s %lf %lf", str, &s, &t);
            aTex.push_back(s);
            aTex.push_back(t);
        }
        // 先頭が'us'の場合
        if (buff[0]=='u' && buff[1]=='s'){
            char str[256], mtl_name[1024];
            sscanf(buff, "%s %s", str, mtl_name);
            {
                CMaterialMap mm;
                mm.itri_start = icount_tri;
                mm.name = std::string(mtl_name);
                aMaterialMap.push_back(mm);
                std::cout << "usemtl: " << mm.name << std::endl;
            }
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
            aTri_XYZ.push_back(vertex0[0]-1);
            aTri_XYZ.push_back(vertex1[0]-1);
            aTri_XYZ.push_back(vertex2[0]-1);
            aTri_Tex.push_back(vertex0[1]-1);
            aTri_Tex.push_back(vertex1[1]-1);
            aTri_Tex.push_back(vertex2[1]-1);
            aTri_Nrm.push_back(vertex0[2]-1);
            aTri_Nrm.push_back(vertex1[2]-1);
            aTri_Nrm.push_back(vertex2[2]-1);
            // 四角形メッシュを加える場合、三角形に分割
            if (i3[0]!='\0'){
                std::vector<unsigned int> vertex3;
                vertex3.clear();
                vertex3 = strip_slash(i3);
                aTri_XYZ.push_back(vertex0[0]-1);
                aTri_XYZ.push_back(vertex2[0]-1);
                aTri_XYZ.push_back(vertex3[0]-1);
                aTri_Tex.push_back(vertex0[1]-1);
                aTri_Tex.push_back(vertex2[1]-1);
                aTri_Tex.push_back(vertex3[1]-1);
                aTri_Nrm.push_back(vertex0[2]-1);
                aTri_Nrm.push_back(vertex2[2]-1);
                aTri_Nrm.push_back(vertex3[2]-1);
                icount_tri++;
            }
            icount_tri++;
        }
    }
    fin.close();
    
    for (unsigned int imm=0; imm<aMaterialMap.size()-1; imm++){
        aMaterialMap[imm].itri_end = aMaterialMap[imm+1].itri_start;
    }
    aMaterialMap[aMaterialMap.size()-1].itri_end = aTri_XYZ.size()/3;
}


/* ----------------- mtl ファイル読み込み用の関数 ---------------*/

void read_mtl(std::vector<CMaterialInfo>& aMat,
              const std::string& file_mtl)
{
    std::ifstream fin(file_mtl);
    if (fin.fail()){
        std::cout << "mtl file open failed" << std::endl;
        return;
    }
    
    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    
    aMat.clear();
    CMaterialInfo mi;
    
    while (!fin.eof()){
        fin.getline(buff, BUFF_SIZE);
        
        // コメント行は飛ばす
        if (buff[0]=='#') continue;
        
        // regigter material
        if ( strncmp(buff, "newmtl", 6) == 0 ){
            //aMat.push_back(mi);
            mi.map_d.clear();
            mi.map_Ka.clear();
            mi.map_Kd.clear();
            char str[256], name[1024];
            sscanf(buff, "%s %s", str, name);
            std::cout << "newmtl: " << name << std::endl;
            mi.name = name;
        }
        // 環境光・拡散光・鏡面光のRGB値を構造体のメンバに追加
        if (buff[1]=='K' && buff[2]=='a'){
            char str[256];
            double r, g, b;
            sscanf(buff, "%s %lf %lf %lf", str, &r, &g, &b);
            mi.Ka[0] = r;
            mi.Ka[1] = g;
            mi.Ka[2] = b;
            mi.Ka[3] = 1.f;
        }
        if (buff[1]=='K' && buff[2]=='d'){
            char str[256];
            double r, g, b;
            sscanf(buff, "%s %lf %lf %lf", str, &r, &g, &b);
            mi.Kd[0] = r;
            mi.Kd[1] = g;
            mi.Kd[2] = b;
            mi.Kd[3] = 1.f;
            std::cout << "Kd _: " << mi.Kd[0] << ", " << mi.Kd[1] << ", " << mi.Kd[2] << std::endl;
        }
        if (buff[1]=='K' && buff[2]=='s'){
            char str[256];
            double r, g, b;
            sscanf(buff, "%s %lf %lf %lf", str, &r, &g, &b);
            mi.Ks[0] = r;
            mi.Ks[1] = g;
            mi.Ks[2] = b;
            mi.Ks[3] = 1.f;
        }
        // テクスチャ画像ファイル名を構造体メンバに追加
        /*if (buff[1]=='m' && buff[5]=='K' && buff[6]=='a'){
            char str[256], map[1024];
            sscanf(buff, "%s %s", str, map);
            mi.map_Ka = map;
        }*/
        if (buff[1]=='m' && buff[5]=='K' && buff[6]=='d'){
            char str[256], map[1024];
            sscanf(buff, "%s %s", str, map);
            mi.map_Kd = map;
            std::cout << "map_Kd _: " << mi.map_Kd << std::endl;
            
            aMat.push_back(mi);
        }
        /*if (buff[1]=='m' && buff[5]=='d'){
            char str[256], map[1024];
            sscanf(buff, "%s %s", str, map);
            mi.map_d = map;
        }*/
    }
    /*for (int i=0; i<3; i++){
        std::cout << "aMat.size(): " << aMat.size() << std::endl;
        std::cout << "aMat.Kd: " << aMat[i].Kd[0] << std::endl;
        std::cout << "aMat.map_Kd: " << aMat[i].map_Kd << std::endl;
        std::cout << std::endl;
    }*/
}

/* ------------------ ディスプレイ関数 -----------------*/

int shaderProgram;
unsigned int VAO;
unsigned int VBO_Tri[3];
unsigned int EBO_Tri;

void display(const std::vector<double>& aXYZ,
             const std::vector<double>& aNrm,
             const std::vector<double>& aTex,
             const std::vector<unsigned int>& aTri_XYZ,
             const std::vector<unsigned int>& aTri_Tex,
             const std::vector<unsigned int>& aTri_Nrm,
             const std::vector<CMaterialMap>& aMtlMap,
             const std::vector<CMaterialInfo>& aMtlInfo)
{
//  glUseProgram(0);
  ::glEnable(GL_LIGHTING);
  for (const auto& mm : aMtlMap){ // mmにマテリアルマップを保存
      int imi = mm.iMaterialInfo;
      if (imi < 0 || imi >= aMtlInfo.size()){
          ::glDisable(GL_TEXTURE_2D);
          const GLfloat color_white[] = { 1.0, 1.0, 1.0, 1.0 };
          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color_white);
      }
      else {
          //std::cout << "texture exist" << std::endl;
          const auto& mi = aMtlInfo[imi]; // miにマテリアル情報を保存
          ::glMaterialfv(GL_FRONT, GL_DIFFUSE, mi.Kd);
          if ( ::glIsTexture(mi.idTex_Kd) ){
              //std::cout << "Bind texture : " << mi.idTex_Kd << std::endl;
              ::glBindTexture(GL_TEXTURE_2D, mi.idTex_Kd);
              ::glEnable(GL_TEXTURE_2D);
          }
          else {
              ::glDisable(GL_TEXTURE_2D);
          }
      }
      
      ::glBegin(GL_TRIANGLES); // 描画ごとに頂点情報を送る（遅い）
      for (unsigned int itri = mm.itri_start; itri<mm.itri_end; itri++){
          //std::cout << "start: " << mm.itri_start << "end: " << mm.itri_end << std::endl;
          ::glTexCoord2dv(aTex.data() + aTri_Tex[itri*3+0] * 2);
          ::glNormal3dv(aNrm.data() + aTri_Nrm[itri*3+0] * 3);
          ::glVertex3dv(aXYZ.data() + aTri_XYZ[itri*3+0] * 3);
          
          ::glTexCoord2dv(aTex.data() + aTri_Tex[itri*3+1] * 2);
          ::glNormal3dv(aNrm.data() + aTri_Nrm[itri*3+1] * 3);
          ::glVertex3dv(aXYZ.data() + aTri_XYZ[itri*3+1] * 3);
          
          ::glTexCoord2dv(aTex.data() + aTri_Tex[itri*3+2] * 2);
          ::glNormal3dv(aNrm.data() + aTri_Nrm[itri*3+2] * 3);
          ::glVertex3dv(aXYZ.data() + aTri_XYZ[itri*3+2] * 3);
      }
      glEnd();
  }
}

 
/* ------------------ main 関数 --------------------*/

int main(void)
{
    
  std::string path_obj = std::string(PATH_ROOT_DIR) + "/data/objInfo/bug/data.obj";
  std::string path_mtl = std::string(PATH_ROOT_DIR) + "/data/objInfo/bug/data.mtl";
  std::string path_dir = std::string(PATH_ROOT_DIR) + "/data/objInfo/bug";
    
  std::cout << path_obj << " " << path_mtl << std::endl;
    
  std::vector<double> aXYZ;
  std::vector<double> aNrm;
  std::vector<double> aTex;
  std::vector<unsigned int> aTri_XYZ;
  std::vector<unsigned int> aTri_Tex;
  std::vector<unsigned int> aTri_Nrm;
  std::vector<CMaterialMap> aMtlMap;
  std::vector<CMaterialInfo> aMtlInfo;
    
  /* Read and check the obj data */
  read_obj(aXYZ, aNrm, aTex,
           aTri_XYZ, aTri_Tex, aTri_Nrm,
           aMtlMap,
           path_obj);
  std::cout << "obj file loading succeeded " << std::endl;
    
  /*for (int i=0; i<aXYZ.size()/3; i++){
      std::cout << aXYZ[3*i] << ", " << aXYZ[3*i+1] << ", " << aXYZ[3*i+2] << std::endl;
  }
  for (int i=0; i<aTri_XYZ.size()/3; i++){
      std::cout << aTri_XYZ[3*i] << ", " << aTri_XYZ[3*i+1] << ", " << aTri_XYZ[3*i+2] << std::endl;
  }*/
  std::cout << "number of vertices : " << aXYZ.size()/3 << std::endl;
  std::cout << "number of faces    : " << aTri_XYZ.size()/3 << std::endl;
  /* --------------------------- */
    
  //for (auto& v : aXYZ) { v *= 0.1; }
  for (auto& v : aNrm) { v *= -1; }
    
  /* Read and check the mtl data */
  read_mtl(aMtlInfo,
           path_mtl);
  std::cout << "mtl file loading succeeded " << std::endl;
  std::cout << std::endl;
  std::cout << "number of material refered from obj : " << aMtlMap.size() << std::endl;
  std::cout << "number of material defined in mtl : " << aMtlInfo.size() << std::endl;
    
  for (const auto& mi : aMtlInfo){
      std::cout << std::endl;
      std::cout << "material information" << std::endl;
      std::cout << "   name : " << mi.name << std::endl;
      std::cout << "   Kd : " << mi.Kd[0] << " " << mi.Kd[1] << " " << mi.Kd[2] << std::endl;
      std::cout << "   map_Kd : " << mi.map_Kd << std::endl;
      std::cout << std::endl;
  }
  /* -------------------------- */
    
  // GLFWを初期化し、ウィンドウを作成
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);
    
    // 新しいものを使う場合、majorを4とかにする。
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    
  // ウィンドウ作成に失敗 -> 終了
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  // 作成したウィンドウをOpenGLの処理対象にする
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
    
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  }
  
  {
      // glsl version 120 shader toy
      // vertex shader
      const char *glslvrt =
      "#version 120\n"
      "varying vec3 normal;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = ftransform();\n"
      "   normal = vec3(gl_NormalMatrix    * gl_Normal);\n"
      "}\0";
      // gl_NormalMatrixは回転行列
      // ftransform()glVertexとかで送った情報に回転をかけたもの
      
      // fragment shader
      const char *glslfrg =
      "#version 120\n"
      "varying vec3 normal;\n"
      "void main()\n"
      "{\n"
      "   gl_FragColor = vec4(0.5*normal+0.5,1);\n"
      "}\n\0";
      
      // vertex shader
      int vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, 1, &glslvrt, nullptr);
      glCompileShader(vertexShader);
      int success;
      char infoLog[512];
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
          glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
          std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
      }
      
      // fragment shader
      int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &glslfrg, nullptr);
      glCompileShader(fragmentShader);
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
          glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
          std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
      }
      
      // link shader
      shaderProgram = glCreateProgram(); //プログラムのIDを取得
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
      glLinkProgram(shaderProgram);
      glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
      if(!success){
          glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
          std::cout << "ERROR::SHADER::PRORAM::LINKAGE_FAILED" << infoLog << std::endl;
      }
      std::cout << fragmentShader << " " << vertexShader << " shader program: " << shaderProgram << std::endl;
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
    
  }
        
  
  {
      GLfloat lightpos[] = { 1.0, 1.0, 1.0, 0.5 };
      GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 };
      /* 光源の初期設定 */
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, lightpos);
      glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  }
    
  for (auto& mm : aMtlMap){
      mm.iMaterialInfo = -1;
      
      for (unsigned int imi=0; imi<aMtlInfo.size(); imi++){
          if (mm.name != aMtlInfo[imi].name) { continue; }
          
          mm.iMaterialInfo = imi;
          CMaterialInfo& mi = aMtlInfo[imi];
          
          if (mi.map_Kd.empty()) continue;
          if (mi.idTex_Kd != 0)  continue;
          
          glGenTextures(1, &(mi.idTex_Kd));
          glBindTexture(GL_TEXTURE_2D, mi.idTex_Kd);
          
          int width, height;
          int channels;
          std::string img_path = path_dir + "/" + mi.map_Kd;
          
          stbi_set_flip_vertically_on_load(true);
          unsigned char *img = stbi_load( img_path.c_str(), &width, &height, &channels, 3 );
          
          glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                       GL_RGB, GL_UNSIGNED_BYTE, img);
          delete[] img;
          glBindTexture(GL_TEXTURE_2D, 0);
          break;
      }
  }

    
  // ウィンドウが表示されている間、繰り返す
  while (!glfwWindowShouldClose(window))
  {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
      
    glViewport(0, 0, width, height);
      
    glClearColor(0.7f, 0.6f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0f, -0.5f, 0.0f);
      
    glUseProgram(shaderProgram); //この後のglBegin, glEndには全てシェーダが適用される。0番は、デフォルトのシェーダ適用
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Tri);
    
    display(aXYZ, aNrm, aTex,
            aTri_XYZ, aTri_Tex, aTri_Nrm,
            aMtlMap, aMtlInfo);
      
    // カラーバッファを入れ替える
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
