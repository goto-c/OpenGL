#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"

const unsigned int WINDOW_WIDTH = 1440;
const unsigned int WINDOW_HEIGHT = 810;

void keyHandler(GLFWwindow*, int, int, int, int);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

    GLfloat positionData[] = 
    {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };
    GLfloat colorData[] = 
    {
      0.0f, 1.0f, 0.0f, 
      0.0f, 1.0f, 0.0f, 
      0.0f, 0.0f, 1.0f
    };

    std::string vert_shader_file = "../shader/v460.vert";
    std::string frag_shader_file = "../shader/v460.frag";
    use_shader(vert_shader_file, frag_shader_file, positionData, colorData);

    while (!glfwWindowShouldClose(window))
    {
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