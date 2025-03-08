#pragma once
#include "document.h" 
#include "dynamic/tags.h" 

namespace Templater::html::ctags {
    template <typename... Children>
    struct a {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::a node = Templater::html::dtags::a();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct abbr {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::abbr node = Templater::html::dtags::abbr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct address {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::address node = Templater::html::dtags::address();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct area {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::area node = Templater::html::dtags::area();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct article {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::article node = Templater::html::dtags::article();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct aside {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::aside node = Templater::html::dtags::aside();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct audio {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::audio node = Templater::html::dtags::audio();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct b {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::b node = Templater::html::dtags::b();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct base {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::base node = Templater::html::dtags::base();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct bdi {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::bdi node = Templater::html::dtags::bdi();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct bdo {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::bdo node = Templater::html::dtags::bdo();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct blockquote {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::blockquote node = Templater::html::dtags::blockquote();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct body {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::body node = Templater::html::dtags::body();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct br {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::br node = Templater::html::dtags::br();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct button {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::button node = Templater::html::dtags::button();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct canvas {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::canvas node = Templater::html::dtags::canvas();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct caption {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::caption node = Templater::html::dtags::caption();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct cite {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::cite node = Templater::html::dtags::cite();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct code {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::code node = Templater::html::dtags::code();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct col {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::col node = Templater::html::dtags::col();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct colgroup {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::colgroup node = Templater::html::dtags::colgroup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct data {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::data node = Templater::html::dtags::data();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct datalist {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::datalist node = Templater::html::dtags::datalist();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dd {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::dd node = Templater::html::dtags::dd();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct del {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::del node = Templater::html::dtags::del();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct details {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::details node = Templater::html::dtags::details();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dfn {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::dfn node = Templater::html::dtags::dfn();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dialog {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::dialog node = Templater::html::dtags::dialog();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct div {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::div node = Templater::html::dtags::div();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dl {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::dl node = Templater::html::dtags::dl();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct dt {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::dt node = Templater::html::dtags::dt();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct em {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::em node = Templater::html::dtags::em();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct embed {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::embed node = Templater::html::dtags::embed();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct fieldset {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::fieldset node = Templater::html::dtags::fieldset();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct figcaption {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::figcaption node = Templater::html::dtags::figcaption();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct figure {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::figure node = Templater::html::dtags::figure();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct footer {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::footer node = Templater::html::dtags::footer();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct form {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::form node = Templater::html::dtags::form();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h1 {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::h1 node = Templater::html::dtags::h1();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h2 {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::h2 node = Templater::html::dtags::h2();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h3 {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::h3 node = Templater::html::dtags::h3();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h4 {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::h4 node = Templater::html::dtags::h4();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h5 {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::h5 node = Templater::html::dtags::h5();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct h6 {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::h6 node = Templater::html::dtags::h6();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct head {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::head node = Templater::html::dtags::head();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct header {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::header node = Templater::html::dtags::header();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct hgroup {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::hgroup node = Templater::html::dtags::hgroup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct hr {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::hr node = Templater::html::dtags::hr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct html {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::html node = Templater::html::dtags::html();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct i {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::i node = Templater::html::dtags::i();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct iframe {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::iframe node = Templater::html::dtags::iframe();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct img {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::img node = Templater::html::dtags::img();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct input {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::input node = Templater::html::dtags::input();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ins {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::ins node = Templater::html::dtags::ins();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct kbd {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::kbd node = Templater::html::dtags::kbd();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct label {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::label node = Templater::html::dtags::label();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct legend {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::legend node = Templater::html::dtags::legend();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct li {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::li node = Templater::html::dtags::li();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct link {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::link node = Templater::html::dtags::link();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct main {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::main node = Templater::html::dtags::main();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct map {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::map node = Templater::html::dtags::map();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct mark {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::mark node = Templater::html::dtags::mark();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct math {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::math node = Templater::html::dtags::math();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct menu {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::menu node = Templater::html::dtags::menu();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct menuitem {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::menuitem node = Templater::html::dtags::menuitem();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct meta {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::meta node = Templater::html::dtags::meta();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct meter {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::meter node = Templater::html::dtags::meter();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct nav {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::nav node = Templater::html::dtags::nav();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct noscript {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::noscript node = Templater::html::dtags::noscript();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct object {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::object node = Templater::html::dtags::object();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ol {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::ol node = Templater::html::dtags::ol();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct optgroup {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::optgroup node = Templater::html::dtags::optgroup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct option {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::option node = Templater::html::dtags::option();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct output {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::output node = Templater::html::dtags::output();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct p {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::p node = Templater::html::dtags::p();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct param {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::param node = Templater::html::dtags::param();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct picture {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::picture node = Templater::html::dtags::picture();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct pre {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::pre node = Templater::html::dtags::pre();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct progress {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::progress node = Templater::html::dtags::progress();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct q {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::q node = Templater::html::dtags::q();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rb {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::rb node = Templater::html::dtags::rb();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rp {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::rp node = Templater::html::dtags::rp();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rt {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::rt node = Templater::html::dtags::rt();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct rtc {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::rtc node = Templater::html::dtags::rtc();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ruby {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::ruby node = Templater::html::dtags::ruby();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct s {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::s node = Templater::html::dtags::s();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct samp {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::samp node = Templater::html::dtags::samp();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct script {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::script node = Templater::html::dtags::script();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct search {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::search node = Templater::html::dtags::search();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct section {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::section node = Templater::html::dtags::section();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct select {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::select node = Templater::html::dtags::select();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct slot {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::slot node = Templater::html::dtags::slot();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct small {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::small node = Templater::html::dtags::small();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct source {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::source node = Templater::html::dtags::source();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct span {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::span node = Templater::html::dtags::span();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct strong {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::strong node = Templater::html::dtags::strong();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct style {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::style node = Templater::html::dtags::style();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct sub {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::sub node = Templater::html::dtags::sub();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct summary {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::summary node = Templater::html::dtags::summary();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct sup {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::sup node = Templater::html::dtags::sup();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct svg {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::svg node = Templater::html::dtags::svg();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct table {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::table node = Templater::html::dtags::table();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct tbody {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::tbody node = Templater::html::dtags::tbody();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct td {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::td node = Templater::html::dtags::td();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct htemplate {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::htemplate node = Templater::html::dtags::htemplate();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct textarea {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::textarea node = Templater::html::dtags::textarea();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct tfoot {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::tfoot node = Templater::html::dtags::tfoot();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct th {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::th node = Templater::html::dtags::th();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct thead {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::thead node = Templater::html::dtags::thead();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct time {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::time node = Templater::html::dtags::time();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct title {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::title node = Templater::html::dtags::title();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct tr {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::tr node = Templater::html::dtags::tr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct track {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::track node = Templater::html::dtags::track();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct u {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::u node = Templater::html::dtags::u();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct ul {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::ul node = Templater::html::dtags::ul();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct var {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::var node = Templater::html::dtags::var();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct video {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::video node = Templater::html::dtags::video();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
    template <typename... Children>
    struct wbr {
        static constexpr std::shared_ptr<Object> value() {
            Templater::html::dtags::wbr node = Templater::html::dtags::wbr();
            (parseChildren<Children>(node), ...);
            return node.clone();
        }
    };
}

