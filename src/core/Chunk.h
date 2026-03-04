#pragma once
#include<iostream>
#include<vector>
#include<list>

class qukuai {
	private:
		static constexpr int CHUNK_SIZE = 16; // 区块尺寸 16x16x16
		static constexpr int VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
		struct qukuai_data {
			int x, y, z; // 区块坐标
			std::vector<int> blocks; // 存储区块内的方块数据
			qukuai_data(int x, int y, int z) : x(x), y(y), z(z), blocks(VOLUME, 163) {} // 初始化区块数据
		};
		struct qukuai_zonghe {
			std::list<qukuai_data> qukuais_1;//第一象限
			std::list<qukuai_data> qukuais_2;//第二象限
			std::list<qukuai_data> qukuais_3;//第三象限
			std::list<qukuai_data> qukuais_4;//第四象限
		};
		qukuai_zonghe qukuais;
public:
	qukuai(){}
	struct qukuai_block {
		int x, y, z; // 方块坐标
		int block_type; // 方块类型
		qukuai_block(int x, int y, int z, int block_type) : x(x), y(y), z(z), block_type(block_type) {}
	};
	qukuai_data jiancha_jiaozai(int x, int y, int z);
	std::vector<qukuai_block> shuchu(int x, int y, int z, int jiaozaifanwui);


};