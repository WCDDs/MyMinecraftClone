#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "../core/Chunk.h"

struct Vertex {
	glm::vec3 Position;// 顶点位置
	glm::vec3 Normal;// 顶点法线
	glm::vec2 TexCoords;// 顶点纹理坐标
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

struct InstanceData {
	glm::mat4 modelMatrix;
	glm::vec2 textureOffset; // 纹理坐标偏移
};

extern std::vector<InstanceData> instances;
extern int shilihuashulian;
extern qukuai sc;
void generateInstances(int x, int y, int z, int qukuaidaxiao); // 生成实例数据