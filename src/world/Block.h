#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cmath>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

const float chang = 1024.0f;
const float kuan = 1024.0f;
const float fangkuaichang = 32.0f;
const float fangkuaikuan = 32.0f;

// 正方体顶点数据（带纹理坐标）
extern std::vector<Vertex> cubeVertices;

void Select_Material();