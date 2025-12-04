#include "CardView.h"
#include <string>

USING_NS_CC;

CardView::~CardView() {
    CC_SAFE_RELEASE(_model);
}

CardView* CardView::create(CardModel* model) {
    CardView* pRet = new(std::nothrow) CardView();
    if (pRet && pRet->init(model)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool CardView::init(CardModel* model) {
    if (!Node::init()) return false;

    _model = model;
    _model->retain(); // 锁住数据

    updateView();
    initTouchEvents(); // 开启触摸

    return true;
}

void CardView::setClickCallback(std::function<void(CardView*)> callback) {
    _clickCallback = callback;
}

void CardView::initTouchEvents() {
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 p = this->convertTouchToNodeSpace(touch);
        Rect rect = Rect(0, 0, this->getContentSize().width, this->getContentSize().height);

        // 只要点中矩形范围就返回 true
        if (rect.containsPoint(p)) {
            return true;
        }
        return false;
        };

    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (_clickCallback) {
            _clickCallback(this);
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::updateView() {
    this->removeAllChildren();

    // 1. 加载底板
    auto bg = Sprite::create("res/card_general.png");
    if (!bg) return;

    bg->setAnchorPoint(Vec2::ZERO);
    this->addChild(bg);
    this->setContentSize(bg->getContentSize());

    //如果是背面，染成蓝色，然后直接结束 ---
    if (!_model->isFaceUp) {
        bg->setColor(Color3B(100, 149, 237)); 
        return; // 背面画完就返回，不画花色和数字
    }
    // ----------------------------------------------------

    // 下面是正面的绘制逻辑 
    std::string colorStr = "black";
    std::string suitFile = "";

    if (_model->suit == CST_CLUBS || _model->suit == CST_SPADES) colorStr = "black";
    else colorStr = "red";

    switch (_model->suit) {
    case CST_CLUBS:    suitFile = "res/club.png"; break;
    case CST_DIAMONDS: suitFile = "res/diamond.png"; break;
    case CST_HEARTS:   suitFile = "res/heart.png"; break;
    case CST_SPADES:   suitFile = "res/spade.png"; break;
    }

    std::string faceStr = std::to_string(_model->face);
    if (_model->face == 1) faceStr = "A";
    else if (_model->face == 11) faceStr = "J";
    else if (_model->face == 12) faceStr = "Q";
    else if (_model->face == 13) faceStr = "K";

    auto bigNum = Sprite::create("res/big_" + colorStr + "_" + faceStr + ".png");
    if (bigNum) {
        bigNum->setPosition(bg->getContentSize().width / 2, bg->getContentSize().height / 2);
        bg->addChild(bigNum);
    }

    auto smallNum = Sprite::create("res/small_" + colorStr + "_" + faceStr + ".png");
    if (smallNum) {
        smallNum->setAnchorPoint(Vec2(0.5, 1));
        smallNum->setPosition(35, bg->getContentSize().height - 20);
        bg->addChild(smallNum);
    }

    if (!suitFile.empty()) {
        auto smallSuit = Sprite::create(suitFile);
        if (smallSuit) {
            smallSuit->setAnchorPoint(Vec2(0.5, 1));
            smallSuit->setScale(0.5f);
            smallSuit->setPosition(35, bg->getContentSize().height - 70);
            bg->addChild(smallSuit);
        }
    }
}