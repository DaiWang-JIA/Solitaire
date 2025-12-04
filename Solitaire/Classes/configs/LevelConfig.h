#pragma once
#include "cocos2d.h"
#include <vector>

// 单个卡牌的配置数据（从JSON里读出来的原始数据）
struct CardConfigData {
    int cardFace;   // 对应 JSON 中的 "CardFace"
    int cardSuit;   // 对应 JSON 中的 "CardSuit"
    float x;        // 对应 JSON 中的 "Position": { "x": ... }
    float y;        // 对应 JSON 中的 "Position": { "y": ... }
};

// 整个关卡的配置
struct LevelConfig {
    // 对应 JSON 中的 "Playfield" (主牌区) [cite: 90]
    std::vector<CardConfigData> playfieldCards;

    // 对应 JSON 中的 "Stack" (堆牌区) [cite: 156]
    std::vector<CardConfigData> stackCards;
};
