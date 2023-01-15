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

#include <objloader.h>

/* --------------- callback ------------------*/

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


/* ------------------ display -----------------*/

void display(const std::vector<double>& aXYZ,
             const std::vector<double>& aNrm,
             const std::vector<double>& aTex,
             const std::vector<unsigned int>& aTri_XYZ,
             const std::vector<unsigned int>& aTri_Tex,
             const std::vector<unsigned int>& aTri_Nrm,
             const std::vector<CMaterialMap>& aMtlMap,
             const std::vector<CMaterialInfo>& aMtlInfo)
{
  ::glEnable(GL_LIGHTING);
  for (const auto& mm : aMtlMap){
//      std::cout << "mm: " << mm.name << std::endl;
      unsigned int imi = mm.iMaterialInfo;
//      std::cout << "mm.MaterialInfo : " << mm.iMaterialInfo << std::endl;
//      std::cout << "aMaterialInfo.size() : " << aMtlInfo.size() << std::endl;
      if (imi < 0 || imi >= aMtlInfo.size()){
          ::glDisable(GL_TEXTURE_2D);
          const GLfloat color_white[] = { 1.0, 1.0, 1.0, 1.0 };
          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color_white);
      }
      else {
          const auto& mi = aMtlInfo[imi];
//          std::cout << "mi.Kd : " << *mi.Kd << std::endl;
          if (mi.Kd[0] != 0){
              ::glMaterialfv(GL_FRONT, GL_DIFFUSE, mi.Kd);
          }
          else{
              float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
              ::glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
          }
          if ( ::glIsTexture(mi.idTex_Kd) ){
//              std::cout << "mi.idTex_Kd : " << mi.idTex_Kd << std::endl;
              ::glBindTexture(GL_TEXTURE_2D, mi.idTex_Kd);
              ::glEnable(GL_TEXTURE_2D);
          }
          else {
              ::glDisable(GL_TEXTURE_2D);
          }
      }
      
      ::glBegin(GL_TRIANGLES);
      for (unsigned int itri = mm.itri_start; itri<mm.itri_end; itri++){
          // aTex.size() = 104078
          // aTri_Tex.size() = 300018
          // mm.itri_start = 0
          // mm.itri_end = 100006
          //std::cout << "start: " << mm.itri_start << "end: " << mm.itri_end << std::endl;
//          std::cout << itri << " : " << aTri_Tex[itri*3+0] * 2;
          ::glTexCoord2dv(aTex.data() + aTri_Tex[itri*3+0] * 2);
          if(aNrm.size()!=0){
              ::glNormal3dv(aNrm.data() + aTri_Nrm[itri*3+0] * 3);
          }
          ::glVertex3dv(aXYZ.data() + aTri_XYZ[itri*3+0] * 3);

          ::glTexCoord2dv(aTex.data() + aTri_Tex[itri*3+1] * 2);
          if(aNrm.size()!=0){
              ::glNormal3dv(aNrm.data() + aTri_Nrm[itri*3+1] * 3);
          }
          ::glVertex3dv(aXYZ.data() + aTri_XYZ[itri*3+1] * 3);

          ::glTexCoord2dv(aTex.data() + aTri_Tex[itri*3+2] * 2);
          if(aNrm.size()!=0){
              ::glNormal3dv(aNrm.data() + aTri_Nrm[itri*3+2] * 3);
          }
          ::glVertex3dv(aXYZ.data() + aTri_XYZ[itri*3+2] * 3);
      }
      glEnd();

      /*
      ::glBegin(GL_TRIANGLES);
      for (unsigned int itri = mm.itri_start; itri<mm.itri_end; itri++){
          // aTex.size() = 104078
          // aTri_Tex.size() = 300018
          // mm.itri_start = 0
          // mm.itri_end = 100006
          //std::cout << "start: " << mm.itri_start << "end: " << mm.itri_end << std::endl;
          std::cout << "aTex.max() : " << "" << std::endl;
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
      glEnd();*/
  }
}


/* ------------------ main --------------------*/

int main(void)
{
    
//  std::string path_obj = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.obj";
//  std::string path_mtl = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.mtl";
//  std::string path_dir = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug";
    std::string path_obj = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.obj";
    std::string path_mtl = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.mtl";
    std::string path_dir = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug";
    
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
    std::cout << "aNrm.size : " << aNrm.size() << std::endl;
    std::cout << "aTri_Nrm.size : " << aTri_Nrm.size() << std::endl;
    std::cout << "contents of aNrm : " << std::endl;
    for (int i=0; i<aNrm.size(); i++){
        std::cout << aNrm[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "contents of aTri_Nrm : " << std::endl;
    for (int i=0; i<aTri_Nrm.size(); i++){
        std::cout << aTri_Nrm[i] << std::endl;
    }
  std::cout << "obj file loading succeeded " << std::endl;
    
//    std::cout << "contents of aXYZ : " << std::endl;
//  for (int i=0; i<aXYZ.size()/3; i++){
//      std::cout << aXYZ[3*i] << ", " << aXYZ[3*i+1] << ", " << aXYZ[3*i+2] << std::endl;
//  }
//    std::cout << std::endl;
//    std::cout << "contents of aTri_XYZ : " << std::endl;
//  for (int i=0; i<aTri_XYZ.size()/3; i++){
//      std::cout << aTri_XYZ[3*i] << ", " << aTri_XYZ[3*i+1] << ", " << aTri_XYZ[3*i+2] << std::endl;
//  }
//        std::cout << "contents of aTex : " << std::endl;
//      for (int i=0; i<aTex.size()/3; i++){
//          std::cout << aTex[3*i] << ", " << aTex[3*i+1] << ", " << aTex[3*i+2] << std::endl;
//      }
//        std::cout << std::endl;
//        std::cout << "contents of aTri_Tex : " << std::endl;
//      for (int i=0; i<aTri_Tex.size()/3; i++){
//          std::cout << aTri_Tex[3*i] << ", " << aTri_Tex[3*i+1] << ", " << aTri_Tex[3*i+2] << std::endl;
//      }
  std::cout << "number of vertices : " << aXYZ.size()/3 << std::endl;
  std::cout << "number of faces    : " << aTri_XYZ.size()/3 << std::endl;
  /* --------------------------- */
    
  //for (auto& v : aXYZ) { v *= 0.01; }
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
    std::cout << "size of MtlInfo : " << aMtlInfo.size() << std::endl;
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
    
  {
//      GLfloat lightpos[] = { 1.0, 1.0, 1.0, 0.5 };
      GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 };
      GLfloat lightdiff[] = { 1.0, 1.0, 1.0, 1.0 };
      GLfloat light0pos[] = { 5.0, 0.0, 0.0, 1.0 };
      GLfloat light1pos[] = { -5.0, 0.0, 0.0, 1.0 };
      GLfloat light2pos[] = { 0.0, 0.0, 5.0, 1.0 };
      GLfloat light3pos[] = { 0.0, 0.0, -5.0, 1.0 };

      /* 光源の初期設定 */
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glEnable(GL_LIGHT1);
      glEnable(GL_LIGHT2);
      glEnable(GL_LIGHT3);
//      glLightfv(GL_LIGHT0, GL_DIFFUSE, lightpos);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);
      glLightfv(GL_LIGHT1, GL_DIFFUSE, lightdiff);
      glLightfv(GL_LIGHT2, GL_DIFFUSE, lightdiff);
      glLightfv(GL_LIGHT3, GL_DIFFUSE, lightdiff);
      glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
      glLightfv(GL_LIGHT1, GL_SPECULAR, lightcol);
      glLightfv(GL_LIGHT2, GL_SPECULAR, lightcol);
      glLightfv(GL_LIGHT3, GL_SPECULAR, lightcol);
      glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
      glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
      glLightfv(GL_LIGHT2, GL_POSITION, light2pos);
      glLightfv(GL_LIGHT3, GL_POSITION, light3pos);
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
//    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(0.0f, -0.9f, 0.0f);
//    glTranslatef(0.0f, -0.5f, 0.0f);
    
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

