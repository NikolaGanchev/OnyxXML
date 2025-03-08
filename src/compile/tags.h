#pragma once
#include "document.h" 
#include "dynamic/tags.h" 

namespace Templater::compile::ctags {
    template <typename... Children>
    struct a {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::a node = Templater::dynamic::dtags::a();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct abbr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::abbr node = Templater::dynamic::dtags::abbr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct address {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::address node = Templater::dynamic::dtags::address();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct area {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::area node = Templater::dynamic::dtags::area();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct article {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::article node = Templater::dynamic::dtags::article();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct aside {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::aside node = Templater::dynamic::dtags::aside();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct audio {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::audio node = Templater::dynamic::dtags::audio();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct b {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::b node = Templater::dynamic::dtags::b();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct base {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::base node = Templater::dynamic::dtags::base();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct bdi {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::bdi node = Templater::dynamic::dtags::bdi();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct bdo {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::bdo node = Templater::dynamic::dtags::bdo();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct blockquote {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::blockquote node = Templater::dynamic::dtags::blockquote();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct body {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::body node = Templater::dynamic::dtags::body();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct br {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::br node = Templater::dynamic::dtags::br();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct button {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::button node = Templater::dynamic::dtags::button();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct canvas {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::canvas node = Templater::dynamic::dtags::canvas();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct caption {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::caption node = Templater::dynamic::dtags::caption();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct cite {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::cite node = Templater::dynamic::dtags::cite();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct code {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::code node = Templater::dynamic::dtags::code();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct col {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::col node = Templater::dynamic::dtags::col();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct colgroup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::colgroup node = Templater::dynamic::dtags::colgroup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct data {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::data node = Templater::dynamic::dtags::data();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct datalist {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::datalist node = Templater::dynamic::dtags::datalist();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dd {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::dd node = Templater::dynamic::dtags::dd();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct del {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::del node = Templater::dynamic::dtags::del();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct details {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::details node = Templater::dynamic::dtags::details();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dfn {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::dfn node = Templater::dynamic::dtags::dfn();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dialog {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::dialog node = Templater::dynamic::dtags::dialog();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct cdiv {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::div node = Templater::dynamic::dtags::div();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dl {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::dl node = Templater::dynamic::dtags::dl();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dt {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::dt node = Templater::dynamic::dtags::dt();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct em {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::em node = Templater::dynamic::dtags::em();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct embed {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::embed node = Templater::dynamic::dtags::embed();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct fieldset {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::fieldset node = Templater::dynamic::dtags::fieldset();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct figcaption {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::figcaption node = Templater::dynamic::dtags::figcaption();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct figure {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::figure node = Templater::dynamic::dtags::figure();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct footer {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::footer node = Templater::dynamic::dtags::footer();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct form {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::form node = Templater::dynamic::dtags::form();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h1 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::h1 node = Templater::dynamic::dtags::h1();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h2 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::h2 node = Templater::dynamic::dtags::h2();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h3 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::h3 node = Templater::dynamic::dtags::h3();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h4 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::h4 node = Templater::dynamic::dtags::h4();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h5 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::h5 node = Templater::dynamic::dtags::h5();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h6 {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::h6 node = Templater::dynamic::dtags::h6();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct head {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::head node = Templater::dynamic::dtags::head();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct header {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::header node = Templater::dynamic::dtags::header();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct hgroup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::hgroup node = Templater::dynamic::dtags::hgroup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct hr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::hr node = Templater::dynamic::dtags::hr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct html {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::html node = Templater::dynamic::dtags::html();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct i {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::i node = Templater::dynamic::dtags::i();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct iframe {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::iframe node = Templater::dynamic::dtags::iframe();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct img {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::img node = Templater::dynamic::dtags::img();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct input {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::input node = Templater::dynamic::dtags::input();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ins {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::ins node = Templater::dynamic::dtags::ins();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct kbd {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::kbd node = Templater::dynamic::dtags::kbd();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct label {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::label node = Templater::dynamic::dtags::label();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct legend {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::legend node = Templater::dynamic::dtags::legend();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct li {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::li node = Templater::dynamic::dtags::li();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct link {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::link node = Templater::dynamic::dtags::link();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct main {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::main node = Templater::dynamic::dtags::main();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct map {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::map node = Templater::dynamic::dtags::map();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct mark {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::mark node = Templater::dynamic::dtags::mark();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct math {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::math node = Templater::dynamic::dtags::math();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct menu {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::menu node = Templater::dynamic::dtags::menu();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct menuitem {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::menuitem node = Templater::dynamic::dtags::menuitem();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct meta {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::meta node = Templater::dynamic::dtags::meta();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct meter {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::meter node = Templater::dynamic::dtags::meter();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct nav {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::nav node = Templater::dynamic::dtags::nav();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct noscript {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::noscript node = Templater::dynamic::dtags::noscript();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct object {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::object node = Templater::dynamic::dtags::object();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ol {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::ol node = Templater::dynamic::dtags::ol();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct optgroup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::optgroup node = Templater::dynamic::dtags::optgroup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct option {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::option node = Templater::dynamic::dtags::option();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct output {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::output node = Templater::dynamic::dtags::output();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct p {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::p node = Templater::dynamic::dtags::p();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct param {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::param node = Templater::dynamic::dtags::param();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct picture {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::picture node = Templater::dynamic::dtags::picture();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct pre {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::pre node = Templater::dynamic::dtags::pre();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct progress {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::progress node = Templater::dynamic::dtags::progress();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct q {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::q node = Templater::dynamic::dtags::q();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rb {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::rb node = Templater::dynamic::dtags::rb();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rp {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::rp node = Templater::dynamic::dtags::rp();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rt {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::rt node = Templater::dynamic::dtags::rt();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rtc {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::rtc node = Templater::dynamic::dtags::rtc();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ruby {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::ruby node = Templater::dynamic::dtags::ruby();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct s {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::s node = Templater::dynamic::dtags::s();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct samp {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::samp node = Templater::dynamic::dtags::samp();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct script {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::script node = Templater::dynamic::dtags::script();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct search {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::search node = Templater::dynamic::dtags::search();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct section {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::section node = Templater::dynamic::dtags::section();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct select {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::select node = Templater::dynamic::dtags::select();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct slot {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::slot node = Templater::dynamic::dtags::slot();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct small {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::small node = Templater::dynamic::dtags::small();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct source {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::source node = Templater::dynamic::dtags::source();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct span {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::span node = Templater::dynamic::dtags::span();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct strong {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::strong node = Templater::dynamic::dtags::strong();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct style {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::style node = Templater::dynamic::dtags::style();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct sub {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::sub node = Templater::dynamic::dtags::sub();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct summary {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::summary node = Templater::dynamic::dtags::summary();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct sup {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::sup node = Templater::dynamic::dtags::sup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct svg {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::svg node = Templater::dynamic::dtags::svg();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct table {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::table node = Templater::dynamic::dtags::table();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct tbody {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::tbody node = Templater::dynamic::dtags::tbody();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct td {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::td node = Templater::dynamic::dtags::td();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ctemplate {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::ctemplate node = Templater::dynamic::dtags::ctemplate();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct textarea {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::textarea node = Templater::dynamic::dtags::textarea();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct tfoot {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::tfoot node = Templater::dynamic::dtags::tfoot();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct th {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::th node = Templater::dynamic::dtags::th();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct thead {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::thead node = Templater::dynamic::dtags::thead();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct time {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::time node = Templater::dynamic::dtags::time();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct title {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::title node = Templater::dynamic::dtags::title();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct tr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::tr node = Templater::dynamic::dtags::tr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct track {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::track node = Templater::dynamic::dtags::track();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct u {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::u node = Templater::dynamic::dtags::u();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ul {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::ul node = Templater::dynamic::dtags::ul();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct var {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::var node = Templater::dynamic::dtags::var();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct video {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::video node = Templater::dynamic::dtags::video();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct wbr {
        static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
            Templater::dynamic::dtags::wbr node = Templater::dynamic::dtags::wbr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
}

