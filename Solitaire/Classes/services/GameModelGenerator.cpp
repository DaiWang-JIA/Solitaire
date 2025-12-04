#include "GameModelGenerator.h"
#include <algorithm> 
#include <random>    
#include <chrono>    

void GameModelGenerator::generateRandomLevel(const LevelConfig& config,
    std::vector<CardModel*>& outPlayCards,
    std::vector<CardModel*>& outStackCards) {
    // 1. 准备一副牌 (52张)
    std::vector<CardModel*> deck;
    int idCounter = 0;

    for (int suit = 0; suit < 4; suit++) {
        for (int face = 1; face <= 13; face++) {
            deck.push_back(CardModel::create(idCounter++, face, suit));
        }
    }

    // 2. 洗牌
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));

    // 3. 发牌给主牌区
    int cardIndex = 0;
    for (const auto& posData : config.playfieldCards) {
        if (cardIndex >= (int)deck.size()) break;

        CardModel* card = deck[cardIndex++];

        // 赋值坐标 
        card->position = cocos2d::Vec2(posData.x, posData.y);
        card->isFaceUp = true;

        outPlayCards.push_back(card);
    }

    // 4. 发牌给抽牌堆
    for (const auto& posData : config.stackCards) {
        if (cardIndex >= (int)deck.size()) break;

        CardModel* card = deck[cardIndex++];
        outStackCards.push_back(card);
    }
}