#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

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
    unsigned int iMaterialInfo;
};

// ------
// OBJECT Class
// ------
class OBJECT {

  public:
    OBJECT(std::string obj_file_path) : m_obj_file_path(obj_file_path) {}
    OBJECT(std::string obj_file_path, std::string mtl_file_path)
        : m_obj_file_path(obj_file_path), m_mtl_file_path(mtl_file_path) {}
    int load_obj();
    int load_mtl();

    std::vector<float> m_aXYZ;
    std::vector<float> m_aNrm;
    std::vector<float> m_aTex;
    std::vector<unsigned int> m_aTri_XYZ;
    std::vector<unsigned int> m_aTri_Tex;
    std::vector<unsigned int> m_aTri_Nrm;
    std::vector<CMaterialMap> m_aMtlMap;
    std::vector<CMaterialInfo> m_aMtlInfo;

  private:
    std::string m_obj_file_path;
    std::string m_mtl_file_path;
};

// strip_slash for read_obj
std::vector<unsigned int> strip_slash(std::string s);

void read_flag(std::vector<unsigned int> &flag, const std::string &file_flag);
