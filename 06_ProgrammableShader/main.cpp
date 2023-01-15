#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "objloader.h"

const unsigned int WINDOW_WIDTH = 810;
const unsigned int WINDOW_HEIGHT = 810;

void keyHandler(GLFWwindow*, int, int, int, int);

void resize(int w, int h);

int main()
{
    std::string path_obj = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.obj";
    std::string path_mtl = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.mtl";
    std::string path_dir = std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug";

    std::vector<double> aXYZ, aNrm, aTex;
    std::vector<unsigned int> aTri_XYZ, aTri_Tex, aTri_Nrm;
    std::vector<CMaterialMap> aMtlMap;
    std::vector<CMaterialInfo> aMtlInfo;
    
    read_obj(aXYZ, aNrm, aTex,
               aTri_XYZ, aTri_Tex, aTri_Nrm,
               aMtlMap,
            path_obj);
    
    // for (auto& v : aNrm) { v *= -1; }
    
    read_mtl(aMtlInfo,
               path_mtl);
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "06_ProgrammableShader", nullptr, nullptr);

    if (!window) 
    {
        std::cerr << "Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyHandler);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    std::string vert_shader_file = "../shader/v460.vert";
    std::string frag_shader_file = "../shader/v460.frag";
    GLuint programHandle = use_shader(vert_shader_file, frag_shader_file, aXYZ, aTri_XYZ);

    std::cout << "aTri_XYZ.size(): " << aTri_XYZ.size() << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 1.0f));

        GLuint location = glGetUniformLocation(programHandle, "RotationMatrix");

        if (location >= 0)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
        }

        glDrawElements(GL_TRIANGLES, aTri_XYZ.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void keyHandler(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    switch (key) 
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            break;
    }
}

void resize(int w, int h){
    std::cout << w << ":" << h << std::endl;
    glViewport(0, 0, w, h);
}