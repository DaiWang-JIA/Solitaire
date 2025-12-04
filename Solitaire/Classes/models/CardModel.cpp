#include "CardModel.h"

CardModel* CardModel::create(int id, int face, int suit) {
    CardModel* model = new (std::nothrow) CardModel();
    if (model && model->init(id, face, suit)) {
        model->autorelease();
        return model;
    }
    delete model;
    return nullptr;
}

bool CardModel::init(int id, int faceVal, int suitVal) {
    this->id = id;
    this->face = faceVal;
    this->suit = suitVal;
    this->isFaceUp = true; // 默认正面

    // 【关键修复】初始化坐标
    this->position = cocos2d::Vec2::ZERO;

    return true;
}