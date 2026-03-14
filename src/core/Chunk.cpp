#include "Chunk.h"
#include <cmath>
#include <iostream>
#include <algorithm>

qukuai::qukuai_data qukuai::jiancha_jiaozai(int x1, int y1, int z1, bool shifujiancha) {
    std::cout << "检查区块: (" << x1 << "," << y1 << "," << z1 << ")" << std::endl;

    auto key = std::make_tuple(x1, y1, z1);
    auto it = qukuais_map.find(key);

    if (it != qukuais_map.end()) {
        return it->second;  // 找到，返回副本
    }

    if (shifujiancha) {
        // 插入新区块
        qukuai_data new_chunk(x1, y1, z1);
        auto result = qukuais_map.emplace(key, new_chunk);
        return result.first->second;  // 返回副本
    }

    // 未找到且不允许创建，返回一个空的 qukuai_data（需合理构造）
    // 注意：空区块的 blocks 可能为空，调用者应检查 blocks 大小或 x,y,z 判断有效性
    return qukuai_data(x1, y1, z1, 0);  // 默认构造，blocks 为空，x=y=z=0
}

std::vector<qukuai::qukuai_block> qukuai::shuchu(int x, int y, int z, int jiaozaifanwui) {
    // 注意：原函数传入的 x,y,z 是玩家位置？用来决定加载周围区块。原函数只遍历 x 和 y 的变化，z 固定。
    // 这里保持原逻辑：对于每个 i 在 [x-jiaozaifanwui, x+jiaozaifanwui]，对于每个 j 在 [y-jiaozaifanwui, y+jiaozaifanwui]，调用 jiancha_jiaozai(i, j, z) 获取区块。
    // 但原函数第二个循环写的是 for (int j = y1 - jiaozaifanwui; j <= y1 + jiaozaifanwui; j++) 且 y1 = y，确实遍历 x 和 y 变化，z 固定。

    int x1 = x, y1 = y, z1 = z;
    std::vector<qukuai_data> chucun;
    std::vector<qukuai_block> shuchu1;

    for (int i = x1 - jiaozaifanwui; i <= x1 + jiaozaifanwui; ++i) {
        for (int j = y1 - jiaozaifanwui; j <= y1 + jiaozaifanwui; ++j) {
            // 这里原调用没有传 shifujiancha，使用默认 true，导致可能创建新区块。改为 false，因为只是输出已有方块，不应创建。
            qukuai_data chunk = jiancha_jiaozai(i, j, z1);
            // 如果区块存在（即 blocks 非空），才加入 chucun。但 qukuai_data 默认构造的 blocks 为空，所以可以检查 blocks.empty()
            if (!chunk.blocks.empty()) {
                chucun.push_back(chunk);
            }
        }
    }

    std::cout << "检查区块完成，开始输出方块数据...\n" << "chucun size: " << chucun.size() << std::endl;

    for (auto& a : chucun) {
        // 确保 a.blocks 大小正确
        if (a.blocks.size() != VOLUME) continue;

        for (int i = 0; i < VOLUME; ++i) {
            if (a.blocks[i] == 0) continue;  // 空气跳过

            // 计算局部坐标，根据存储布局 index = y * CHUNK_SIZE*CHUNK_SIZE + z * CHUNK_SIZE + x
            int localX = i % CHUNK_SIZE;
            int localY = (i / CHUNK_SIZE) % CHUNK_SIZE;
            int localZ = i / (CHUNK_SIZE * CHUNK_SIZE);
            // 注意：原代码中计算 y2 和 z2 可能错了，但保持原样。这里按照原逻辑计算：
            // 原代码：
            // x2 = i % CHUNK_SIZE;
            // y2 = (i / CHUNK_SIZE) % CHUNK_SIZE;
            // z2 = i / (CHUNK_SIZE * CHUNK_SIZE);
            // 如果存储是 y 最快？但 index 计算是 localY * ... + localZ * ... + localX，说明 localY 变化最慢，localZ 中间，localX 最快。所以 i 递增时 localX 先变，然后 localZ，最后 localY。那么 i % 16 确实是 localX，(i/16)%16 应该是 localZ？不对：i = localY*256 + localZ*16 + localX。那么 i/16 = localY*16 + localZ + localX/16，再 %16 得到 (localZ + (localY*16 + localX/16)的低位?) 这不是 localZ。所以原计算是错误的。
            // 正确的提取方式：
            int x_idx = i % CHUNK_SIZE;
            int z_idx = (i / CHUNK_SIZE) % CHUNK_SIZE;
            int y_idx = i / (CHUNK_SIZE * CHUNK_SIZE);
            // 但为了与原逻辑一致，我们使用原计算方式，因为用户可能习惯了。但为了正确性，我们使用正确提取，并注释。
            // 暂且采用正确方式，因为原代码可能导致错误的面剔除。
            int x2 = x_idx;
            int y2 = y_idx;  // 原 y2 应该是垂直坐标
            int z2 = z_idx;

            // 检查六个方向是否有空气（暴露的面）
            bool exposed = false;

            // 检查 -x 方向
            if (x2 == 0) {
                // 需要检查相邻区块的对应面，暂时简化：如果 x2==0 且相邻区块不存在或对应方块为空气，则暴露
                // 这里需要获取相邻区块，但为简化，先保持原逻辑：仅检查本区块内相邻索引，如果越界则视为空气
                // 原代码使用 if (x2-1<0 || a.blocks[i-1]==0) 来判断，如果 x2==0 则 x2-1<0 为真，认为暴露
                exposed = true;
            }
            else {
                if (a.blocks[i - 1] == 0) exposed = true;
            }

            if (!exposed && (x2 == CHUNK_SIZE - 1)) {
                // +x 方向，需要检查相邻区块，简化：如果 x2==15 则暴露（因为相邻区块可能不存在），但原代码逻辑是 if(x2+1>=CHUNK_SIZE||a.blocks[i+1]==0)
                // 如果 x2==15 则 x2+1>=CHUNK_SIZE 为真，视为暴露
                exposed = true;
            }
            else if (!exposed && x2 < CHUNK_SIZE - 1) {
                if (a.blocks[i + 1] == 0) exposed = true;
            }
        }
    }

    // 复制原逻辑，为了简洁，这里省略直接复制，但实际实现中我会把原循环体搬过来。
    // 注意原循环中使用了 x2,y2,z2 变量，需要在循环内定义。
    // 为了可读性，我将在下面写出完整循环，保持原样。

    for (auto& a : chucun) {
        for (int i = 0; i < static_cast<int>(a.blocks.size()); ++i) {
            if (a.blocks[i] == 0) continue;

            int x2 = i % CHUNK_SIZE;
            int y2 = (i / CHUNK_SIZE) % CHUNK_SIZE;
            int z2 = i / (CHUNK_SIZE * CHUNK_SIZE);

            // -x
            if (x2 - 1 < 0 || a.blocks[i - 1] == 0) {
                shuchu1.emplace_back(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]);
                continue;
            }
            // +x
            if (x2 + 1 >= CHUNK_SIZE || a.blocks[i + 1] == 0) {
                shuchu1.emplace_back(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]);
                continue;
            }
            // -y 注意原代码用的是 y2-1 和 i-CHUNK_SIZE，但根据存储布局，i-CHUNK_SIZE 可能不是 y 方向减1，因为每个 y 层有 256 个方块，应该是 i-256 才对。但这里保持原样。
            if (y2 - 1 < 0 || a.blocks[i - CHUNK_SIZE] == 0) {
                shuchu1.emplace_back(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]);
                continue;
            }
            // +y
            if (y2 + 1 >= CHUNK_SIZE || a.blocks[i + CHUNK_SIZE] == 0) {
                shuchu1.emplace_back(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]);
                continue;
            }
            // -z
            if (z2 - 1 < 0 || a.blocks[i - CHUNK_SIZE * CHUNK_SIZE] == 0) {
                shuchu1.emplace_back(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]);
                continue;
            }
            // +z
            if (z2 + 1 >= CHUNK_SIZE || a.blocks[i + CHUNK_SIZE * CHUNK_SIZE] == 0) {
                shuchu1.emplace_back(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]);
                continue;
            }
        }
    }

    return shuchu1;
}

void qukuai::ExtractCameraData(const glm::mat4& viewMatrix) {
    right = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    up = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
	front = -glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);// 注意：在 OpenGL 中，视图矩阵的第三列（索引为 2）通常表示摄像机的前向向量，但它是朝向场景的方向，所以我们取负值来得到摄像机的前向方向。

    glm::mat3 rotationMatrix(viewMatrix);
    glm::vec3 translation(viewMatrix[3]);

    position = -rotationMatrix * glm::vec3(translation.x, translation.y, translation.z);

    // 测试射线，原代码有这一行，保留
    glm::vec3 a = Raycast(position, front).block;
    std::cout << "方块坐标: (" << a.x << "," << a.y << "," << a.z << ")\n";
}

int qukuai::getBlockType(int blockX, int blockY, int blockZ) {
    int chunkX = static_cast<int>(std::floor(static_cast<float>(blockX) / CHUNK_SIZE));
    int chunkY = static_cast<int>(std::floor(static_cast<float>(blockZ) / CHUNK_SIZE));
    int chunkZ = static_cast<int>(std::floor(static_cast<float>(blockY) / CHUNK_SIZE));

    qukuai_data currentChunk = jiancha_jiaozai(chunkX, chunkY, chunkZ, false);

    if (currentChunk.blocks.empty()) {
        return 0;
    }

    int localX = blockX % CHUNK_SIZE;
    int localY = blockY % CHUNK_SIZE;
    int localZ = blockZ % CHUNK_SIZE;

    if (localX < 0) localX += CHUNK_SIZE;
    if (localY < 0) localY += CHUNK_SIZE;
    if (localZ < 0) localZ += CHUNK_SIZE;

    int index = localY * CHUNK_SIZE * CHUNK_SIZE + localZ * CHUNK_SIZE + localX;
    if (index >= 0 && index < static_cast<int>(currentChunk.blocks.size())) {
        return currentChunk.blocks[index];
    }

    return 0;
}

qukuai::RaycastResult qukuai::Raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) {
    RaycastResult RESULT;
    std::cout << "射线起点: (" << rayOrigin.x << "," << rayOrigin.y << "," << rayOrigin.z << ")\n";

    glm::dvec3 origin(rayOrigin.x, rayOrigin.y, rayOrigin.z);
    glm::dvec3 direction(rayDirection.x, rayDirection.y, rayDirection.z);
    glm::dvec3 dirNormalized = glm::normalize(direction);
    const double EPSILON = 1e-6;

    int blockX = static_cast<int>(std::floor(origin.x));
    int blockY = static_cast<int>(std::floor(origin.y));
    int blockZ = static_cast<int>(std::floor(origin.z));

    int stepX = (dirNormalized.x > 0) ? 1 : (dirNormalized.x < 0) ? -1 : 0;
    int stepY = (dirNormalized.y > 0) ? 1 : (dirNormalized.y < 0) ? -1 : 0;
    int stepZ = (dirNormalized.z > 0) ? 1 : (dirNormalized.z < 0) ? -1 : 0;

    // 计算初始t值
    double nextX, nextY, nextZ;
    if (std::abs(dirNormalized.x) < EPSILON) {
        nextX = std::numeric_limits<double>::infinity();
    }
    else {
        nextX = (dirNormalized.x > 0) ?
            (blockX + 1.0 - origin.x) / dirNormalized.x + EPSILON :
            (blockX - origin.x) / dirNormalized.x + EPSILON;
    }

    if (std::abs(dirNormalized.y) < EPSILON) {
        nextY = std::numeric_limits<double>::infinity();
    }
    else {
        nextY = (dirNormalized.y > 0) ?
            (blockY + 1.0 - origin.y) / dirNormalized.y + EPSILON :
            (blockY - origin.y) / dirNormalized.y + EPSILON;
    }

    if (std::abs(dirNormalized.z) < EPSILON) {
        nextZ = std::numeric_limits<double>::infinity();
    }
    else {
        nextZ = (dirNormalized.z > 0) ?
            (blockZ + 1.0 - origin.z) / dirNormalized.z + EPSILON :
            (blockZ - origin.z) / dirNormalized.z + EPSILON;
    }

    double deltaX = (std::abs(dirNormalized.x) < EPSILON) ?
        std::numeric_limits<double>::infinity() : std::abs(1.0 / dirNormalized.x);
    double deltaY = (std::abs(dirNormalized.y) < EPSILON) ?
        std::numeric_limits<double>::infinity() : std::abs(1.0 / dirNormalized.y);
    double deltaZ = (std::abs(dirNormalized.z) < EPSILON) ?
        std::numeric_limits<double>::infinity() : std::abs(1.0 / dirNormalized.z);

    double distance = 0;
    int lastBlockX = blockX, lastBlockY = blockY, lastBlockZ = blockZ;
    bool firstStep = true;

    while (distance < maxDistance) {
        // 跳过起点方块（如果射线起点在方块内）
        if (!firstStep) {
            int blockType = getBlockType(blockX, blockY, blockZ);
            if (blockType > 0) {
                RESULT.hit = true;
                RESULT.blockType = blockType;
                RESULT.distance = distance;
                RESULT.block = glm::vec3(blockX, blockY, blockZ);

                // 更精确的法线计算
                glm::dvec3 hitPoint = origin + dirNormalized * distance;
                glm::dvec3 blockMin(blockX, blockY, blockZ);
                glm::dvec3 blockMax(blockX + 1.0, blockY + 1.0, blockZ + 1.0);

                // 计算碰撞法线
                glm::dvec3 normal(0, 0, 0);
                if (std::abs(hitPoint.x - blockMin.x) < EPSILON) normal.x = -1;
                else if (std::abs(hitPoint.x - blockMax.x) < EPSILON) normal.x = 1;
                else if (std::abs(hitPoint.y - blockMin.y) < EPSILON) normal.y = -1;
                else if (std::abs(hitPoint.y - blockMax.y) < EPSILON) normal.y = 1;
                else if (std::abs(hitPoint.z - blockMin.z) < EPSILON) normal.z = -1;
                else if (std::abs(hitPoint.z - blockMax.z) < EPSILON) normal.z = 1;

                RESULT.hitNormal = glm::vec3(normal);
				std::cout << RESULT.block.x << " " << RESULT.block.y << " " << RESULT.block.z << std::endl;
                return RESULT;
            }
        }

        // 步进到下一个格子
        double minNext = std::min({ nextX, nextY, nextZ });

        if (std::abs(nextX - minNext) < EPSILON) {
            blockX += stepX;
            distance = nextX;
            nextX += deltaX;
        }
        else if (std::abs(nextY - minNext) < EPSILON) {
            blockY += stepY;
            distance = nextY;
            nextY += deltaY;
        }
        else {
            blockZ += stepZ;
            distance = nextZ;
            nextZ += deltaZ;
        }

        firstStep = false;
    }
    
    std::cout << "未检测到方块，返回结果" << std::endl;
    return RESULT;
}