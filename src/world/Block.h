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

const int chang = 16;
const int kuan = 16;

// 正方体顶点数据（带纹理坐标）
extern std::vector<Vertex> cubeVertices;

void Select_Material(int a1);