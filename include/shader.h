#include <string>
#include <vector>

std::string read_shader_file(std::string &f);

int use_shader(std::string &vrt_path, std::string &frg_path,
               std::vector<float> aXYZ, std::vector<float> aNrm,
               std::vector<float> aTex, std::vector<unsigned int> aTri_XYZ);

int use_shader(std::string &vrt_path, std::string &frg_path);