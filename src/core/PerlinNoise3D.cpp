#include"PerlinNoise3D.h"
PerlinNoise3D::PerlinNoise3D(unsigned int seed) {
    // 初始化排列表（permutation table）
    // 标准Perlin噪声使用256个值的排列，通过重复两次来避免数组越界
    p.resize(512);

    // 先用种子生成0-255的随机排列
    std::vector<int> perm(256);
    for (int i = 0; i < 256; ++i) {
        perm[i] = i;
    }
    // 使用种子打乱顺序
    if (seed != 0) {
        std::mt19937 rng(seed);
        std::shuffle(perm.begin(), perm.end(), rng);
    }
    // 复制一份到p中，使索引可以用模256快速访问
    for (int i = 0; i < 512; ++i) {
        p[i] = perm[i % 256];
    }
}
float PerlinNoise3D::noise3D(float x, float y, float z) const {
    // 1. 找到包含该点的立方体的整数顶点坐标
    int xi = static_cast<int>(std::floor(x)) & 255;
    int yi = static_cast<int>(std::floor(y)) & 255;
    int zi = static_cast<int>(std::floor(z)) & 255;

    // 2. 计算该点在立方体内部的相对坐标（范围[0,1]）
    float xf = x - std::floor(x);
    float yf = y - std::floor(y);
    float zf = z - std::floor(z);

    // 3. 使用缓动曲线（fade）将相对坐标映射到更平滑的插值权重
    //    标准缓动函数为 6t^5 - 15t^4 + 10t^3，导数在0和1处为零，保证平滑过渡
    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);

    // 4. 获取立方体8个顶点的梯度索引（通过排列表）
    //    索引计算方法：p[xi + p[yi + p[zi]]] 等，利用排列表将整数坐标映射到梯度索引
    int aaa = p[p[p[xi] + yi] + zi];
    int aba = p[p[p[xi] + yi + 1] + zi];
    int aab = p[p[p[xi] + yi] + zi + 1];
    int abb = p[p[p[xi] + yi + 1] + zi + 1];
    int baa = p[p[p[xi + 1] + yi] + zi];
    int bba = p[p[p[xi + 1] + yi + 1] + zi];
    int bab = p[p[p[xi + 1] + yi] + zi + 1];
    int bbb = p[p[p[xi + 1] + yi + 1] + zi + 1];

    // 5. 计算8个顶点处的梯度贡献（点积）
    //    梯度向量从预定义的12个方向中选择（标准Perlin噪声）
    float x1, x2, y1, y2;

    // 底层四个顶点（z = zf）
    x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
    x2 = lerp(grad(aba, xf, yf - 1, zf), grad(bba, xf - 1, yf - 1, zf), u);
    y1 = lerp(x1, x2, v);

    // 顶层四个顶点（z = zf-1）
    x1 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
    x2 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);
    y2 = lerp(x1, x2, v);

    // 6. 最终插值得到结果
    //    结果范围理论上在[-1,1]之间，但实际可能略超出，但非常接近
    float result = lerp(y1, y2, w);

    // 可选：限制输出范围到[-1,1]（保证稳定）
    // return std::max(-1.0f, std::min(1.0f, result));
    return result;
}
float PerlinNoise3D::fbm3D(float x, float y, float z, int octaves, float persistence, float lacunarity) const {
    float value = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxAmp = 0.0f;  // 用于归一化（可选）

    for (int i = 0; i < octaves; ++i) {
        value += amplitude * noise3D(x * frequency, y * frequency, z * frequency);
        maxAmp += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    // 归一化到[-1,1]范围，避免振幅累积导致超出范围
    // 注意：由于噪声本身范围可能略超1，归一化后仍然可能稍微超出，但可接受
    return value / maxAmp;
}
