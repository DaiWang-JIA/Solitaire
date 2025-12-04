#include "UndoManager.h"

UndoManager::UndoManager() {
    _history.clear();
}

UndoManager::~UndoManager() {
    // 清理残留的引用，防止内存泄漏
    for (const auto& action : _history) {
        if (action.previousStackCard) {
            action.previousStackCard->release();
        }
    }
    _history.clear();
}

void UndoManager::pushAction(CardView* current, CardView* old, std::string type) {
    UndoAction action;
    action.currentCard = current;
    action.previousStackCard = old;
    action.originalPos = current->getPosition();

    // 记录原始状态：如果是翻牌(FLIP)，原来肯定是背面(false)；如果是消除(MATCH)，原来是正面(true)
    if (type == "FLIP") action.originalFaceUp = false;
    else if (type == "MATCH") action.originalFaceUp = true;
    else action.originalFaceUp = !current->getModel()->isFaceUp;

    action.type = type;

    // 只要 UndoManager 持有这个旧牌的指针，就必须 retain，防止它被 Cocos 自动回收
    if (old) {
        old->retain();
    }

    _history.push_back(action);
    cocos2d::log("MANAGER: Pushed undo action. Stack size: %d", (int)_history.size());
}

UndoAction UndoManager::popAction() {
    if (_history.empty()) {
        // 返回一个空的 action
        return { nullptr, nullptr, cocos2d::Vec2::ZERO, false, "" };
    }

    UndoAction action = _history.back();
    _history.pop_back();
    return action;
}

bool UndoManager::hasHistory() const {
    return !_history.empty();
}