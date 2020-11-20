#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    
    // link shader
    int shaderProgram;
    shaderProgram = glCreateProgram(); // プログラムのIDを取得
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKAGE_FAILED" << infoLog << std::endl;
    }
    
    std::cout << vertexShader << " " << fragmentShader << " shader program: " << shaderProgram << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //glUseProgram(shaderProgram);
    return shaderProgram;
}
