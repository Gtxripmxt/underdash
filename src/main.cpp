#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/loader/SettingV3.hpp>
#include "ui/utMenuBackground.hpp"
#include "ui/utMenuItemLayer.hpp"

using namespace geode::prelude;

bool playSfx;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        createUtMenu();
        playSfx = true;
        return true;
    }

    void createUtMenu() {
        auto utMenuButton = CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSprite("soul.png"_spr, 0.9f),
            this,
            menu_selector(MyMenuLayer::onShowUtMenu)
        );
        auto menu = this->getChildByID("bottom-menu");
        menu->addChild(utMenuButton);
        utMenuButton->setID("utMenuButton"_spr);
        menu->updateLayout();

        onShowUtMenu();
        if (!this->getChildByID("UtMenuBackground"_spr)) this->addChild(utMenuBackground::create());
        if (!this->getChildByID("UtMenuItemLayer"_spr)) this->addChild(utMenuItemLayer::create(this, menu_selector(MyMenuLayer::onShowClassicMenu)));
    }

    void onShowUtMenu(CCObject* = nullptr) {
        CCObject* obj;
        CCARRAY_FOREACH(this->getChildren(), obj) {
            auto item = typeinfo_cast<CCNode*>(obj);
            if (item && item->getID() != "FLAlertLayer") item->setVisible(false);
        }
        if (auto bg = this->getChildByID("UtMenuBackground"_spr)) bg->setVisible(true);
        if (auto layer = this->getChildByID("UtMenuItemLayer"_spr)) layer->setVisible(true);
    }

    void onShowClassicMenu(CCObject* = nullptr) {
        CCObject* obj;
        CCARRAY_FOREACH(this->getChildren(), obj) {
            auto item = typeinfo_cast<CCNode*>(obj);
            if (item) item->setVisible(true);
        }
        if (auto bg = this->getChildByID("UtMenuBackground"_spr)) bg->setVisible(false);
        if (auto layer = this->getChildByID("UtMenuItemLayer"_spr)) layer->setVisible(false);
    }
};

class $modify(GameManager) {
    void fadeInMenuMusic() {
        auto setting = Mod::get()->getSettingValue("utMusic");
        if (setting && setting->get<bool>() && !getGameVariable("0122")) {
            FMODAudioEngine::sharedEngine()->playMusic("menu.ogg"_spr, true, 0.0f, 0);
        } else {
            GameManager::fadeInMenuMusic();
        }
    }

    void playMenuMusic() {
        auto setting = Mod::get()->getSettingValue("utMusic");
        if (setting && setting->get<bool>() && !getGameVariable("0122")) {
            FMODAudioEngine::sharedEngine()->playMusic("menu.ogg"_spr, true, 0.0f, 0);
        } else {
#ifdef GEODE_IS_MACOS
            GameManager::fadeInMenuMusic();
#else
            GameManager::playMenuMusic();
#endif
        }
    }
};

$execute {
    listenForSettingChanges("utMusic", +[](bool) {
        GameManager::sharedState()->fadeInMenuMusic();
    });
}

class $modify(LoadingLayer) {
    bool init(bool p0) {
        if (!LoadingLayer::init(p0)) return false;

        auto setting = Mod::get()->getSettingValue
alue("loadingBar");
        bool barVisible = setting && setting->get<bool>();

        CCObject* obj;
        CCARRAY_FOREACH(this->getChildren(), obj) {
            auto item = typeinfo_cast<CCNode*>(obj);
