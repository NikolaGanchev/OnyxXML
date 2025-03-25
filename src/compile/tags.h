#pragma once
#include "document.h" 
#include "dynamic/tags.h" 

namespace Templater::compile::ctags {
    template <typename... Children>
    struct a {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::a> node = std::make_shared<Templater::dynamic::dtags::a>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct abbr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::abbr> node = std::make_shared<Templater::dynamic::dtags::abbr>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct address {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::address> node = std::make_shared<Templater::dynamic::dtags::address>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct area {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::area> node = std::make_shared<Templater::dynamic::dtags::area>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct article {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::article> node = std::make_shared<Templater::dynamic::dtags::article>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct aside {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::aside> node = std::make_shared<Templater::dynamic::dtags::aside>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct audio {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::audio> node = std::make_shared<Templater::dynamic::dtags::audio>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct b {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::b> node = std::make_shared<Templater::dynamic::dtags::b>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct base {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::base> node = std::make_shared<Templater::dynamic::dtags::base>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct bdi {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::bdi> node = std::make_shared<Templater::dynamic::dtags::bdi>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct bdo {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::bdo> node = std::make_shared<Templater::dynamic::dtags::bdo>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct blockquote {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::blockquote> node = std::make_shared<Templater::dynamic::dtags::blockquote>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct body {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::body> node = std::make_shared<Templater::dynamic::dtags::body>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct br {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::br> node = std::make_shared<Templater::dynamic::dtags::br>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct button {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::button> node = std::make_shared<Templater::dynamic::dtags::button>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct canvas {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::canvas> node = std::make_shared<Templater::dynamic::dtags::canvas>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct caption {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::caption> node = std::make_shared<Templater::dynamic::dtags::caption>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct cite {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::cite> node = std::make_shared<Templater::dynamic::dtags::cite>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct code {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::code> node = std::make_shared<Templater::dynamic::dtags::code>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct col {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::col> node = std::make_shared<Templater::dynamic::dtags::col>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct colgroup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::colgroup> node = std::make_shared<Templater::dynamic::dtags::colgroup>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct data {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::data> node = std::make_shared<Templater::dynamic::dtags::data>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct datalist {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::datalist> node = std::make_shared<Templater::dynamic::dtags::datalist>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dd {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::dd> node = std::make_shared<Templater::dynamic::dtags::dd>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct del {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::del> node = std::make_shared<Templater::dynamic::dtags::del>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct details {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::details> node = std::make_shared<Templater::dynamic::dtags::details>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dfn {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::dfn> node = std::make_shared<Templater::dynamic::dtags::dfn>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dialog {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::dialog> node = std::make_shared<Templater::dynamic::dtags::dialog>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct cdiv {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::div> node = std::make_shared<Templater::dynamic::dtags::div>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dl {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::dl> node = std::make_shared<Templater::dynamic::dtags::dl>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct dt {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::dt> node = std::make_shared<Templater::dynamic::dtags::dt>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct em {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::em> node = std::make_shared<Templater::dynamic::dtags::em>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct embed {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::embed> node = std::make_shared<Templater::dynamic::dtags::embed>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct fieldset {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::fieldset> node = std::make_shared<Templater::dynamic::dtags::fieldset>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct figcaption {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::figcaption> node = std::make_shared<Templater::dynamic::dtags::figcaption>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct figure {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::figure> node = std::make_shared<Templater::dynamic::dtags::figure>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct footer {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::footer> node = std::make_shared<Templater::dynamic::dtags::footer>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct form {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::form> node = std::make_shared<Templater::dynamic::dtags::form>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h1 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::h1> node = std::make_shared<Templater::dynamic::dtags::h1>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h2 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::h2> node = std::make_shared<Templater::dynamic::dtags::h2>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h3 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::h3> node = std::make_shared<Templater::dynamic::dtags::h3>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h4 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::h4> node = std::make_shared<Templater::dynamic::dtags::h4>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h5 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::h5> node = std::make_shared<Templater::dynamic::dtags::h5>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct h6 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::h6> node = std::make_shared<Templater::dynamic::dtags::h6>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct head {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::head> node = std::make_shared<Templater::dynamic::dtags::head>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct header {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::header> node = std::make_shared<Templater::dynamic::dtags::header>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct hgroup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::hgroup> node = std::make_shared<Templater::dynamic::dtags::hgroup>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct hr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::hr> node = std::make_shared<Templater::dynamic::dtags::hr>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct html {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::html> node = std::make_shared<Templater::dynamic::dtags::html>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct i {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::i> node = std::make_shared<Templater::dynamic::dtags::i>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct iframe {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::iframe> node = std::make_shared<Templater::dynamic::dtags::iframe>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct img {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::img> node = std::make_shared<Templater::dynamic::dtags::img>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct input {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::input> node = std::make_shared<Templater::dynamic::dtags::input>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ins {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::ins> node = std::make_shared<Templater::dynamic::dtags::ins>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct kbd {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::kbd> node = std::make_shared<Templater::dynamic::dtags::kbd>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct label {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::label> node = std::make_shared<Templater::dynamic::dtags::label>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct legend {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::legend> node = std::make_shared<Templater::dynamic::dtags::legend>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct li {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::li> node = std::make_shared<Templater::dynamic::dtags::li>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct link {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::link> node = std::make_shared<Templater::dynamic::dtags::link>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct main {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::main> node = std::make_shared<Templater::dynamic::dtags::main>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct map {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::map> node = std::make_shared<Templater::dynamic::dtags::map>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct mark {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::mark> node = std::make_shared<Templater::dynamic::dtags::mark>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct math {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::math> node = std::make_shared<Templater::dynamic::dtags::math>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct menu {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::menu> node = std::make_shared<Templater::dynamic::dtags::menu>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct menuitem {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::menuitem> node = std::make_shared<Templater::dynamic::dtags::menuitem>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct meta {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::meta> node = std::make_shared<Templater::dynamic::dtags::meta>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct meter {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::meter> node = std::make_shared<Templater::dynamic::dtags::meter>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct nav {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::nav> node = std::make_shared<Templater::dynamic::dtags::nav>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct noscript {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::noscript> node = std::make_shared<Templater::dynamic::dtags::noscript>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct object {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::object> node = std::make_shared<Templater::dynamic::dtags::object>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ol {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::ol> node = std::make_shared<Templater::dynamic::dtags::ol>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct optgroup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::optgroup> node = std::make_shared<Templater::dynamic::dtags::optgroup>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct option {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::option> node = std::make_shared<Templater::dynamic::dtags::option>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct output {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::output> node = std::make_shared<Templater::dynamic::dtags::output>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct p {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::p> node = std::make_shared<Templater::dynamic::dtags::p>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct param {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::param> node = std::make_shared<Templater::dynamic::dtags::param>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct picture {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::picture> node = std::make_shared<Templater::dynamic::dtags::picture>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct pre {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::pre> node = std::make_shared<Templater::dynamic::dtags::pre>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct progress {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::progress> node = std::make_shared<Templater::dynamic::dtags::progress>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct q {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::q> node = std::make_shared<Templater::dynamic::dtags::q>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rb {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::rb> node = std::make_shared<Templater::dynamic::dtags::rb>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rp {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::rp> node = std::make_shared<Templater::dynamic::dtags::rp>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rt {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::rt> node = std::make_shared<Templater::dynamic::dtags::rt>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct rtc {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::rtc> node = std::make_shared<Templater::dynamic::dtags::rtc>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ruby {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::ruby> node = std::make_shared<Templater::dynamic::dtags::ruby>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct s {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::s> node = std::make_shared<Templater::dynamic::dtags::s>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct samp {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::samp> node = std::make_shared<Templater::dynamic::dtags::samp>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct script {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::script> node = std::make_shared<Templater::dynamic::dtags::script>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct search {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::search> node = std::make_shared<Templater::dynamic::dtags::search>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct section {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::section> node = std::make_shared<Templater::dynamic::dtags::section>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct select {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::select> node = std::make_shared<Templater::dynamic::dtags::select>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct slot {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::slot> node = std::make_shared<Templater::dynamic::dtags::slot>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct small {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::small> node = std::make_shared<Templater::dynamic::dtags::small>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct source {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::source> node = std::make_shared<Templater::dynamic::dtags::source>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct span {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::span> node = std::make_shared<Templater::dynamic::dtags::span>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct strong {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::strong> node = std::make_shared<Templater::dynamic::dtags::strong>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct style {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::style> node = std::make_shared<Templater::dynamic::dtags::style>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct sub {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::sub> node = std::make_shared<Templater::dynamic::dtags::sub>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct summary {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::summary> node = std::make_shared<Templater::dynamic::dtags::summary>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct sup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::sup> node = std::make_shared<Templater::dynamic::dtags::sup>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct svg {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::svg> node = std::make_shared<Templater::dynamic::dtags::svg>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct table {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::table> node = std::make_shared<Templater::dynamic::dtags::table>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct tbody {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::tbody> node = std::make_shared<Templater::dynamic::dtags::tbody>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct td {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::td> node = std::make_shared<Templater::dynamic::dtags::td>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ctemplate {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::ctemplate> node = std::make_shared<Templater::dynamic::dtags::ctemplate>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct textarea {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::textarea> node = std::make_shared<Templater::dynamic::dtags::textarea>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct tfoot {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::tfoot> node = std::make_shared<Templater::dynamic::dtags::tfoot>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct th {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::th> node = std::make_shared<Templater::dynamic::dtags::th>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct thead {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::thead> node = std::make_shared<Templater::dynamic::dtags::thead>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct time {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::time> node = std::make_shared<Templater::dynamic::dtags::time>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct title {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::title> node = std::make_shared<Templater::dynamic::dtags::title>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct tr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::tr> node = std::make_shared<Templater::dynamic::dtags::tr>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct track {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::track> node = std::make_shared<Templater::dynamic::dtags::track>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct u {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::u> node = std::make_shared<Templater::dynamic::dtags::u>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct ul {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::ul> node = std::make_shared<Templater::dynamic::dtags::ul>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct var {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::var> node = std::make_shared<Templater::dynamic::dtags::var>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct video {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::video> node = std::make_shared<Templater::dynamic::dtags::video>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
    template <typename... Children>
    struct wbr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            std::shared_ptr<Templater::dynamic::dtags::wbr> node = std::make_shared<Templater::dynamic::dtags::wbr>();
            (parseChildren<Children>(node), ...);
            return node;
        }
    };
}

