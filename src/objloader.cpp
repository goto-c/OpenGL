#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include "objloader.h"

/* --------------- ***.obj loader --------------*/

std::vector<unsigned int> Object::strip_slash(std::string s) {

    std::vector<unsigned int> vtn_values;
    vtn_values.clear();

    std::vector<int> digits_v;
    std::vector<int> digits_t;
    std::vector<int> digits_n;
    digits_v.clear();
    digits_n.clear();
    digits_t.clear();
    int v_value = 0;
    int t_value = 0;
    int n_value = 0;
    unsigned int idx = 0;

    // "***/***/***"の状態の文字列から、v,t,nの各インデックス文字列に分離する
    while (s[idx] != '/') {
        digits_v.push_back(s[idx]);
        idx++;
    }
    idx++;
    while (s[idx] != '/' && idx < s.size()) {
        digits_t.push_back(s[idx]);
        //        std::cout << "idx : " << idx << ", " << "t <- s[idx] : " <<
        //        s[idx] << std::endl;
        idx++;
    }
    idx++;
    while (idx < s.size()) {
        digits_n.push_back(s[idx]);
        idx++;
    }
    // インデックスの文字列を10進数に変換
    for (unsigned int i = 0; i < digits_v.size(); i++) {
        v_value += std::pow(10, i) * (digits_v[digits_v.size() - 1 - i] - '0');
    }
    for (unsigned int i = 0; i < digits_t.size(); i++) {
        t_value += std::pow(10, i) * (digits_t[digits_t.size() - 1 - i] - '0');
    }
    for (unsigned int i = 0; i < digits_n.size(); i++) {
        n_value += std::pow(10, i) * (digits_n[digits_n.size() - 1 - i] - '0');
    }
    vtn_values.push_back(v_value);
    vtn_values.push_back(t_value);
    vtn_values.push_back(n_value);

    return vtn_values;
}

int Object::load_obj(std::string obj_file_path) {
    m_aMtlMap.clear();

    std::ifstream fin;
    fin.open(obj_file_path);
    if (fin.fail()) {
        throw std::runtime_error("obj file open failed");
    }

    m_aXYZ.clear();
    m_aNrm.clear();
    m_aTex.clear();
    m_aTri_XYZ.clear();
    m_aTri_Tex.clear();
    m_aTri_Nrm.clear();

    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    int icount_tri = 0;

    while (!fin.eof()) {
        fin.getline(buff, BUFF_SIZE);

        // 先頭が'v'の場合
        if (buff[0] == 'v' && buff[1] == ' ') {
            char str[256];
            float x, y, z;
            sscanf(buff, "%s %f %f %f", str, &x, &y, &z);
            m_aXYZ.push_back(x);
            m_aXYZ.push_back(y);
            m_aXYZ.push_back(z);
        }
        // 先頭が'vn'の場合
        if (buff[0] == 'v' && buff[1] == 'n') {
            char str[256];
            float x, y, z;
            sscanf(buff, "%s %f %f %f", str, &x, &y, &z);
            m_aNrm.push_back(x);
            m_aNrm.push_back(y);
            m_aNrm.push_back(z);
        }
        // 先頭が'vt'の場合
        if (buff[0] == 'v' && buff[1] == 't') {
            char str[256];
            float s, t;
            sscanf(buff, "%s %f %f", str, &s, &t);
            m_aTex.push_back(s);
            m_aTex.push_back(t);
        }
        // 先頭が'us'の場合
        if (buff[0] == 'u' && buff[1] == 's') {
            char str[256], mtl_name[1024];
            sscanf(buff, "%s %s", str, mtl_name);
            {
                CMaterialMap mm;
                mm.itri_start = icount_tri;
                mm.name = std::string(mtl_name);
                m_aMtlMap.push_back(mm);
                std::cout << "usemtl: " << mm.name << std::endl;
            }
        }
        // 先頭が'f'の場合
        if (buff[0] == 'f') {
            char str[256];
            char i0[256], i1[256], i2[256], i3[256] = {'\0'};
            sscanf(buff, "%s %s %s %s %s", str, i0, i1, i2, i3);
            std::vector<unsigned int> vertex0, vertex1, vertex2;
            vertex0.clear();
            vertex1.clear();
            vertex2.clear();
            vertex0 = strip_slash(i0);
            vertex1 = strip_slash(i1);
            vertex2 = strip_slash(i2);
            m_aTri_XYZ.push_back(vertex0[0] - 1);
            m_aTri_XYZ.push_back(vertex1[0] - 1);
            m_aTri_XYZ.push_back(vertex2[0] - 1);
            m_aTri_Tex.push_back(vertex0[1] - 1);
            m_aTri_Tex.push_back(vertex1[1] - 1);
            m_aTri_Tex.push_back(vertex2[1] - 1);
            m_aTri_Nrm.push_back(vertex0[2] - 1);
            m_aTri_Nrm.push_back(vertex1[2] - 1);
            m_aTri_Nrm.push_back(vertex2[2] - 1);
            // 四角形メッシュを加える場合、三角形に分割
            if (i3[0] != '\0') {
                std::vector<unsigned int> vertex3;
                vertex3.clear();
                vertex3 = strip_slash(i3);
                m_aTri_XYZ.push_back(vertex0[0] - 1);
                m_aTri_XYZ.push_back(vertex2[0] - 1);
                m_aTri_XYZ.push_back(vertex3[0] - 1);
                m_aTri_Tex.push_back(vertex0[1] - 1);
                m_aTri_Tex.push_back(vertex2[1] - 1);
                m_aTri_Tex.push_back(vertex3[1] - 1);
                m_aTri_Nrm.push_back(vertex0[2] - 1);
                m_aTri_Nrm.push_back(vertex2[2] - 1);
                m_aTri_Nrm.push_back(vertex3[2] - 1);
                icount_tri++;
            }
            icount_tri++;
        }
    }
    fin.close();

    if (m_aNrm.size() == 0) {
        m_aTri_Nrm.clear();
    }

    for (unsigned int imm = 0; imm < m_aMtlMap.size() - 1; imm++) {
        m_aMtlMap[imm].itri_end = m_aMtlMap[imm + 1].itri_start;
    }
    m_aMtlMap[m_aMtlMap.size() - 1].itri_end = m_aTri_XYZ.size() / 3;

    return 1;
}

void read_flag(std::vector<unsigned int> &flag, const std::string &file_flag) {
    flag.clear();
    std::cout << "initial flag size: " << flag.size() << std::endl;

    std::ifstream fin;
    fin.open(file_flag);
    if (fin.fail()) {
        std::cout << "flag file open failed" << std::endl;
    } else {
        std::cout << "flag file: " << file_flag << std::endl;
    }

    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];
    while (!fin.eof()) {
        fin.getline(buff, BUFF_SIZE);

        int i = 0;
        while (buff[i] != ' ') {
            i++;
        }
        i++;
        flag.push_back(buff[i] - '0');
    }
    fin.close();
}

/* ----------------- ***.mtl loader ---------------*/

int Object::load_mtl(std::string mtl_file_path) {
    // void read_mtl(std::vector<CMaterialInfo> &aMtlInfo, const std::string
    // &file_mtl) {
    std::ifstream fin(mtl_file_path);
    if (fin.fail()) {
        throw std::runtime_error("mtl file open failed");
    }

    const int BUFF_SIZE = 256;
    char buff[BUFF_SIZE];

    m_aMtlInfo.clear();
    CMaterialInfo mi;

    while (!fin.eof()) {
        fin.getline(buff, BUFF_SIZE);

        // コメント行は飛ばす
        if (buff[0] == '#')
            continue;

        // regigter material
        if (strncmp(buff, "newmtl", 6) == 0) {
            // aMtlInfo.push_back(mi);
            mi.map_d.clear();
            mi.map_Ka.clear();
            mi.map_Kd.clear();
            char str[256], name[1024];
            sscanf(buff, "%s %s", str, name);
            std::cout << "newmtl: " << name << std::endl;
            mi.name = name;
        }
        // 環境光・拡散光・鏡面光のRGB値を構造体のメンバに追加
        if (buff[1] == 'K' && buff[2] == 'a') {
            char str[256];
            float r, g, b;
            sscanf(buff, "%s %f %f %f", str, &r, &g, &b);
            mi.Ka[0] = r;
            mi.Ka[1] = g;
            mi.Ka[2] = b;
            mi.Ka[3] = 1.f;
        }
        if (buff[1] == 'K' && buff[2] == 'd') {
            char str[256];
            float r, g, b;
            sscanf(buff, "%s %f %f %f", str, &r, &g, &b);
            mi.Kd[0] = r;
            mi.Kd[1] = g;
            mi.Kd[2] = b;
            mi.Kd[3] = 1.f;
            std::cout << "Kd _: " << mi.Kd[0] << ", " << mi.Kd[1] << ", "
                      << mi.Kd[2] << std::endl;
        }
        if (buff[1] == 'K' && buff[2] == 's') {
            char str[256];
            float r, g, b;
            sscanf(buff, "%s %f %f %f", str, &r, &g, &b);
            mi.Ks[0] = r;
            mi.Ks[1] = g;
            mi.Ks[2] = b;
            mi.Ks[3] = 1.f;
        }
        // テクスチャ画像ファイル名を構造体メンバに追加
        /*if (buff[1]=='m' && buff[5]=='K' && buff[6]=='a'){
            char str[256], map[1024];
            sscanf(buff, "%s %s", str, map);
            mi.map_Ka = map;
        }*/
        if (buff[1] == 'm' && buff[5] == 'K' && buff[6] == 'd') {
            char str[256], map[1024];
            sscanf(buff, "%s %s", str, map);
            mi.map_Kd = map;
            std::cout << "map_Kd _: " << mi.map_Kd << std::endl;

            m_aMtlInfo.push_back(mi);
        }
        /*if (buff[1]=='m' && buff[5]=='d'){
            char str[256], map[1024];
            sscanf(buff, "%s %s", str, map);
            mi.map_d = map;
        }*/
    }
    /*for (int i=0; i<3; i++){
        std::cout << "aMtlInfo.size(): " << aMat.size() << std::endl;
        std::cout << "aMtlInfo.Kd: " << aMat[i].Kd[0] << std::endl;
        std::cout << "aMtlInfo.map_Kd: " << aMat[i].map_Kd << std::endl;
        std::cout << std::endl;
    }*/
    return 0;
}
