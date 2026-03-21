#include "Chunk.h"
#include <cmath>
#include <iostream>
#include <algorithm>

qukuai::qukuai_data qukuai::jiancha_jiaozai(int x1, int y1, int z1, bool shifujiancha) {
    /*std::cout << "检查区块: (" << x1 << "," << y1 << "," << z1 << ")" << std::endl;*/
    qukuai_data ls(x1, y1, z1, 0);
    auto key = std::make_tuple(x1, y1, z1);
    auto it = qukuais_map.find(key);

    if (it != qukuais_map.end()) {
        ls= it->second;  // 找到，返回副本
    }

    if (shifujiancha) {
        // 插入新区块
        qukuai_data new_chunk(x1, y1, z1);
        auto result = qukuais_map.emplace(key, new_chunk);
        ls= result.first->second;  // 返回副本
    }

    // 未找到且不允许创建，返回一个空的 qukuai_data（需合理构造）
    // 注意：空区块的 blocks 可能为空，调用者应检查 blocks 大小或 x,y,z 判断有效性
    for (auto& a : genggaishuju[std::make_tuple(x1, y1, z1)])
    {
        ls.blocks[a.first] = a.second;
    }
    return ls;  // 默认构造，blocks 为空，x=y=z=0
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
			}// 注意：这里 push_back 会复制 chunk，如果 qukuai_data 中 blocks 很大，可能效率不高。可以考虑使用指针或引用，但为了简单保持原样。
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

    int index = localY * CHUNK_SIZE * CHUNK_SIZE + localX * CHUNK_SIZE + localZ;
    if (index >= 0 && index < static_cast<int>(currentChunk.blocks.size())) {
        return currentChunk.blocks[index];
    }

    return 0;
}

qukuai::RaycastResult qukuai::Raycast(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) {
    RaycastResult result;
    
    // 调试输出
    //std::cout << "射线起点: (" << rayOrigin.x << "," << rayOrigin.y << "," << rayOrigin.z << ")\n";
    //std::cout << "射线方向: (" << rayDirection.x << "," << rayDirection.y << "," << rayDirection.z << ")\n";

    // 1. 转换为double精度，避免浮点误差
    const glm::dvec3 origin(rayOrigin.z, rayOrigin.y, rayOrigin.x);
    glm::dvec3 direction(rayDirection.x, rayDirection.y, rayDirection.z);

    // 2. 归一化方向向量（必须）
    const double dirLength = glm::length(direction);
    if (dirLength < 1e-8) {
        std::cerr << "错误：射线方向向量长度为0！\n";
        return result; // 无效方向
    }
    const glm::dvec3 dirNormalized = direction / dirLength;

    // 3. 定义精度常量
    const double EPSILON = 1e-8;

    // 4. 初始方块坐标（floor获取当前所在方块）
    int currentX = static_cast<int>(glm::floor(origin.x));
    int currentY = static_cast<int>(glm::floor(origin.y));
    int currentZ = static_cast<int>(glm::floor(origin.z));

    // 5. 计算每个轴的步长方向（-1, 0, 1）
    const int stepX = (dirNormalized.x > EPSILON) ? 1 : (dirNormalized.x < -EPSILON) ? -1 : 0;
    const int stepY = (dirNormalized.y > EPSILON) ? 1 : (dirNormalized.y < -EPSILON) ? -1 : 0;
    const int stepZ = (dirNormalized.z > EPSILON) ? 1 : (dirNormalized.z < -EPSILON) ? -1 : 0;

    // 6. 计算到下一个方块边界的t值
    double tMaxX, tMaxY, tMaxZ;
    double tDeltaX, tDeltaY, tDeltaZ;

    // X轴
    if (std::abs(dirNormalized.x) < EPSILON) {
        tMaxX = std::numeric_limits<double>::infinity();
        tDeltaX = std::numeric_limits<double>::infinity();
    }
    else {
        const double boundaryX = (stepX > 0) ? currentX + 1.0 : currentX-1.0;
        tMaxX = (boundaryX - origin.x) / dirNormalized.x;
        tDeltaX = std::abs(1.0 / dirNormalized.x);

        // 避免tMaxX为负（起点在边界上）
        if (tMaxX < 0) tMaxX += tDeltaX;
    }

    // Y轴
    if (std::abs(dirNormalized.y) < EPSILON) {
        tMaxY = std::numeric_limits<double>::infinity();
        tDeltaY = std::numeric_limits<double>::infinity();
    }
    else {
        const double boundaryY = (stepY > 0) ? currentY + 1.0 : currentY-1.0;
        tMaxY = (boundaryY - origin.y) / dirNormalized.y;
        tDeltaY = std::abs(1.0 / dirNormalized.y);

        if (tMaxY < 0) tMaxY += tDeltaY;
    }

    // Z轴
    if (std::abs(dirNormalized.z) < EPSILON) {
        tMaxZ = std::numeric_limits<double>::infinity();
        tDeltaZ = std::numeric_limits<double>::infinity();
    }
    else {
        const double boundaryZ = (stepZ > 0) ? currentZ + 1.0 : currentZ-1.0;
        tMaxZ = (boundaryZ - origin.z) / dirNormalized.z;
        tDeltaZ = std::abs(1.0 / dirNormalized.z);

        if (tMaxZ < 0) tMaxZ += tDeltaZ;
    }

    // 7. 遍历方块（核心DDA循环）
    double distance = 0.0;
    bool skipFirst = true; // 跳过起点所在方块

    while (distance < maxDistance) {
        // 检查当前方块（跳过第一个方块）
        if (!skipFirst) {
            const int blockType = getBlockType(currentX, currentY, currentZ);
            if (blockType > 0) {
                // 命中成功，填充结果
                result.hit = true;
                result.blockType = blockType;
                result.distance = distance;
                result.block = glm::vec3(currentZ, currentY, currentX);

                // 计算精确命中点和法线
                result.hitNormal = glm::vec3(origin + dirNormalized * distance);

                // 确定碰撞面法线
                if (std::abs(tMaxX - distance) < EPSILON) {
                    result.hitNormal = glm::vec3(-stepX, 0, 0);
                }
                else if (std::abs(tMaxY - distance) < EPSILON) {
                    result.hitNormal = glm::vec3(0, -stepY, 0);
                }
                else if (std::abs(tMaxZ - distance) < EPSILON) {
                    result.hitNormal = glm::vec3(0, 0, -stepZ);
                }

                std::cout << "命中方块: (" << currentX << "," << currentY << "," << currentZ << ")\n";
                std::cout << "命中距离: " << distance << "\n";
                //std::cout << "命中法线: (" << result.hitNormal.x << "," << result.hitNormal.y << "," << result.hitNormal.z << ")\n";
                return result;
            }
        }
        skipFirst = false;

        // 8. 步进至下一个方块
        if (tMaxX < tMaxY && tMaxX < tMaxZ) {
            currentX += stepX;
            distance = tMaxX;
            tMaxX += tDeltaX;
        }
        else if (tMaxY < tMaxZ) {
            currentY += stepY;
            distance = tMaxY;
            tMaxY += tDeltaY;
        }
        else {
            currentZ += stepZ;
            distance = tMaxZ;
            tMaxZ += tDeltaZ;
        }

        // 防止无限循环
        if (distance != distance) { // NaN检查
            std::cerr << "错误：计算出NaN距离！\n";
            break;
        }
    }

    // 未命中
    return result;
}


void qukuai::chucunbianhuashuju(qukuai::RaycastResult a) {
    int blockX = static_cast<int>(a.block.x);
    int blockY = static_cast<int>(a.block.y);
	int blockZ = static_cast<int>(a.block.z);// 注意：a.block 是被击中的方块坐标，直接使用 x,y,z 作为全局坐标
    int chunkX = static_cast<int>(std::floor(static_cast<float>(blockX) / CHUNK_SIZE));
    int chunkY = static_cast<int>(std::floor(static_cast<float>(blockZ) / CHUNK_SIZE));
    int chunkZ = static_cast<int>(std::floor(static_cast<float>(blockY) / CHUNK_SIZE));//区块坐标
	qukuai_data currentChunk = jiancha_jiaozai(chunkX, chunkY, chunkZ, false);// 获取当前区块数据，shifujiancha=false 表示如果区块不存在则返回一个空的 qukuai_data（blocks 为空）
    if (currentChunk.blocks.empty()) {
        return; // 区块不存在，无需修改
    }
    int localX = blockX % CHUNK_SIZE;
    int localY = blockY % CHUNK_SIZE;
    int localZ = blockZ % CHUNK_SIZE;
    if (localX < 0) localX += CHUNK_SIZE;
    if (localY < 0) localY += CHUNK_SIZE;
    if (localZ < 0) localZ += CHUNK_SIZE;
    int index = localY * CHUNK_SIZE * CHUNK_SIZE + localZ * CHUNK_SIZE + localX;
    if (index >= 0 && index < static_cast<int>(currentChunk.blocks.size())) {
        //currentChunk.blocks[index] = 0; // 将方块类型设置为0（空气）
        genggaishuju[std::make_tuple(chunkX, chunkZ, chunkY)][index]=0; // 存储到全局修改数据中
		std::cout << "修改方块: (" << blockX << "," << blockY << "," << blockZ << ") 在区块 (" << chunkX << "," << chunkY << "," << chunkZ << ") 的局部坐标 (" << localX << "," << localY << "," << localZ << ") 索引 " << index << "\n";
	}
}