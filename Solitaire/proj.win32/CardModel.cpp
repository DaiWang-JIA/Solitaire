#include "CardModel.h"

CardModel* CardModel::create(int id, int face, int suit) {
    CardModel* model = new (std::nothrow) CardModel();
    // 强制类型转换，确保传入的 int 能转为枚举
    if (model && model->init(id, face, suit)) {
        model->autorelease();
        return model;
    }
    delete model;
    return nullptr;
}

bool CardModel::init(int id, int faceVal, int suitVal) {
    this->id = id;
    this->face = static_cast<CardFaceType>(faceVal);
    this->suit = static_cast<CardSuitType>(suitVal);
    this->isFaceUp = true; // 默认正面朝上
    this->position = cocos2d::Vec2::ZERO;
    return true;
}