#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"

const unsigned int WINDOW_WIDTH = 1440;
const unsigned int WINDOW_HEIGHT = 810;

void keyHandler(GLFWwindow*, int, int, int, int);

void resize(int w, int h);

int main()
{
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

    GLfloat positionData[] = 
    {
        -0.8f, -0.8f, -0.8f,
        -0.8f, -0.8f, 0.8f,
        -0.8f, 0.8f, -0.8f,
        -0.8f, 0.8f, 0.8f,
        0.8f, -0.8f, -0.8f,
        0.8f, -0.8f, 0.8f,
        0.8f, 0.8f, -0.8f,
        0.8f, 0.8f, 0.8f,
    };
    GLfloat colorData[] = 
    {
      1.0f, 1.0f, 0.0f, 
      0.0f, 1.0f, 0.0f, 
      0.0f, 1.0f, 1.0f, 
      0.0f, 1.0f, 0.0f, 
      0.0f, 1.0f, 0.0f, 
      0.0f, 1.0f, 0.0f, 
      0.0f, 1.0f, 0.0f, 
      0.0f, 1.0f, 0.0f
    };
    GLuint elementData[] = 
    {
      2, 0, 4,
      4, 6, 2,

      6, 4, 5,
      6, 5, 7,

      3, 7, 5,
      3, 5, 1,

      3, 2, 1,
      2, 1, 0,

      2, 6, 3,
      3, 6, 7,

      0, 4, 1, 
      1, 4, 5
    };

    std::string vert_shader_file = "../shader/v460.vert";
    std::string frag_shader_file = "../shader/v460.frag";
    use_shader(vert_shader_file, frag_shader_file, positionData, colorData, elementData);

    while (!glfwWindowShouldClose(window))
    {
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
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