#pragma once
#include "../configs/LevelConfig.h"
#include "../models/CardModel.h"
#include <vector>

class GameModelGenerator {
public:
    // 静态方法：传入关卡配置（只为了要位置），返回一副洗好的随机牌数据
    static void generateRandomLevel(const LevelConfig& config,
        std::vector<CardModel*>& outPlayCards,
        std::vector<CardModel*>& outStackCards);
};