std::string read_shader_file(std::string &f);

int use_shader(std::string &vrt_path,
               std::string &frg_path,
               GLfloat positionData[],
               GLfloat colorData[],
               GLuint elementData[]);

int use_shader(std::string &vrt_path,
               std::string &frg_path);