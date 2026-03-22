#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include"PerlinNoise3D.h"

class qukuai {
private:
    static constexpr int CHUNK_SIZE = 16;
    static constexpr int VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
    static constexpr float maxDistance = 16.0f;
    static constexpr int chushifangkuaileixing = 36;

    unsigned int a1 = 0;
    PerlinNoise3D noise;

    // 参数设置
    const int width = CHUNK_SIZE;
    const int depth = CHUNK_SIZE;
    const float scale = 0.05f;   // 频率缩放
    const int octaves = 4;
    const float persistence = 0.5f;
    const float lacunarity = 2.0f;

    glm::vec3 position = glm::vec3(0, 0, 0), front = glm::vec3(0, 0, 0), up = glm::vec3(0, 0, 0), right = glm::vec3(0, 0, 0);


    struct qukuai_data {
        int x, y, z;
        std::vector<int> blocks;
        qukuai_data(int x_, int y_, int z_, int type_ = chushifangkuaileixing)
            : x(x_), y(y_), z(z_), blocks(VOLUME, 0) {

        }
        // 添加默认构造函数以便 map 的某些操作（如 operator[]）
        qukuai_data() : x(0), y(0), z(0), blocks(VOLUME, 0) { if (x == 0 && y == 0 && z == 0)blocks[240] = 35; }
    };

    void generateChunk(qukuai_data& chunk);

    struct RaycastResult {
        bool hit = false;
        int blockType = 0;
        float distance = 0;
		glm::vec3 block;// 被击中的方块坐标
		glm::vec3 hitNormal;// 碰撞法线
    };
    struct qukuai_block {
        int x, y, z;
        int block_type;
        qukuai_block(int x_, int y_, int z_, int bt) : x(x_), y(y_), z(z_), block_type(bt) {}
    };

    // 使用 map 存储所有区块，键为 (x,y,z) 三元组
	std::map<std::tuple<int, int, int>, qukuai_data> qukuais_map;// 存储所有区块数据
    std::map<std::tuple<int, int, int>, std::map<int, int>>genggaishuju;// 存储被修改过的方块数据
	std::vector<qukuai_block> shuchu1;// 存储输出的方块数据

public:
    qukuai() :noise(a1){}
    int fanhuikejianfangkuai(int x, int y, int z);
    qukuai_data jiancha_jiaozai(int x, int y, int z, bool shifujiancha = true);
    std::vector<qukuai_block> shuchu(int x, int y, int z, int jiaozaifanwui);
    void ExtractCameraData(const glm::mat4& viewMatrix);
    RaycastResult Raycast(const glm::vec3& rayOrigin1, const glm::vec3& rayDirection);
    int getBlockType(int blockX, int blockY, int blockZ);
    void chucunbianhuashuju(RaycastResult a);
};
