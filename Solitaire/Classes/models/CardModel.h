#pragma once
#include "cocos2d.h"

// 花色枚举
enum CardSuitType {
    CST_CLUBS = 0,    // 梅花
    CST_DIAMONDS = 1, // 方块
    CST_HEARTS = 2,   // 红桃
    CST_SPADES = 3    // 黑桃
};

class CardModel : public cocos2d::Ref {
public:
    int id;
    int face; // 1=A, 2=2... 13=K
    int suit;
    bool isFaceUp;

    //这个变量，用来存坐标！
    cocos2d::Vec2 position;

    // 声明工厂方法
    static CardModel* create(int id, int face, int suit);

    // 声明初始化方法
    bool init(int id, int face, int suit);
};