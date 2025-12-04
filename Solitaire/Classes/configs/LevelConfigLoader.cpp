#include "LevelConfigLoader.h"
#include "json/document.h"  // 引入 rapidjson
#include "json/rapidjson.h"

USING_NS_CC;

LevelConfig LevelConfigLoader::loadLevel(int levelId) {
    LevelConfig config; // 准备返回的数据对象

    // 1. 读取文件内容 (暂时写死读取 level1.json)
    std::string filename = StringUtils::format("level1.json");
    std::string content = FileUtils::getInstance()->getStringFromFile(filename);

    if (content.empty()) {
        CCLOG("Error: Failed to read %s", filename.c_str());
        return config;
    }

    // 2. 解析 JSON
    rapidjson::Document doc;
    doc.Parse(content.c_str());

    if (doc.HasParseError()) {
        CCLOG("Error: JSON Parse Error in %s", filename.c_str());
        return config;
    }

    // 3. 解析 "Playfield" (主牌区数据)
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfield = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfield.Size(); i++) {
            const rapidjson::Value& item = playfield[i];

            CardConfigData cardData;
            // 读取 CardFace, CardSuit
            cardData.cardFace = item["CardFace"].GetInt();
            cardData.cardSuit = item["CardSuit"].GetInt();

            // 读取嵌套的 Position
            if (item.HasMember("Position")) {
                const rapidjson::Value& pos = item["Position"];
                cardData.x = pos["x"].GetDouble(); // 支持小数
                cardData.y = pos["y"].GetDouble();
            }

            config.playfieldCards.push_back(cardData);
        }
    }

    // 4. 解析 "Stack" (堆牌区数据)
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& stack = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stack.Size(); i++) {
            const rapidjson::Value& item = stack[i];

            CardConfigData cardData;
            cardData.cardFace = item["CardFace"].GetInt();
            cardData.cardSuit = item["CardSuit"].GetInt();
            // Stack里的牌通常没有坐标或者是(0,0)，但也读一下防备
            if (item.HasMember("Position")) {
                const rapidjson::Value& pos = item["Position"];
                cardData.x = pos["x"].GetDouble();
                cardData.y = pos["y"].GetDouble();
            }

            config.stackCards.push_back(cardData);
        }
    }

    CCLOG("Success: Loaded level %d with %d play cards and %d stack cards",
        levelId, (int)config.playfieldCards.size(), (int)config.stackCards.size());

    return config;
}