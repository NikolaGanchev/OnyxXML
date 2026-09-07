#pragma once
#include <iostream>
#include <utility>

#include "onyx.h"

template <std::size_t N, typename F>
constexpr void forConstexpr(F&& f) {
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        (f.template operator()<Is>(), ...);
    }(std::make_index_sequence<N>{});
}

class IndexFixture : public onyx::dynamic::Node::Index {
   public:
    std::size_t putCalledCount = 0;
    std::size_t removeCalledCount = 0;
    std::size_t updateCalledCount = 0;

    ADD_INDEX_MOVE_OPERATIONS(public, IndexFixture,
                              &IndexFixture::updateCalledCount,
                              &IndexFixture::putCalledCount,
                              &IndexFixture::removeCalledCount);

    BEFRIEND_INDEX_CREATOR_FUNCTIONS;

   protected:
    bool putIfNeeded(onyx::dynamic::Node* node) override {
        putCalledCount++;
        return true;
    }
    bool removeIfNeeded(onyx::dynamic::Node* node) override {
        removeCalledCount++;
        return true;
    }
    bool update(onyx::dynamic::Node* node) override {
        updateCalledCount++;
        return true;
    }

    using onyx::dynamic::Node::Index::Index;
};

// Test fixture class to expose protected getFlag and setFlag methods
class FlagTestNode : public onyx::tags::GenericNode {
   public:
    FlagTestNode()
        : GenericNode(onyx::dynamic::NonOwning, "div", Type::NonVoid) {}

    template <std::size_t Bit>
    bool testGetFlag() const {
        return this->getFlag<Bit>();
    }

    template <std::size_t Bit>
    void testSetFlag(bool value) {
        this->setFlag<Bit>(value);
    }

    static consteval std::size_t testMaxFlagBits() {
        return GenericNode::maxFlagBits();
    }
};

class SaxListenerLogger : public virtual onyx::parser::SaxListener {
   private:
    int eventCount = 0;
    std::ostream& os;

   public:
    SaxListenerLogger(std::ostream& os) : os(os) {}

    void onStart() override {
        os << "Start\n";
        eventCount++;
    }

    void onText(std::string text) override {
        os << "Text: " << text << "\n";
        eventCount++;
    }

    void onComment(std::string text) override {
        os << "Comment: " << text << "\n";
        eventCount++;
    }

    void onCData(std::string text) override {
        os << "CData: " << text << "\n";
        eventCount++;
    }

    void onInstruction(std::string tag, std::string instruction) override {
        os << "Instruction Tag: " << tag << "\n\tInstruction: " << instruction
           << "\n";
        eventCount++;
    }

    void onTagOpen(std::string namespacePrefix, std::string name,
                   bool isSelfClosing,
                   std::vector<onyx::dynamic::Attribute> attributes) override {
        os << "Tag open: " << namespacePrefix << " " << name
           << "\n\tisSelfClosing: " << isSelfClosing << "\n";
        for (size_t i = 0; i < attributes.size(); i++) {
            os << "\tAttribute Name: " << attributes[i].getName()
               << " | Attribute Value: " << attributes[i].getValue() << "\n";
        }

        eventCount++;
    }

    void onTagClose(std::string namespacePrefix, std::string name) override {
        os << "Tag close: " << namespacePrefix << " " << name << "\n";

        eventCount++;
    }

    void onXMLDeclaration(std::string version, std::string encoding,
                          bool hasEncoding, bool isStandalone,
                          bool hasStandalone) override {
        os << "XML Declaration: \t" << "\tVersion: " << version
           << "\n\tEncoding: " << encoding
           << "\n\tisStandalone: " << isStandalone << "\n";
        eventCount++;
    }

    void onDoctype(std::string text) override {
        os << "Doctype: " << text << "\n";
        eventCount++;
    }

    void onException(std::exception& e) override {
        os << "Exception: " << e.what() << "\n";
        eventCount++;
    }

    void onEnd() override { eventCount++; }

    int getEventCount() { return eventCount; }
};