#pragma once
#include "document.h" 
#include "dynamic/tags.h" 

namespace Templater::compile::ctags {
    template <typename... Children>
    struct a {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("a");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::a> node = std::make_unique<Templater::dynamic::dtags::a>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct abbr {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("abbr");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::abbr> node = std::make_unique<Templater::dynamic::dtags::abbr>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct address {
        static consteval size_t size() {
            size_t size = 19;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("address");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::address> node = std::make_unique<Templater::dynamic::dtags::address>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct area {
        static consteval size_t size() {
            size_t size = 8;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("area");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::area> node = std::make_unique<Templater::dynamic::dtags::area>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct article {
        static consteval size_t size() {
            size_t size = 19;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("article");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::article> node = std::make_unique<Templater::dynamic::dtags::article>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct aside {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("aside");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::aside> node = std::make_unique<Templater::dynamic::dtags::aside>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct audio {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("audio");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::audio> node = std::make_unique<Templater::dynamic::dtags::audio>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct b {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("b");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::b> node = std::make_unique<Templater::dynamic::dtags::b>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct base {
        static consteval size_t size() {
            size_t size = 8;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("base");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::base> node = std::make_unique<Templater::dynamic::dtags::base>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct bdi {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("bdi");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::bdi> node = std::make_unique<Templater::dynamic::dtags::bdi>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct bdo {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("bdo");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::bdo> node = std::make_unique<Templater::dynamic::dtags::bdo>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct blockquote {
        static consteval size_t size() {
            size_t size = 25;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("blockquote");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::blockquote> node = std::make_unique<Templater::dynamic::dtags::blockquote>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct body {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("body");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::body> node = std::make_unique<Templater::dynamic::dtags::body>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct br {
        static consteval size_t size() {
            size_t size = 6;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("br");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::br> node = std::make_unique<Templater::dynamic::dtags::br>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct button {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("button");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::button> node = std::make_unique<Templater::dynamic::dtags::button>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct canvas {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("canvas");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::canvas> node = std::make_unique<Templater::dynamic::dtags::canvas>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct caption {
        static consteval size_t size() {
            size_t size = 19;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("caption");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::caption> node = std::make_unique<Templater::dynamic::dtags::caption>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct cite {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("cite");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::cite> node = std::make_unique<Templater::dynamic::dtags::cite>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct code {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("code");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::code> node = std::make_unique<Templater::dynamic::dtags::code>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct col {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("col");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::col> node = std::make_unique<Templater::dynamic::dtags::col>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct colgroup {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("colgroup");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::colgroup> node = std::make_unique<Templater::dynamic::dtags::colgroup>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct data {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("data");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::data> node = std::make_unique<Templater::dynamic::dtags::data>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct datalist {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("datalist");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::datalist> node = std::make_unique<Templater::dynamic::dtags::datalist>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dd {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("dd");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::dd> node = std::make_unique<Templater::dynamic::dtags::dd>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct del {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("del");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::del> node = std::make_unique<Templater::dynamic::dtags::del>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct details {
        static consteval size_t size() {
            size_t size = 19;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("details");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::details> node = std::make_unique<Templater::dynamic::dtags::details>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dfn {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("dfn");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::dfn> node = std::make_unique<Templater::dynamic::dtags::dfn>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dialog {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("dialog");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::dialog> node = std::make_unique<Templater::dynamic::dtags::dialog>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct cdiv {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("div");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::cdiv> node = std::make_unique<Templater::dynamic::dtags::cdiv>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dl {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("dl");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::dl> node = std::make_unique<Templater::dynamic::dtags::dl>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dt {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("dt");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::dt> node = std::make_unique<Templater::dynamic::dtags::dt>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct em {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("em");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::em> node = std::make_unique<Templater::dynamic::dtags::em>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct embed {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("embed");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::embed> node = std::make_unique<Templater::dynamic::dtags::embed>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct fieldset {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("fieldset");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::fieldset> node = std::make_unique<Templater::dynamic::dtags::fieldset>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct figcaption {
        static consteval size_t size() {
            size_t size = 25;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("figcaption");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::figcaption> node = std::make_unique<Templater::dynamic::dtags::figcaption>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct figure {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("figure");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::figure> node = std::make_unique<Templater::dynamic::dtags::figure>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct footer {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("footer");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::footer> node = std::make_unique<Templater::dynamic::dtags::footer>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct form {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("form");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::form> node = std::make_unique<Templater::dynamic::dtags::form>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h1 {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("h1");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::h1> node = std::make_unique<Templater::dynamic::dtags::h1>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h2 {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("h2");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::h2> node = std::make_unique<Templater::dynamic::dtags::h2>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h3 {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("h3");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::h3> node = std::make_unique<Templater::dynamic::dtags::h3>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h4 {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("h4");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::h4> node = std::make_unique<Templater::dynamic::dtags::h4>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h5 {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("h5");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::h5> node = std::make_unique<Templater::dynamic::dtags::h5>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h6 {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("h6");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::h6> node = std::make_unique<Templater::dynamic::dtags::h6>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct head {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("head");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::head> node = std::make_unique<Templater::dynamic::dtags::head>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct header {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("header");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::header> node = std::make_unique<Templater::dynamic::dtags::header>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct hgroup {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("hgroup");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::hgroup> node = std::make_unique<Templater::dynamic::dtags::hgroup>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct hr {
        static consteval size_t size() {
            size_t size = 6;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("hr");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::hr> node = std::make_unique<Templater::dynamic::dtags::hr>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct html {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("html");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::html> node = std::make_unique<Templater::dynamic::dtags::html>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct i {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("i");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::i> node = std::make_unique<Templater::dynamic::dtags::i>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct iframe {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("iframe");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::iframe> node = std::make_unique<Templater::dynamic::dtags::iframe>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct img {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("img");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::img> node = std::make_unique<Templater::dynamic::dtags::img>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct input {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("input");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::input> node = std::make_unique<Templater::dynamic::dtags::input>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ins {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("ins");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::ins> node = std::make_unique<Templater::dynamic::dtags::ins>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct kbd {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("kbd");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::kbd> node = std::make_unique<Templater::dynamic::dtags::kbd>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct label {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("label");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::label> node = std::make_unique<Templater::dynamic::dtags::label>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct legend {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("legend");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::legend> node = std::make_unique<Templater::dynamic::dtags::legend>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct li {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("li");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::li> node = std::make_unique<Templater::dynamic::dtags::li>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct clink {
        static consteval size_t size() {
            size_t size = 8;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("link");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::clink> node = std::make_unique<Templater::dynamic::dtags::clink>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct main {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("main");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::main> node = std::make_unique<Templater::dynamic::dtags::main>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct map {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("map");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::map> node = std::make_unique<Templater::dynamic::dtags::map>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct mark {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("mark");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::mark> node = std::make_unique<Templater::dynamic::dtags::mark>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct math {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("math");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::math> node = std::make_unique<Templater::dynamic::dtags::math>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct menu {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("menu");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::menu> node = std::make_unique<Templater::dynamic::dtags::menu>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct menuitem {
        static consteval size_t size() {
            size_t size = 12;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("menuitem");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::menuitem> node = std::make_unique<Templater::dynamic::dtags::menuitem>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct meta {
        static consteval size_t size() {
            size_t size = 8;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("meta");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::meta> node = std::make_unique<Templater::dynamic::dtags::meta>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct meter {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("meter");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::meter> node = std::make_unique<Templater::dynamic::dtags::meter>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct nav {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("nav");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::nav> node = std::make_unique<Templater::dynamic::dtags::nav>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct noscript {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("noscript");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::noscript> node = std::make_unique<Templater::dynamic::dtags::noscript>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct object {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("object");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::object> node = std::make_unique<Templater::dynamic::dtags::object>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ol {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("ol");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::ol> node = std::make_unique<Templater::dynamic::dtags::ol>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct optgroup {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("optgroup");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::optgroup> node = std::make_unique<Templater::dynamic::dtags::optgroup>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct option {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("option");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::option> node = std::make_unique<Templater::dynamic::dtags::option>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct output {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("output");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::output> node = std::make_unique<Templater::dynamic::dtags::output>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct p {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("p");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::p> node = std::make_unique<Templater::dynamic::dtags::p>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct param {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("param");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::param> node = std::make_unique<Templater::dynamic::dtags::param>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct picture {
        static consteval size_t size() {
            size_t size = 19;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("picture");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::picture> node = std::make_unique<Templater::dynamic::dtags::picture>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct pre {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("pre");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::pre> node = std::make_unique<Templater::dynamic::dtags::pre>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct progress {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("progress");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::progress> node = std::make_unique<Templater::dynamic::dtags::progress>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct q {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("q");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::q> node = std::make_unique<Templater::dynamic::dtags::q>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rb {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("rb");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::rb> node = std::make_unique<Templater::dynamic::dtags::rb>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rp {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("rp");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::rp> node = std::make_unique<Templater::dynamic::dtags::rp>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rt {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("rt");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::rt> node = std::make_unique<Templater::dynamic::dtags::rt>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rtc {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("rtc");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::rtc> node = std::make_unique<Templater::dynamic::dtags::rtc>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ruby {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("ruby");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::ruby> node = std::make_unique<Templater::dynamic::dtags::ruby>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct s {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("s");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::s> node = std::make_unique<Templater::dynamic::dtags::s>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct samp {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("samp");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::samp> node = std::make_unique<Templater::dynamic::dtags::samp>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct script {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("script");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::script> node = std::make_unique<Templater::dynamic::dtags::script>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct search {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("search");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::search> node = std::make_unique<Templater::dynamic::dtags::search>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct section {
        static consteval size_t size() {
            size_t size = 19;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("section");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::section> node = std::make_unique<Templater::dynamic::dtags::section>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct select {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("select");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::select> node = std::make_unique<Templater::dynamic::dtags::select>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct slot {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("slot");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::slot> node = std::make_unique<Templater::dynamic::dtags::slot>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct small {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("small");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::small> node = std::make_unique<Templater::dynamic::dtags::small>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct source {
        static consteval size_t size() {
            size_t size = 10;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("source");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::source> node = std::make_unique<Templater::dynamic::dtags::source>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct span {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("span");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::span> node = std::make_unique<Templater::dynamic::dtags::span>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct strong {
        static consteval size_t size() {
            size_t size = 17;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("strong");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::strong> node = std::make_unique<Templater::dynamic::dtags::strong>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct style {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("style");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::style> node = std::make_unique<Templater::dynamic::dtags::style>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct sub {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("sub");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::sub> node = std::make_unique<Templater::dynamic::dtags::sub>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct summary {
        static consteval size_t size() {
            size_t size = 19;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("summary");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::summary> node = std::make_unique<Templater::dynamic::dtags::summary>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct sup {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("sup");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::sup> node = std::make_unique<Templater::dynamic::dtags::sup>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct svg {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("svg");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::svg> node = std::make_unique<Templater::dynamic::dtags::svg>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct table {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("table");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::table> node = std::make_unique<Templater::dynamic::dtags::table>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct tbody {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("tbody");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::tbody> node = std::make_unique<Templater::dynamic::dtags::tbody>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct td {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("td");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::td> node = std::make_unique<Templater::dynamic::dtags::td>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ctemplate {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("template");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::ctemplate> node = std::make_unique<Templater::dynamic::dtags::ctemplate>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct textarea {
        static consteval size_t size() {
            size_t size = 21;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("textarea");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::textarea> node = std::make_unique<Templater::dynamic::dtags::textarea>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct tfoot {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("tfoot");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::tfoot> node = std::make_unique<Templater::dynamic::dtags::tfoot>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct th {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("th");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::th> node = std::make_unique<Templater::dynamic::dtags::th>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct thead {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("thead");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::thead> node = std::make_unique<Templater::dynamic::dtags::thead>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct time {
        static consteval size_t size() {
            size_t size = 13;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("time");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::time> node = std::make_unique<Templater::dynamic::dtags::time>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct title {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("title");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::title> node = std::make_unique<Templater::dynamic::dtags::title>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct tr {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("tr");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::tr> node = std::make_unique<Templater::dynamic::dtags::tr>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct track {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("track");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::track> node = std::make_unique<Templater::dynamic::dtags::track>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct u {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("u");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::u> node = std::make_unique<Templater::dynamic::dtags::u>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ul {
        static consteval size_t size() {
            size_t size = 9;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("ul");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::ul> node = std::make_unique<Templater::dynamic::dtags::ul>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct var {
        static consteval size_t size() {
            size_t size = 11;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("var");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::var> node = std::make_unique<Templater::dynamic::dtags::var>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct video {
        static consteval size_t size() {
            size_t size = 15;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseNode<size(), Children...>("video");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::video> node = std::make_unique<Templater::dynamic::dtags::video>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
    template <typename... Children>
    struct wbr {
        static consteval size_t size() {
            size_t size = 7;
            ((size += Children::size()), ...);
            return size;
        }
        static consteval std::array<char, size() + 1> serialise() {
            return serialiseVoidNode<size(), Children...>("wbr");
        }
        static std::unique_ptr<Templater::dynamic::Node> value() {
            std::unique_ptr<Templater::dynamic::dtags::wbr> node = std::make_unique<Templater::dynamic::dtags::wbr>();
            (parseChildren<Children>(node.get()), ...);
            return node;
        }
    };
}

