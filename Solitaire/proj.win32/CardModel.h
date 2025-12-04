#pragma once
#include "cocos2d.h"

// 对应文档 [54-62]：花色枚举
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS = 0,    // 梅花
    CST_DIAMONDS = 1, // 方块
    CST_HEARTS = 2,   // 红桃
    CST_SPADES = 3,   // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

// 对应文档 [65-82]：点数枚举
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE = 1,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

class CardModel : public cocos2d::Ref {
public:
    int id;                 // 唯一ID
    CardFaceType face;      // 点数
    CardSuitType suit;      // 花色
    cocos2d::Vec2 position; // 逻辑坐标
    bool isFaceUp;          // 是否正面朝上

    static CardModel* create(int id, int face, int suit);
    bool init(int id, int face, int suit);
};
