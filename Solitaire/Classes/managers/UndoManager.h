#pragma once
#include "cocos2d.h"
#include "../models/UndoModel.h"
#include <vector>

class UndoManager {
public:
    UndoManager();
    ~UndoManager();

    // 记录操作
    void pushAction(CardView* current, CardView* old, std::string type);

    // 撤销操作（返回复活的旧底牌，如果没有旧底牌则返回nullptr）
    UndoAction popAction();

    // 检查是否有历史记录
    bool hasHistory() const;

private:
    std::vector<UndoAction> _history;
};