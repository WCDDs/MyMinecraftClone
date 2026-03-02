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
extern std::vector<glm::vec2> textureCoords;

void Select_Material();

glm::vec2 getTextureCoords(int index);// 根据索引获取对应的纹理坐标