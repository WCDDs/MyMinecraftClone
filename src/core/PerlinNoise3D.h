#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

/**
 * 3D Perlin噪声生成器
 *
 * Perlin噪声是一种梯度噪声，通过在一个规则网格上随机生成梯度向量，
 * 然后对每个点的相邻网格点贡献进行插值，得到平滑的伪随机值。
 * 常用于程序化地形、纹理、动画等。
 *
 * 此类实现标准的3D Perlin噪声，并提供了分形布朗运动(FBM)接口，
 * 用于生成更自然的多尺度细节。
 */
class PerlinNoise3D {
public:
    /**
     * 构造函数
     * @param seed 随机种子，确保相同种子产生相同的噪声场
     */
    PerlinNoise3D(unsigned int seed = 0);

    /**
     * 计算3D Perlin噪声值
     * @param x, y, z 输入坐标（浮点数）
     * @return 噪声值，范围大约在[-1, 1]之间
     */
    float noise3D(float x, float y, float z) const;

    /**
     * 分形布朗运动（FBM）
     * 通过叠加不同频率和振幅的噪声，产生更自然、多尺度的地形
     *
     * @param x, y, z 输入坐标
     * @param octaves 叠加层数（通常3-6层）
     * @param persistence 每层振幅衰减系数（通常0.5）
     * @param lacunarity 每层频率增加系数（通常2.0）
     * @return 叠加后的噪声值，范围大致在[-amplitude_sum, amplitude_sum]
     */
    float fbm3D(float x, float y, float z, int octaves, float persistence, float lacunarity) const;
private:
    std::vector<int> p;  // 排列表（512长度）

    // 缓动曲线：6t^5 - 15t^4 + 10t^3
    static float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    // 线性插值
    static float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    // 梯度函数：根据hash值返回对应的梯度向量，并计算该向量与点(dirX, dirY, dirZ)的点积
    // 标准Perlin噪声使用12个方向的梯度（中心指向立方体边中点）
    static float grad(int hash, float x, float y, float z) {
        int h = hash & 15;          // 取低4位，范围0-15
        float u = h < 8 ? x : y;    // 根据hash选择使用x或y作为第一个分量
        float v = h < 4 ? y : (h == 12 || h == 14 ? x : z); // 选择第二个分量
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        // 实际上返回的是梯度与方向向量的点积，因为梯度向量的分量是±1或0，简化了计算
    }
};