#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include "stb_image_write.h"

#include "objloader.h"
#include "shader.h"

// ------
// callback
// ------
//
static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize(int w, int h) {
    std::cout << w << ":" << h << std::endl;
    glViewport(0, 0, w, h);
}

// ------
// display
// ------
//
void display(const std::vector<float> &aXYZ, const std::vector<float> &aNrm,
             const std::vector<float> &aTex,
             const std::vector<unsigned int> &aTri_XYZ,
             const std::vector<unsigned int> &aTri_Tex,
             const std::vector<unsigned int> &aTri_Nrm,
             const std::vector<CMaterialMap> &aMtlMap,
             const std::vector<CMaterialInfo> &aMtlInfo) {
    //  glUseProgram(0);
    ::glEnable(GL_LIGHTING);
    for (const auto &mm : aMtlMap) { // mmにマテリアルマップを保存
        unsigned int imi = mm.iMaterialInfo;
        if (imi < 0 || imi >= aMtlInfo.size()) {
            ::glDisable(GL_TEXTURE_2D);
            const GLfloat color_white[] = {1.0, 1.0, 1.0, 1.0};
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color_white);
        } else {
            // std::cout << "texture exist" << std::endl;
            const auto &mi = aMtlInfo[imi]; // miにマテリアル情報を保存
            ::glMaterialfv(GL_FRONT, GL_DIFFUSE, mi.Kd);
            if (::glIsTexture(mi.idTex_Kd)) {
                // std::cout << "Bind texture : " << mi.idTex_Kd << std::endl;
                ::glBindTexture(GL_TEXTURE_2D, mi.idTex_Kd);
                ::glEnable(GL_TEXTURE_2D);
            } else {
                ::glDisable(GL_TEXTURE_2D);
            }
        }

        ::glBegin(GL_TRIANGLES); // 描画ごとに頂点情報を送る
        for (unsigned int itri = mm.itri_start; itri < mm.itri_end; itri++) {
            // std::cout << "start: " << mm.itri_start << "end: " << mm.itri_end
            // << std::endl;
            ::glTexCoord2fv(aTex.data() + aTri_Tex[itri * 3 + 0] * 2);
            ::glNormal3fv(aNrm.data() + aTri_Nrm[itri * 3 + 0] * 3);
            ::glVertex3fv(aXYZ.data() + aTri_XYZ[itri * 3 + 0] * 3);

            ::glTexCoord2fv(aTex.data() + aTri_Tex[itri * 3 + 1] * 2);
            ::glNormal3fv(aNrm.data() + aTri_Nrm[itri * 3 + 1] * 3);
            ::glVertex3fv(aXYZ.data() + aTri_XYZ[itri * 3 + 1] * 3);

            ::glTexCoord2fv(aTex.data() + aTri_Tex[itri * 3 + 2] * 2);
            ::glNormal3fv(aNrm.data() + aTri_Nrm[itri * 3 + 2] * 3);
            ::glVertex3fv(aXYZ.data() + aTri_XYZ[itri * 3 + 2] * 3);
        }
        glEnd();
    }
}

// ------
// main
// ------
//
int main(void) {

    std::string path_obj =
        std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.obj";
    std::string path_mtl =
        std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug/data.mtl";
    std::string path_dir =
        std::string(PATH_ROOT_DIR) + "/test_data/objInfo/bug";

    std::cout << path_obj << " " << path_mtl << std::endl;

    std::vector<float> aXYZ;
    std::vector<float> aNrm;
    std::vector<float> aTex;
    std::vector<unsigned int> aTri_XYZ;
    std::vector<unsigned int> aTri_Tex;
    std::vector<unsigned int> aTri_Nrm;
    std::vector<CMaterialMap> aMtlMap;
    std::vector<CMaterialInfo> aMtlInfo;

    /* Read and check the obj data */
    read_obj(aXYZ, aNrm, aTex, aTri_XYZ, aTri_Tex, aTri_Nrm, aMtlMap, path_obj);
    std::cout << "obj file loading succeeded " << std::endl;

    std::cout << "number of vertices : " << aXYZ.size() / 3 << std::endl;
    std::cout << "number of faces    : " << aTri_XYZ.size() / 3 << std::endl;
    /* --------------------------- */

    // for (auto& v : aXYZ) { v *= 0.1; }
    for (auto &v : aNrm) {
        v *= -1;
    }

    /* Read and check the mtl data */
    read_mtl(aMtlInfo, path_mtl);
    std::cout << "mtl file loading succeeded " << std::endl;
    std::cout << std::endl;
    std::cout << "number of material refered from obj : " << aMtlMap.size()
              << std::endl;
    std::cout << "number of material defined in mtl : " << aMtlInfo.size()
              << std::endl;

    for (const auto &mi : aMtlInfo) {
        std::cout << std::endl;
        std::cout << "material information" << std::endl;
        std::cout << "   name : " << mi.name << std::endl;
        std::cout << "   Kd : " << mi.Kd[0] << " " << mi.Kd[1] << " "
                  << mi.Kd[2] << std::endl;
        std::cout << "   map_Kd : " << mi.map_Kd << std::endl;
        std::cout << std::endl;
    }
    /* -------------------------- */

    // GLFWを初期化し、ウィンドウを作成
    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // 新しいものを使う場合、majorを4とかにする。
    //  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    //  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    {
        std::string vrt_path =
            std::string(PATH_ROOT_DIR) + "/test_data/normalmap.vert";
        std::string frg_path =
            std::string(PATH_ROOT_DIR) + "/test_data/normalmap.frag";
        use_shader(vrt_path, frg_path);
    }

    {
        GLfloat lightpos[] = {1.0, 1.0, 1.0, 0.5};
        GLfloat lightcol[] = {1.0, 1.0, 1.0, 1.0};
        /* 光源の初期設定 */
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightpos);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
    }

    for (auto &mm : aMtlMap) {
        mm.iMaterialInfo = -1;

        for (unsigned int imi = 0; imi < aMtlInfo.size(); imi++) {
            if (mm.name != aMtlInfo[imi].name) {
                continue;
            }

            mm.iMaterialInfo = imi;
            CMaterialInfo &mi = aMtlInfo[imi];

            if (mi.map_Kd.empty())
                continue;
            if (mi.idTex_Kd != 0)
                continue;

            glGenTextures(1, &(mi.idTex_Kd));
            glBindTexture(GL_TEXTURE_2D, mi.idTex_Kd);

            int width, height;
            int channels;
            std::string img_path = path_dir + "/" + mi.map_Kd;

            stbi_set_flip_vertically_on_load(true);
            unsigned char *img =
                stbi_load(img_path.c_str(), &width, &height, &channels, 3);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, img);
            delete[] img;
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        }
    }

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);

        glClearColor(0.7f, 0.6f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // glRotatef((float) glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
        glRotatef(90, 0.0, 1.0, 0.0);
        glTranslatef(0.0f, -0.5f, 0.0f);

        // glUseProgram(shaderProgram);
        // この後のglBegin,
        // glEndには全てシェーダが適用される。0番を指定した場合は、デフォルトのシェーダを適用
        //    glBindVertexArray(VAO);
        //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Tri);

        display(aXYZ, aNrm, aTex, aTri_XYZ, aTri_Tex, aTri_Nrm, aMtlMap,
                aMtlInfo);

        GLubyte *pixel_data =
            (GLubyte *)malloc((width) * (height)*3 * (sizeof(GLubyte)));
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);
        if (!pixel_data)
            std::cout << "error pixel data " << std::endl;

        stbi_write_png(
            (std::string(PATH_ROOT_DIR) + "/05_ScreenShot/output/Screen_0.png")
                .c_str(),
            width, height, 3, pixel_data, 0);
        stbi_flip_vertically_on_write(1);
        free(pixel_data);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
