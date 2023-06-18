#include <glad/glad.h>

#include <iostream>

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>

#include "objloader.h"
#include "shader.h"

const unsigned int WINDOW_WIDTH = 810;
const unsigned int WINDOW_HEIGHT = 810;

// ------
// callback
// -------

void keyHandler(GLFWwindow *window, int key, int scancode, int action,
                int mods) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        break;
    }
}

void resize(int w, int h) {
    std::cout << w << ":" << h << std::endl;
    glViewport(0, 0, w, h);
}

// ------
// main
// ------
//
int main() {
    std::string path_obj =
        std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.obj";
    std::string path_mtl =
        std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.mtl";
    std::string path_dir =
        std::string(PATH_ROOT_DIR) + "/test_data/objInfo/lion";

    Object obj = Object(path_obj, path_mtl);
    obj.load_obj();
    obj.load_mtl();

    // for (auto& v : obj.m_aNrm) { v *= -1; }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, "06_ProgrammableShader", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyHandler);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::string vert_shader_file = "../shader/v460.vert";
    std::string frag_shader_file = "../shader/v460.frag";
    GLuint programHandle =
        use_shader(vert_shader_file, frag_shader_file, obj.m_aXYZ, obj.m_aNrm,
                   obj.m_aTex, obj.m_aTri_XYZ);

    glEnable(GL_DEPTH_TEST);

    std::cout << "aTri_XYZ.size(): " << obj.m_aTri_XYZ.size() << std::endl;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float time = glfwGetTime();
        glm::mat4 projection =
            glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(
            glm::vec3(glm::cos(time), 0.0f, glm::sin(time)),
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 mvp = projection * view;

        GLuint location = glGetUniformLocation(programHandle, "MVP");

        if (location >= 0) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
        }

        glDrawElements(GL_TRIANGLES, obj.m_aTri_XYZ.size(), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
