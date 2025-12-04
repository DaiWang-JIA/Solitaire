#pragma once
#include "cocos2d.h"
#include "../views/CardView.h"

#include "../managers/UndoManager.h" 

class GameController {
public:
    static GameController* getInstance();

    // 初始化（分配 Manager 内存）
    void init();

    void onCardClicked(CardView* clickedCard);
    void onDrawCardClicked(CardView* clickedCard);
    void onUndoClicked();

    void setStackCard(CardView* stackCard);

private:
    CardView* _topStackCard;

    //Manager 作为 Controller 的成员变量
    UndoManager* _undoManager;
};