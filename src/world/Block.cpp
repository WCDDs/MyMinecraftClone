#include"Block.h"

float a = 0, b = 0,
c = 1, d = 1;

float e = 1.0f / chang, f = 1.0f / kuan;
float tmdwc = 1.0;

std::vector<Vertex> cubeVertices = {
    // 前面 (V坐标翻转：0变1，1变0)
    {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

    // 后面 (V坐标翻转)
    {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},

    // 左面 (V坐标翻转)
    {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

    // 右面 (V坐标翻转)
    {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

    // 上面 (V坐标翻转)
    {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

    // 下面 (V坐标翻转)
    {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)}
};

void Select_Material()
{
    float a1 = fangkuaichang / chang;
	float a2 = fangkuaikuan / kuan;
    for (int i = 0; i < cubeVertices.size(); i++)
    {
        cubeVertices[i].TexCoords.x *= a1;
		cubeVertices[i].TexCoords.y *= a2;
    }
}

// 精灵表纹理坐标数组 (每个元素对应精灵表上的一个32x32区域)
// 坐标格式: {x, y} 位置（像素）
std::vector<glm::vec2> textureCoords = {
    // 索引0-9: 流体动画区域 (0-31行)
    {0, 0},      // 0: 熔岩流 (lava_flow.png) - 64x1024
    {64, 0},     // 1: 火焰层0 (fire_layer_0.png) - 32x1024
    {96, 0},     // 2: 火焰层1 (fire_layer_1.png) - 32x1024
    {128, 0},    // 3: 传送门 (portal.png) - 32x1024
    {160, 0},    // 4: 静态水 (water_still.png) - 32x1024
    {192, 0},    // 5: 静态熔岩 (lava_still.png) - 32x640

    // 索引6-14: 铁砧区域 (行20-23)
    {192, 640},  // 6: 铁砧底座 (anvil_base.png)
    {192, 672},  // 7: 铁砧顶部(0损坏) (anvil_top_damaged_0.png)
    {192, 704},  // 8: 铁砧顶部(1损坏) (anvil_top_damaged_1.png)
    {192, 736},  // 9: 铁砧顶部(2损坏) (anvil_top_damaged_2.png)

    // 索引10-12: 功能方块区域 (行24-30)
    {192, 768},  // 10: 信标 (beacon.png)
    {192, 992},  // 11: 基岩 (bedrock.png)
    {224, 0},    // 12: 书架 (bookshelf.png)
    {256, 0},    // 13: 酿造台 (brewing_stand.png)
    {288, 0},    // 14: 酿造台底座 (brewing_stand_base.png)
    {288, 32},   // 15: 命令方块 (command_block.png)
    {320, 32},   // 16: 比较器(关) (comparator_off.png)
    {352, 32},   // 17: 比较器(开) (comparator_on.png)
    {384, 32},   // 18: 工作台(正面) (crafting_table_front.png)
    {416, 32},   // 19: 工作台(侧面) (crafting_table_side.png)
    {448, 32},   // 20: 工作台(顶部) (crafting_table_top.png)
    {480, 32},   // 21: 日光探测器(侧面) (daylight_detector_side.png)
    {512, 32},   // 22: 日光探测器(顶部) (daylight_detector_top.png)

    // 索引23-36: 矿石/资源区域
    {320, 0},    // 23: 砖块 (brick.png)
    {832, 0},    // 24: 粘土 (clay.png)
    {864, 0},    // 25: 煤矿块 (coal_block.png)
    {896, 0},    // 26: 煤矿石 (coal_ore.png)
    {928, 0},    // 27: 圆石 (cobblestone.png)
    {960, 0},    // 28: 苔石 (cobblestone_mossy.png)
    {896, 32},   // 29: 钻石块 (diamond_block.png)
    {928, 32},   // 30: 钻石矿石 (diamond_ore.png)
    {960, 32},   // 31: 泥土 (dirt.png)
    {480, 64},   // 32: 绿宝石块 (emerald_block.png)
    {512, 64},   // 33: 绿宝石矿石 (emerald_ore.png)
    {384, 96},   // 34: 金块 (gold_block.png)
    {416, 96},   // 35: 金矿石 (gold_ore.png)
    {576, 128},  // 36: 铁块 (iron_block.png)
    {608, 128},  // 37: 铁矿石 (iron_ore.png)
    {768, 128},  // 38: 青金石块 (lapis_block.png)
    {800, 128},  // 39: 青金石矿石 (lapis_ore.png)

    // 索引40-55: 植物/作物区域
    {352, 0},    // 40: 仙人掌(底) (cactus_bottom.png)
    {384, 0},    // 41: 仙人掌(侧) (cactus_side.png)
    {416, 0},    // 42: 仙人掌(顶) (cactus_top.png)
    {576, 0},    // 43: 胡萝卜(阶段0) (carrots_stage_0.png)
    {608, 0},    // 44: 胡萝卜(阶段1) (carrots_stage_1.png)
    {640, 0},    // 45: 胡萝卜(阶段2) (carrots_stage_2.png)
    {672, 0},    // 46: 胡萝卜(阶段3) (carrots_stage_3.png)
    {992, 0},    // 47: 可可豆(阶段0) (cocoa_stage_0.png)
    {224, 32},   // 48: 可可豆(阶段1) (cocoa_stage_1.png)
    {256, 32},   // 49: 可可豆(阶段2) (cocoa_stage_2.png)
    {544, 32},   // 50: 枯死灌木 (deadbush.png)
    {832, 64},   // 51: 蕨 (fern.png)
    {864, 64},   // 52: 蒲公英 (flower_dandelion.png)
    {928, 64},   // 53: 玫瑰 (flower_rose.png)
    {576, 224},  // 54: 高草 (tallgrass.png)

    // 索引55-75: 树木/木材区域
    {832, 128},  // 55: 白桦树叶 (leaves_birch.png)
    {864, 128},  // 56: 白桦树叶(不透明) (leaves_birch_opaque.png)
    {896, 128},  // 57: 丛林树叶 (leaves_jungle.png)
    {928, 128},  // 58: 丛林树叶(不透明) (leaves_jungle_opaque.png)
    {832, 128},  // 59: 橡树叶 (leaves_oak.png) - 与白桦树叶相同坐标
    {864, 128},  // 60: 橡树叶(不透明) (leaves_oak_opaque.png)
    {960, 128},  // 61: 云杉树叶 (leaves_spruce.png)
    {992, 128},  // 62: 云杉树叶(不透明) (leaves_spruce_opaque.png)
    {256, 160},  // 63: 白桦原木 (log_birch.png)
    {320, 160},  // 64: 丛林原木 (log_jungle.png)
    {352, 160},  // 65: 橡木原木 (log_oak.png)
    {384, 160},  // 66: 云杉原木 (log_spruce.png)
    {352, 192},  // 67: 白桦木板 (planks_birch.png)
    {384, 192},  // 68: 丛林木板 (planks_jungle.png)
    {416, 192},  // 69: 橡木木板 (planks_oak.png)
    {448, 192},  // 70: 云杉木板 (planks_spruce.png)
    {224, 896},  // 71: 白桦树苗 (sapling_birch.png)
    {224, 928},  // 72: 丛林树苗 (sapling_jungle.png)
    {224, 960},  // 73: 橡树苗 (sapling_oak.png)
    {224, 992},  // 74: 云杉树苗 (sapling_spruce.png)

    // 索引75-89: 食物/作物区域
    {448, 0},    // 75: 蛋糕(底) (cake_bottom.png)
    {480, 0},    // 76: 蛋糕(内) (cake_inner.png)
    {512, 0},    // 77: 蛋糕(侧) (cake_side.png)
    {544, 0},    // 78: 蛋糕(顶) (cake_top.png)
    {576, 0},    // 79: 马铃薯(阶段0) (potatoes_stage_0.png)
    {608, 0},    // 80: 马铃薯(阶段1) (potatoes_stage_1.png)
    {640, 0},    // 81: 马铃薯(阶段2) (potatoes_stage_2.png)
    {480, 192},  // 82: 马铃薯(阶段3) (potatoes_stage_3.png)
    {928, 224},  // 83: 小麦(阶段0) (wheat_stage_0.png)
    {960, 224},  // 84: 小麦(阶段1) (wheat_stage_1.png)
    {992, 224},  // 85: 小麦(阶段2) (wheat_stage_2.png)
    {256, 256},  // 86: 小麦(阶段3) (wheat_stage_3.png)
    {256, 288},  // 87: 小麦(阶段4) (wheat_stage_4.png)
    {256, 320},  // 88: 小麦(阶段5) (wheat_stage_5.png)
    {256, 352},  // 89: 小麦(阶段6) (wheat_stage_6.png)
    {256, 384},  // 90: 小麦(阶段7) (wheat_stage_7.png)

    // 索引91-118: 红石元件区域
    {992, 32},   // 91: 发射器(水平) (dispenser_front_horizontal.png)
    {224, 64},   // 92: 发射器(垂直) (dispenser_front_vertical.png)
    {416, 64},   // 93: 投掷器(水平) (dropper_front_horizontal.png)
    {448, 64},   // 94: 投掷器(垂直) (dropper_front_vertical.png)
    {416, 128},  // 95: 漏斗(内) (hopper_inside.png)
    {448, 128},  // 96: 漏斗(外) (hopper_outside.png)
    {480, 128},  // 97: 漏斗(顶) (hopper_top.png)
    {224, 160},  // 98: 拉杆 (lever.png)
    {224, 192},  // 99: 活塞(内) (piston_inner.png)
    {256, 192},  // 100: 活塞(侧) (piston_side.png)
    {288, 192},  // 101: 活塞(普通顶) (piston_top_normal.png)
    {320, 192},  // 102: 活塞(粘性顶) (piston_top_sticky.png)
    {224, 320},  // 103: 红石块 (redstone_block.png)
    {224, 352},  // 104: 红石线(十字) (redstone_dust_cross.png)
    {224, 416},  // 105: 红石线(线) (redstone_dust_line.png)
    {224, 448},  // 106: 红石灯(关) (redstone_lamp_off.png)
    {224, 480},  // 107: 红石灯(开) (redstone_lamp_on.png)
    {224, 512},  // 108: 红石矿石 (redstone_ore.png)
    {224, 544},  // 109: 红石火把(关) (redstone_torch_off.png)
    {224, 576},  // 110: 红石火把(开) (redstone_torch_on.png)
    {224, 640},  // 111: 中继器(关) (repeater_off.png)
    {224, 672},  // 112: 中继器(开) (repeater_on.png)

    // 索引113-121: 铁轨区域
    {864, 192},  // 113: 激活铁轨 (rail_activator.png)
    {896, 192},  // 114: 激活铁轨(充能) (rail_activator_powered.png)
    {928, 192},  // 115: 探测铁轨 (rail_detector.png)
    {960, 192},  // 116: 探测铁轨(充能) (rail_detector_powered.png)
    {992, 192},  // 117: 充能铁轨 (rail_golden.png)
    {224, 224},  // 118: 充能铁轨(充能) (rail_golden_powered.png)
    {224, 256},  // 119: 普通铁轨 (rail_normal.png)
    {224, 288},  // 120: 普通铁轨(转弯) (rail_normal_turned.png)

    // 索引121-127: 床区域
    {192, 800},  // 121: 床脚(端) (bed_feet_end.png)
    {192, 832},  // 122: 床脚(侧) (bed_feet_side.png)
    {192, 864},  // 123: 床脚(顶) (bed_feet_top.png)
    {192, 896},  // 124: 床头(端) (bed_head_end.png)
    {192, 928},  // 125: 床头(侧) (bed_head_side.png)
    {192, 960},  // 126: 床头(顶) (bed_head_top.png)

    // 索引127-140: 容器/设备区域
    {704, 0},    // 127: 炼药锅(底) (cauldron_bottom.png)
    {736, 0},    // 128: 炼药锅(内) (cauldron_inner.png)
    {768, 0},    // 129: 炼药锅(侧) (cauldron_side.png)
    {800, 0},    // 130: 炼药锅(顶) (cauldron_top.png)
    {544, 64},   // 131: 附魔台(底) (enchanting_table_bottom.png)
    {576, 64},   // 132: 附魔台(侧) (enchanting_table_side.png)
    {608, 64},   // 133: 附魔台(顶) (enchanting_table_top.png)
    {672, 64},   // 134: 末地传送门(眼) (endframe_eye.png)
    {704, 64},   // 135: 末地传送门(侧) (endframe_side.png)
    {736, 64},   // 136: 末地传送门(顶) (endframe_top.png)
    {960, 64},   // 137: 熔炉(正面关) (furnace_front_off.png)
    {992, 64},   // 138: 熔炉(正面开) (furnace_front_on.png)
    {224, 96},   // 139: 熔炉(侧) (furnace_side.png)
    {256, 96},   // 140: 熔炉(顶) (furnace_top.png)
    {672, 128},  // 141: 音符盒 (noteblock.png)
    {672, 128},  // 142: 唱片机(侧) (jukebox_side.png) - 与音符盒相同坐标
    {704, 128},  // 143: 唱片机(顶) (jukebox_top.png)

    // 索引144-157: 环境/自然区域
    {768, 64},   // 144: 耕地(干) (farmland_dry.png)
    {800, 64},   // 145: 耕地(湿) (farmland_wet.png)
    {448, 96},   // 146: 草(侧) (grass_side.png)
    {480, 96},   // 147: 草(侧覆盖) (grass_side_overlay.png)
    {512, 96},   // 148: 草(侧雪) (grass_side_snowed.png)
    {544, 96},   // 149: 草(顶) (grass_top.png)
    {576, 96},   // 150: 砂砾 (gravel.png)
    {512, 128},  // 151: 冰 (ice.png)
    {768, 160},  // 152: 菌丝(侧) (mycelium_side.png)
    {800, 160},  // 153: 菌丝(顶) (mycelium_top.png)
    {224, 704},  // 154: 沙 (sand.png)
    {256, 224},  // 155: 雪 (snow.png)
    {288, 224},  // 156: 灵魂沙 (soul_sand.png)
    {320, 224},  // 157: 海绵 (sponge.png)

    // 索引158-170: 石头/建筑区域
    {640, 64},   // 158: 末地石 (end_stone.png)
    {608, 96},   // 159: 硬化粘土 (hardened_clay.png)
    {832, 160},  // 160: 地狱砖 (nether_brick.png)
    {960, 160},  // 161: 地狱岩 (netherrack.png)
    {992, 160},  // 162: 黑曜石 (obsidian.png)
    {352, 224},  // 163: 石头 (stone.png)
    {448, 224},  // 164: 石砖 (stonebrick.png)
    {480, 224},  // 165: 雕刻石砖 (stonebrick_carved.png)
    {512, 224},  // 166: 裂石砖 (stonebrick_cracked.png)
    {544, 224},  // 167: 苔石砖 (stonebrick_mossy.png)

    // 索引168-169: 玻璃区域
    {288, 96},   // 168: 玻璃 (glass.png)
    {320, 96},   // 169: 玻璃板(顶) (glass_pane_top.png)

    // 索引170-172: 下界/末地区域
    {864, 160},  // 170: 下界疣(阶段0) (nether_wart_stage_0.png)
    {896, 160},  // 171: 下界疣(阶段1) (nether_wart_stage_1.png)
    {928, 160},  // 172: 下界疣(阶段2) (nether_wart_stage_2.png)

    // 索引173-195: 工具/杂项区域
    {576, 32},   // 173: 破坏阶段0 (destroy_stage_0.png)
    {608, 32},   // 174: 破坏阶段1 (destroy_stage_1.png)
    {640, 32},   // 175: 破坏阶段2 (destroy_stage_2.png)
    {672, 32},   // 176: 破坏阶段3 (destroy_stage_3.png)
    {704, 32},   // 177: 破坏阶段4 (destroy_stage_4.png)
    {736, 32},   // 178: 破坏阶段5 (destroy_stage_5.png)
    {768, 32},   // 179: 破坏阶段6 (destroy_stage_6.png)
    {800, 32},   // 180: 破坏阶段7 (destroy_stage_7.png)
    {832, 32},   // 181: 破坏阶段8 (destroy_stage_8.png)
    {864, 32},   // 182: 破坏阶段9 (destroy_stage_9.png)
    {384, 64},   // 183: 龙蛋 (dragon_egg.png)
    {896, 64},   // 184: 花盆 (flower_pot.png)
    {544, 128},  // 185: 铁栏杆 (iron_bars.png)
    {640, 128},  // 186: 物品框背景 (itemframe_background.png)
    {736, 128},  // 187: 梯子 (ladder.png)
    {544, 160},  // 188: 刷怪笼 (mob_spawner.png)
    {224, 608},  // 189: 甘蔗 (reeds.png)
    {704, 224},  // 190: 火把 (torch_on.png)
    {736, 224},  // 191: 活板门 (trapdoor.png)
    {768, 224},  // 192: 绊线 (trip_wire.png)
    {800, 224},  // 193: 绊线钩 (trip_wire_source.png)
    {832, 224},  // 194: 藤蔓 (vine.png)
    {864, 224},  // 195: 睡莲 (waterlily.png)
    {896, 224},  // 196: 蜘蛛网 (web.png)

    // 索引197-203: 蘑菇区域
    {576, 160},  // 197: 蘑菇(内部) (mushroom_block_inside.png)
    {608, 160},  // 198: 蘑菇(棕色皮) (mushroom_block_skin_brown.png)
    {640, 160},  // 199: 蘑菇(红色皮) (mushroom_block_skin_red.png)
    {672, 160},  // 200: 蘑菇(茎皮) (mushroom_block_skin_stem.png)
    {704, 160},  // 201: 棕色蘑菇 (mushroom_brown.png)
    {736, 160},  // 202: 红色蘑菇 (mushroom_red.png)

    // 索引203-208: 石英区域
    {640, 192},  // 203: 石英块(底) (quartz_block_bottom.png)
    {672, 192},  // 204: 雕刻石英块 (quartz_block_chiseled.png)
    {736, 192},  // 205: 石英柱 (quartz_block_lines.png)
    {800, 192},  // 206: 石英块(侧) (quartz_block_side.png)
    {832, 192},  // 207: 石英矿石 (quartz_ore.png)

    // 索引208-215: 南瓜/西瓜区域
    {512, 192},  // 208: 南瓜(脸关) (pumpkin_face_off.png)
    {544, 192},  // 209: 南瓜(脸开) (pumpkin_face_on.png)
    {576, 192},  // 210: 南瓜(侧) (pumpkin_side.png)
    {608, 192},  // 211: 南瓜(顶) (pumpkin_top.png)
    {416, 160},  // 212: 西瓜(侧) (melon_side.png)
    {512, 160},  // 213: 西瓜(顶) (melon_top.png)

    // 索引214-219: 砂岩区域
    {224, 736},  // 214: 砂岩(底) (sandstone_bottom.png)
    {224, 768},  // 215: 雕刻砂岩 (sandstone_carved.png)
    {224, 800},  // 216: 砂岩(普通) (sandstone_normal.png)
    {224, 832},  // 217: 平滑砂岩 (sandstone_smooth.png)
    {224, 864},  // 218: 砂岩(顶) (sandstone_top.png)

    // 索引219-235: 染色粘土区域
    {640, 96},   // 219: 黑色粘土 (hardened_clay_stained_black.png)
    {672, 96},   // 220: 蓝色粘土 (hardened_clay_stained_blue.png)
    {704, 96},   // 221: 棕色粘土 (hardened_clay_stained_brown.png)
    {736, 96},   // 222: 青色粘土 (hardened_clay_stained_cyan.png)
    {768, 96},   // 223: 灰色粘土 (hardened_clay_stained_gray.png)
    {800, 96},   // 224: 绿色粘土 (hardened_clay_stained_green.png)
    {832, 96},   // 225: 淡蓝色粘土 (hardened_clay_stained_light_blue.png)
    {864, 96},   // 226: 黄绿色粘土 (hardened_clay_stained_lime.png)
    {896, 96},   // 227: 品红色粘土 (hardened_clay_stained_magenta.png)
    {928, 96},   // 228: 橙色粘土 (hardened_clay_stained_orange.png)
    {960, 96},   // 229: 粉色粘土 (hardened_clay_stained_pink.png)
    {992, 96},   // 230: 紫色粘土 (hardened_clay_stained_purple.png)
    {224, 128},  // 231: 红色粘土 (hardened_clay_stained_red.png)
    {256, 128},  // 232: 淡灰色粘土 (hardened_clay_stained_silver.png)
    {288, 128},  // 233: 白色粘土 (hardened_clay_stained_white.png)
    {320, 128},  // 234: 黄色粘土 (hardened_clay_stained_yellow.png)

    // 索引235-251: 染色羊毛区域
    {256, 416},  // 235: 黑色羊毛 (wool_colored_black.png)
    {256, 448},  // 236: 蓝色羊毛 (wool_colored_blue.png)
    {256, 480},  // 237: 棕色羊毛 (wool_colored_brown.png)
    {256, 512},  // 238: 青色羊毛 (wool_colored_cyan.png)
    {256, 544},  // 239: 灰色羊毛 (wool_colored_gray.png)
    {256, 576},  // 240: 绿色羊毛 (wool_colored_green.png)
    {256, 608},  // 241: 淡蓝色羊毛 (wool_colored_light_blue.png)
    {256, 640},  // 242: 黄绿色羊毛 (wool_colored_lime.png)
    {256, 672},  // 243: 品红色羊毛 (wool_colored_magenta.png)
    {256, 704},  // 244: 橙色羊毛 (wool_colored_orange.png)
    {256, 736},  // 245: 粉色羊毛 (wool_colored_pink.png)
    {256, 768},  // 246: 紫色羊毛 (wool_colored_purple.png)
    {256, 800},  // 247: 红色羊毛 (wool_colored_red.png)
    {256, 832},  // 248: 淡灰色羊毛 (wool_colored_silver.png)
    {256, 864},  // 249: 白色羊毛 (wool_colored_white.png)
    {256, 896},  // 250: 黄色羊毛 (wool_colored_yellow.png)

    // 索引251-254: TNT区域
    {608, 224},  // 251: TNT(底) (tnt_bottom.png)
    {640, 224},  // 252: TNT(侧) (tnt_side.png)
    {672, 224},  // 253: TNT(顶) (tnt_top.png)
};

// 使用示例：
// 获取橡木木板的纹理坐标
// glm::vec2 oakPlanksCoord = textureCoords[69];  // {416, 192}

glm::vec2 getTextureCoords(int index)
{
    if (index < 0 || index >= textureCoords.size())
    {
        return glm::vec2(0.0f, 0.0f); // 返回默认坐标或抛出异常
    }
	return textureCoords[index];
}
std::vector<InstanceData> instances;
void generateInstances(int numInstances)
{
    glm::mat4 model = glm::mat4(1.0f);
    for (int i = 0; i < numInstances; i++)
    {
        // 生成随机位置
        float x = static_cast<float>(rand() % 10 - 5); // -50到50之间
        float y = static_cast<float>(rand() % 10 - 5);
        float z = static_cast<float>(rand() % 10 - 5);
        // 创建实例数据
        InstanceData instance;
        instance.modelMatrix = glm::translate(model, glm::vec3(x, y, z));
        instance.textureOffset.x = textureCoords[216].x/chang; // 随机选择一个纹理索引
		instance.textureOffset.y = textureCoords[216].y/kuan;
        // 添加到实例列表
        instances.push_back(instance);
	}
}