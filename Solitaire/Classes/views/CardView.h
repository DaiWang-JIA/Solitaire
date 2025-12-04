#pragma once
#include "cocos2d.h"
#include "../models/CardModel.h"
#include <functional>

class CardView : public cocos2d::Node {
public:
    // 析构函数：对象销毁时自动调用
    ~CardView();

    static CardView* create(CardModel* model);
    virtual bool init(CardModel* model);

    void updateView();
    void setClickCallback(std::function<void(CardView*)> callback);
    CardModel* getModel() { return _model; }

private:
    CardModel* _model;
    std::function<void(CardView*)> _clickCallback;
    void initTouchEvents();
};