#include <GLFW/glfw3.h>

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

// strip_slash for read_obj
std::vector<unsigned int> strip_slash(std::string s);

void read_obj(std::vector<double>& aXYZ,
std::vector<double>& aNrm,
std::vector<double>& aTex,
std::vector<unsigned int>& aTri_XYZ,
std::vector<unsigned int>& aTri_Tex,
std::vector<unsigned int>& aTri_Nrm,
std::vector<CMaterialMap>& aMaterialMap,
const std::string& file_obj);

void read_mtl(std::vector<CMaterialInfo>& aMat,
const std::string& file_mtl);
