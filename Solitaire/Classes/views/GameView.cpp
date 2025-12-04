#include "GameView.h"
#include "CardView.h"
#include "../configs/LevelConfigLoader.h"
#include "../models/CardModel.h"
#include "../controllers/GameController.h"
#include "../services/GameModelGenerator.h" // 必须引用这个服务

USING_NS_CC;

Scene* GameView::createScene() {
    auto scene = Scene::create();
    auto layer = GameView::create();
    scene->addChild(layer);
    return scene;
}

bool GameView::init() {
    if (!Layer::init()) return false;

    // --- 1. 画背景 (还原文档配色) ---

    // 主牌区 (上方)：土黄色
    auto mainZone = LayerColor::create(Color4B(170, 130, 90, 255), 1080, 1500);
    mainZone->setPosition(0, 580);
    this->addChild(mainZone);

    // 堆牌区 (下方)：深紫色
    auto stackZone = LayerColor::create(Color4B(140, 20, 120, 255), 1080, 580);
    stackZone->setPosition(0, 0);
    this->addChild(stackZone);

    // --- 2. 加载配置并生成【随机】数据 ---
    // 我们不再直接使用 config 里的死数据，而是用 Generator 生成随机牌

    LevelConfig config = LevelConfigLoader::loadLevel(1);

    std::vector<CardModel*> randomPlayCards;
    std::vector<CardModel*> randomStackCards;

    // 调用服务层：洗牌、发牌
    GameModelGenerator::generateRandomLevel(config, randomPlayCards, randomStackCards);

    // --- 3. 生成【主牌区】卡牌 (使用洗好的牌) ---
    for (auto model : randomPlayCards) {
        // 创建 View
        auto view = CardView::create(model);

        // 坐标已经在 Generator 里根据 Config 设置好了
        view->setPosition(model->position);

        // 注册点击：消除逻辑
        view->setClickCallback([=](CardView* card) {
            GameController::getInstance()->onCardClicked(card);
            });

        this->addChild(view);
    }

    // --- 4. 生成【下方牌堆】卡牌 (使用洗好的牌) ---
    int totalStack = randomStackCards.size();

    for (int i = 0; i < totalStack; i++) {
        auto model = randomStackCards[i];
        auto view = CardView::create(model);
        this->addChild(view);

        bool isLastOne = (i == totalStack - 1);

        if (isLastOne) {
            // === 初始底牌 (Open Pile) ===
            // 位置：右侧 700
            view->setPosition(700, 250);

            // 强制设为正面 (UI更新)
            model->isFaceUp = true;
            view->updateView();

            GameController::getInstance()->setStackCard(view);
        }
        else {
            // === 抽牌堆 (Draw Pile) ===
            // 位置：左侧 380，稍微错开
            view->setPosition(380 + i * 2, 250);

            // 强制设为背面
            model->isFaceUp = false;
            view->updateView();

            // 注册点击：翻牌逻辑
            view->setClickCallback([=](CardView* card) {
                GameController::getInstance()->onDrawCardClicked(card);
                });
        }
    }

    // --- 5. 添加【回退】文字按钮 ---

    // 使用 Arial 字体，字号 30，白色
    auto label = Label::createWithSystemFont("UNDO", "Arial", 30);

    auto undoBtn = MenuItemLabel::create(label, [](Ref* sender) {
        GameController::getInstance()->onUndoClicked();
        });

    // 位置：放在底牌的右侧 (900, 250)
    undoBtn->setPosition(900, 250);

    auto menu = Menu::create(undoBtn, nullptr);
    menu->setPosition(Vec2::ZERO);

    // 【关键】设置层级为 9999，确保按钮永远在最上层
    this->addChild(menu, 9999);

    return true;
}