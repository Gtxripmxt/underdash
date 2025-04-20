#pragma once
#include <Geode/loader/CustomSetting.hpp>
using namespace geode::prelude;

class HeaderValue;

class HeaderNode : public CustomSettingNode {
protected:
    bool init(HeaderValue* value, float width);

public:
    static HeaderNode* create(HeaderValue* value, float width);

    void commit() override {}
    void resetToDefault() override {}
    bool hasUncommittedChanges() const override { return false; }
    bool hasNonDefaultValue() const override { return true; }
};

class HeaderValue : public CustomSettingValue {
private:
    int m_something;

public:
    HeaderValue(std::string const& key, std::string const& modID, int something)
        : CustomSettingValue(key, modID), m_something(something) {}

    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }

    CustomSettingNode* createNode(float width) override {
        return HeaderNode::create(this, width);
    }
};

bool HeaderNode::init(HeaderValue* value, float width) {
    if (!CustomSettingNode::init(value, width))
        return false;

    this->setContentSize({ width, 40.f });

    auto settingDef = Mod::get()->getSettingDefinition(value->getKey());
    std::string sectionName = "Header";
    if (settingDef && settingDef->json->contains("name")) {
        sectionName = settingDef->json->get<std::string>("name");
    }

    auto label = CCLabelBMFont::create(sectionName.c_str(), "utFont.fnt"_spr);
    label->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
    label->setColor({ 242, 242, 0 });
    this->addChild(label);

    return true;
}

HeaderNode* HeaderNode::create(HeaderValue* value, float width) {
    auto ret = new HeaderNode;
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
