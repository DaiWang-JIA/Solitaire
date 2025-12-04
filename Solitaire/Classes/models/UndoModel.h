#pragma once
#include "cocos2d.h"
#include "../views/CardView.h"
#include <string>

// 动作结构体定义 (只在这里写一次！)
struct UndoAction {
    CardView* currentCard;       // 飞回去的牌
    CardView* previousStackCard; // 要复活的旧牌
    cocos2d::Vec2 originalPos;   // 原始位置
    bool originalFaceUp;         // 原始朝向
    std::string type;            // 类型
};

class UndoModel {
    // 作为一个纯数据头文件存在
};