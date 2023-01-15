
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

std::string read_shader_file(std::string &f)
{
    std::string source;
    source.clear();

    std::ifstream fin;
    fin.open(f);
    if(fin.fail())
        std::cout << "shader source file open failed" << std::endl;
    
    const int BUFF_SIZE = 1024;
    char buff[BUFF_SIZE];
    
    while (!fin.eof()){
        fin.getline(buff, BUFF_SIZE);
        source += buff;
        source += "\n";
    }
    source.pop_back();
    source += "\0";
    
    fin.close();

    return source;
}

int use_shader(std::string &vrt_path,
               std::string &frg_path,
               std::vector<double> aXYZ,
               std::vector<unsigned int> aTri_XYZ)
{

    std::string sglslvrt = read_shader_file(vrt_path);
    std::string sglslfrg = read_shader_file(frg_path);
    const char *glslvrt = sglslvrt.c_str();
    const char *glslfrg = sglslfrg.c_str();
    
    std::cout << "read vrt_shader: " << std::endl;
    std::cout << read_shader_file(vrt_path) << std::endl;
    std::cout << "read frg_shader: " << std::endl;
    std::cout << read_shader_file(frg_path) << std::endl;
    
    if (glslvrt == NULL){
        std::cout << "Could not read file: " << vrt_path << std::endl;
        return -1;
    }
    
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
    
    if (glslfrg == NULL){
        std::cout << "Could not read file: " << frg_path << std::endl;
        return -1;
    }
    
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &glslfrg, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }
    
    GLuint programHandle;
     
    // link shader
    programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(programHandle, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED" << infoLog << std::endl;
    }
    else {
        glUseProgram(programHandle);
    }
    std::cout << vertexShader << " " << fragmentShader << " shader program: " << programHandle << std::endl;

    GLuint vaoHandle;

    GLuint vaoHandles[3];
    glGenBuffers(3, vaoHandles);
    GLuint positionBufferHandle = vaoHandles[0];
    GLuint colorBufferHandle = vaoHandles[1];
    GLuint elementBufferHandle = vaoHandles[2];

    // GLfloat a = 0.5f;
    // GLfloat vert[] = {
       // -a, -a, 0.0f,
       // a, -a, 0.0f,
       // 0.0f, a, 0.0f
    // };
    // GLfloat color[] = {
        // 0.0, 1.0, 1.0,
        // 1.0, 0.0, 1.0,
        // 1.0, 1.0, 0.0
    // };
    // GLuint elem[] = {
        // 0, 1, 2
    // };

    // GLfloat positionData[] = 
    // {
        // -a, -a, -a,
        // -a, -a, a,
        // -a, a, -a,
        // -a, a, a,
        // a, -a, -a,
        // a, -a, a,
        // a, a, -a,
        // a, a, a,
    // };
    // GLfloat colorData[] = 
    // {
      // 1.0f, 1.0f, 1.0f, 
      // 1.0f, 1.0f, 0.0f, 
      // 0.0f, 1.0f, 0.0f, 
      // 1.0f, 0.0f, 0.0f, 
      // 0.0f, 1.0f, 1.0f, 
      // 0.0f, 1.0f, 0.0f, 
      // 0.0f, 0.0f, 1.0f, 
      // 0.5f, 0.5f, 0.5f
    // };
    // GLuint elementData[] = 
    // {
      // 2, 0, 4,
      // 4, 6, 2,

      // 6, 4, 5,
      // 6, 5, 7,

      // 3, 7, 5,
      // 3, 5, 1,

      // 3, 1, 2,
      // 2, 1, 0,

      // 2, 6, 3,
      // 3, 6, 7,

      // 0, 1, 4, 
      // 1, 5, 4
    // };
    std::vector<GLfloat> color_white(aXYZ.size(), 1.0f);

    // Send data to buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, aXYZ.size()*sizeof(double), aXYZ.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, color_white.size()*sizeof(GLfloat), color_white.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, aTri_XYZ.size()*sizeof(unsigned int), aTri_XYZ.data(), GL_STATIC_DRAW);

    // 
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (GLubyte *)NULL);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferHandle);
    // 

    glBindVertexArray(vaoHandle);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return programHandle;
}

int use_shader(std::string &vrt_path,
               std::string &frg_path)
{
    std::string sglslvrt = read_shader_file(vrt_path);
    std::string sglslfrg = read_shader_file(frg_path);
    const char *glslvrt = sglslvrt.c_str();
    const char *glslfrg = sglslfrg.c_str();
    
    std::cout << "read vrt_shader: " << std::endl;
    std::cout << read_shader_file(vrt_path) << std::endl;
    std::cout << "read frg_shader: " << std::endl;
    std::cout << read_shader_file(frg_path) << std::endl;
    
    if (glslvrt == NULL){
        std::cout << "Could not read file: " << vrt_path << std::endl;
        return -1;
    }
    
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
    
    if (glslfrg == NULL){
        std::cout << "Could not read file: " << frg_path << std::endl;
        return -1;
    }
    
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &glslfrg, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }
    
    GLuint programHandle;
     
    // link shader
    programHandle = glCreateProgram(); // プログラムのIDを取得
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(programHandle, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED" << infoLog << std::endl;
    }
    else {
        glUseProgram(programHandle);
    }
    std::cout << vertexShader << " " << fragmentShader << " shader program: " << programHandle << std::endl;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //glUseProgram(programHandle);
    return programHandle;
}
