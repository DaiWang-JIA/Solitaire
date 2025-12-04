#include "AppDelegate.h"
#include "views/GameView.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if (!glview) {
        // 竖屏窗口，缩放 0.45 方便调试
        glview = GLViewImpl::createWithRect("Solitaire", Rect(0, 0, 1080, 2080), 0.45f);
        director->setOpenGLView(glview);
    }

    // 适配策略
    glview->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);

    // 【修改在这里】关闭左下角的 FPS 数字显示
    director->setDisplayStats(false);

    // 帧率 60
    director->setAnimationInterval(1.0f / 60);

    // 运行场景
    auto scene = GameView::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}