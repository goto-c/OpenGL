#include <vector>
std::string read_shader_file(std::string &f);

int use_shader(std::string &vrt_path,
               std::string &frg_path,
               std::vector<double> aXYZ,
               std::vector<unsigned int> aTri_XYZ);

int use_shader(std::string &vrt_path,
               std::string &frg_path);