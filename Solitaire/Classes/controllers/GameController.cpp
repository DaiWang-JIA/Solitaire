#include "GameController.h"
#include <cmath>

USING_NS_CC;

static GameController* _instance = nullptr;

GameController* GameController::getInstance() {
    if (!_instance) {
        _instance = new GameController();
        _instance->init(); // 确保初始化
    }
    return _instance;
}

void GameController::init() {
    //手动管理生命周期，不使用单例
    if (!_undoManager) {
        _undoManager = new UndoManager();
    }
    _topStackCard = nullptr;
}

void GameController::setStackCard(CardView* stackCard) {
    _topStackCard = stackCard;
    if (_topStackCard) {
        _topStackCard->getModel()->isFaceUp = true;
        _topStackCard->updateView();
        _topStackCard->setLocalZOrder(10);
    }
}

// 翻牌逻辑
void GameController::onDrawCardClicked(CardView* clickedCard) {
    if (!clickedCard || !_topStackCard || clickedCard->getModel()->isFaceUp) return;

    // 1. 记录到 Manager
    if (_undoManager) {
        _undoManager->pushAction(clickedCard, _topStackCard, "FLIP");
    }

    // 2. 逻辑处理
    clickedCard->getModel()->isFaceUp = true;
    clickedCard->updateView();

    Vec2 targetPos = _topStackCard->getPosition();
    clickedCard->setLocalZOrder(100);

    clickedCard->runAction(Sequence::create(
        MoveTo::create(0.4f, targetPos),
        CallFunc::create([this, clickedCard]() {
            if (_topStackCard) _topStackCard->removeFromParent();
            _topStackCard = clickedCard;
            _topStackCard->setLocalZOrder(10);
            }),
        nullptr
    ));
}

// 消除逻辑
void GameController::onCardClicked(CardView* clickedCard) {
    if (!clickedCard || !_topStackCard) return;
    if (!clickedCard->getModel()->isFaceUp) return;

    int playFace = clickedCard->getModel()->face;
    int stackFace = _topStackCard->getModel()->face;
    int diff = std::abs(playFace - stackFace);
    bool isMatch = (diff == 1) || (diff == 12);

    if (isMatch) {
        // 1. 记录到 Manager
        if (_undoManager) {
            _undoManager->pushAction(clickedCard, _topStackCard, "MATCH");
        }

        // 2. 逻辑处理
        clickedCard->setLocalZOrder(100);
        Vec2 target = _topStackCard->getPosition();

        clickedCard->runAction(Sequence::create(
            MoveTo::create(0.4f, target),
            CallFunc::create([this, clickedCard]() {
                _topStackCard->removeFromParent();
                _topStackCard = clickedCard;
                _topStackCard->setLocalZOrder(10);
                }),
            nullptr
        ));
    }
    else {
        clickedCard->runAction(Sequence::create(
            MoveBy::create(0.05f, Vec2(-20, 0)),
            MoveBy::create(0.05f, Vec2(40, 0)),
            MoveBy::create(0.05f, Vec2(-20, 0)),
            nullptr
        ));
    }
}

// 撤销逻辑
void GameController::onUndoClicked() {
    if (!_undoManager || !_undoManager->hasHistory()) {
        CCLOG("UNDO: No history to undo.");
        return;
    }

    CCLOG("ACTION: Undo clicked");

    // 1. 从 Manager 获取上一步的数据
    UndoAction action = _undoManager->popAction();
    if (!action.currentCard) return;

    CardView* cardToReturn = action.currentCard;
    CardView* cardToRevive = action.previousStackCard;

    // 2. 让现在的底牌飞回去 (View操作)
    cardToReturn->stopAllActions();
    cardToReturn->setLocalZOrder(100);

    cardToReturn->runAction(Sequence::create(
        MoveTo::create(0.4f, action.originalPos),
        CallFunc::create([cardToReturn, action]() {
            cardToReturn->getModel()->isFaceUp = action.originalFaceUp;
            cardToReturn->updateView();
            cardToReturn->setLocalZOrder(0);
            }),
        nullptr
    ));

    // 3. 复活旧底牌
    if (cardToRevive) {
        if (cardToReturn->getParent()) {
            cardToReturn->getParent()->addChild(cardToRevive);
        }
        // 恢复位置，这里硬编码了底牌位置，正规做法应该读配置，但在Controller里处理也算合理
        cardToRevive->setPosition(700, 250);
        cardToRevive->setLocalZOrder(10);
        cardToRevive->setVisible(true); // 确保可见

        // 恢复引用
        _topStackCard = cardToRevive;

        // Manager 已经完成了它的使命，现在 Controller 接管这个指针
        // 所以我们需要 release 一次，抵消 Manager 里的 retain
        cardToRevive->release();
    }
}