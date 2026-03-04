#include"Chunk.h"
qukuai::qukuai_data qukuai::jiancha_jiaozai(int x1, int y1, int z1) {
	std::cout << "检查区块: (" << x1 << "," << y1 << "," << z1 << ")\n";
	if (x1 >= 0&& y1 >= 0&& z1 >= 0) {
		if(qukuais.qukuais_1.begin()==qukuais.qukuais_1.end()){
			qukuai_data new_qukuai(x1, y1, z1);
			qukuais.qukuais_1.push_back(new_qukuai);
			return new_qukuai;
		}
		for (auto it = qukuais.qukuais_1.begin(); it != qukuais.qukuais_1.end(); ++it) {
			if (it->x == x1 && it->y == y1 && it->z == z1) {
				return *it;
			}
			if (it->x > x1 || (it->x == x1 && it->y > y1) || (it->x == x1 && it->y == y1 && it->z > z1)) {
				qukuai_data new_qukuai(x1, y1, z1);
				qukuais.qukuais_1.insert(it, new_qukuai);
				return new_qukuai;
			}
		}
		qukuais.qukuais_1.push_back(qukuai_data(x1, y1, z1));
		return qukuais.qukuais_1.back();
	}
	if (x1 < 0&&y1 >= 0&&z1 >= 0) {
		if(qukuais.qukuais_2.begin()==qukuais.qukuais_2.end()){
			qukuai_data new_qukuai(x1, y1, z1);
			qukuais.qukuais_2.push_back(new_qukuai);
			return new_qukuai;
		}
		for (auto it = qukuais.qukuais_2.begin(); it != qukuais.qukuais_2.end(); ++it) {
			if (it->x == x1 && it->y == y1 && it->z == z1) {
				return *it;
			}
			if (it->x < x1 || (it->x == x1 && it->y > y1) || (it->x == x1 && it->y == y1 && it->z > z1)) {
				qukuai_data new_qukuai(x1, y1, z1);
				qukuais.qukuais_2.insert(it, new_qukuai);
				return new_qukuai;
			}
		}
		qukuais.qukuais_2.push_back(qukuai_data(x1, y1, z1));
		return qukuais.qukuais_2.back();
	}
	if (x1 < 0&&y1 < 0&& z1 >= 0) {
		if(qukuais.qukuais_3.begin()==qukuais.qukuais_3.end()){
			qukuai_data new_qukuai(x1, y1, z1);
			qukuais.qukuais_3.push_back(new_qukuai);
			return new_qukuai;
		}
		for (auto it = qukuais.qukuais_3.begin(); it != qukuais.qukuais_3.end(); ++it) {
			if (it->x == x1 && it->y == y1 && it->z == z1) {
				return *it;
			}
			if (it->x < x1 || (it->x == x1 && it->y < y1) || (it->x == x1 && it->y == y1 && it->z > z1)) {
				qukuai_data new_qukuai(x1, y1, z1);
				qukuais.qukuais_3.insert(it, new_qukuai);
				return new_qukuai;
			}
		}
		qukuais.qukuais_3.push_back(qukuai_data(x1, y1, z1));
		return qukuais.qukuais_3.back();
	}
	if (x1 >= 0&&y1 < 0&&z1 >= 0) {
		if(qukuais.qukuais_4.begin()==qukuais.qukuais_4.end()){
			qukuai_data new_qukuai(x1, y1, z1);
			qukuais.qukuais_4.push_back(new_qukuai);
			return new_qukuai;
		}
		for (auto it = qukuais.qukuais_4.begin(); it != qukuais.qukuais_4.end(); ++it) {
			if (it->x == x1 && it->y == y1 && it->z == z1) {
				return *it;
			}
			if (it->x > x1 || (it->x == x1 && it->y < y1) || (it->x == x1 && it->y == y1 && it->z > z1)) {
				qukuai_data new_qukuai(x1, y1, z1);
				qukuais.qukuais_4.insert(it, new_qukuai);
				return new_qukuai;
			}
		}
		qukuais.qukuais_4.push_back(qukuai_data(x1, y1, z1));
		return qukuais.qukuais_4.back();
	}
}
std::vector<qukuai::qukuai_block> qukuai::shuchu(int x, int y, int z, int jiaozaifanwui) {
	int x1 = x / CHUNK_SIZE; int y1 = y / CHUNK_SIZE; int z1 = z / CHUNK_SIZE;
	std::vector<qukuai_data> chucun;
	std::vector<qukuai_block> shuchu1;
	int x2 = 0, y2 = 0, z2 = 0;
	for (int i=x1-jiaozaifanwui;i<=x1+jiaozaifanwui;i++)
	{
		for (int j = y1 - jiaozaifanwui; j <= y1 + jiaozaifanwui; j++)
		{
			chucun.push_back(jiancha_jiaozai(i, j, z1));
		}
	}
	std::cout << "检查区块完成，开始输出方块数据...\n" << "chucun" << chucun.size() << std::endl;;
	for (auto& a: chucun) {
		for (int i = 0; i < a.blocks.size();i++) {
			x2 = i % CHUNK_SIZE; y2 = (i / CHUNK_SIZE) % CHUNK_SIZE; z2 = i / (CHUNK_SIZE * CHUNK_SIZE);
			if (a.blocks[i]) {
				if (x2 - 1 < 0||a.blocks[i-1]==0) {
					shuchu1.push_back(qukuai_block(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]));
					continue;
				}
				if(x2+1>=CHUNK_SIZE||a.blocks[i+1]==0){
					shuchu1.push_back(qukuai_block(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]));
					continue;
				}
				if(y2-1<0||a.blocks[i-CHUNK_SIZE]==0){
					shuchu1.push_back(qukuai_block(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]));
					continue;
				}
				if(y2+1>=CHUNK_SIZE||a.blocks[i+CHUNK_SIZE]==0){
					shuchu1.push_back(qukuai_block(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]));
					continue;
				}
				if(z2-1<0||a.blocks[i-CHUNK_SIZE*CHUNK_SIZE]==0){
					shuchu1.push_back(qukuai_block(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]));
					continue;
				}
				if(z2+1>=CHUNK_SIZE||a.blocks[i+CHUNK_SIZE*CHUNK_SIZE]==0){
					shuchu1.push_back(qukuai_block(a.x * CHUNK_SIZE + x2, a.y * CHUNK_SIZE + y2, a.z * CHUNK_SIZE + z2, a.blocks[i]));
					continue;
				}
			}

		}
	}
	return shuchu1;
}