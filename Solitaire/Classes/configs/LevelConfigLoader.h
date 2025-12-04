#pragma once
#include "cocos2d.h"
#include "LevelConfig.h" // 引用我们刚才定义的结构体

class LevelConfigLoader {
public:
    // 静态函数：传入关卡ID，返回读取到的配置数据
    static LevelConfig loadLevel(int levelId);
};