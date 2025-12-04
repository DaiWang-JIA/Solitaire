#pragma once
#include "cocos2d.h"

// 继承自 Layer，作为游戏的主画面
class GameView : public cocos2d::Layer {
public:
    // 创建场景的静态方法（Cocos的标准写法）
    static cocos2d::Scene* createScene();

    // 初始化函数
    virtual bool init();

    // 宏：自动实现 create() 函数
    CREATE_FUNC(GameView);
};
