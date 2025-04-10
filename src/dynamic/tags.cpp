#include "tags.h"

namespace Templater::dynamic::dtags {
    const std::string& a::getTagName() const {
        static const std::string name = "a";
        return name;
    }
    std::unique_ptr<Node> a::shallowCopy() const {
        return std::make_unique<a>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool a::isVoid() const {
        return 0;
    }
    const std::string& abbr::getTagName() const {
        static const std::string name = "abbr";
        return name;
    }
    std::unique_ptr<Node> abbr::shallowCopy() const {
        return std::make_unique<abbr>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool abbr::isVoid() const {
        return 0;
    }
    const std::string& address::getTagName() const {
        static const std::string name = "address";
        return name;
    }
    std::unique_ptr<Node> address::shallowCopy() const {
        return std::make_unique<address>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool address::isVoid() const {
        return 0;
    }
    const std::string& area::getTagName() const {
        static const std::string name = "area";
        return name;
    }
    std::unique_ptr<Node> area::shallowCopy() const {
        return std::make_unique<area>(this->getAttributes());
    }
    const std::string& article::getTagName() const {
        static const std::string name = "article";
        return name;
    }
    std::unique_ptr<Node> article::shallowCopy() const {
        return std::make_unique<article>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool article::isVoid() const {
        return 0;
    }
    const std::string& aside::getTagName() const {
        static const std::string name = "aside";
        return name;
    }
    std::unique_ptr<Node> aside::shallowCopy() const {
        return std::make_unique<aside>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool aside::isVoid() const {
        return 0;
    }
    const std::string& audio::getTagName() const {
        static const std::string name = "audio";
        return name;
    }
    std::unique_ptr<Node> audio::shallowCopy() const {
        return std::make_unique<audio>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool audio::isVoid() const {
        return 0;
    }
    const std::string& b::getTagName() const {
        static const std::string name = "b";
        return name;
    }
    std::unique_ptr<Node> b::shallowCopy() const {
        return std::make_unique<b>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool b::isVoid() const {
        return 0;
    }
    const std::string& base::getTagName() const {
        static const std::string name = "base";
        return name;
    }
    std::unique_ptr<Node> base::shallowCopy() const {
        return std::make_unique<base>(this->getAttributes());
    }
    const std::string& bdi::getTagName() const {
        static const std::string name = "bdi";
        return name;
    }
    std::unique_ptr<Node> bdi::shallowCopy() const {
        return std::make_unique<bdi>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool bdi::isVoid() const {
        return 0;
    }
    const std::string& bdo::getTagName() const {
        static const std::string name = "bdo";
        return name;
    }
    std::unique_ptr<Node> bdo::shallowCopy() const {
        return std::make_unique<bdo>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool bdo::isVoid() const {
        return 0;
    }
    const std::string& blockquote::getTagName() const {
        static const std::string name = "blockquote";
        return name;
    }
    std::unique_ptr<Node> blockquote::shallowCopy() const {
        return std::make_unique<blockquote>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool blockquote::isVoid() const {
        return 0;
    }
    const std::string& body::getTagName() const {
        static const std::string name = "body";
        return name;
    }
    std::unique_ptr<Node> body::shallowCopy() const {
        return std::make_unique<body>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool body::isVoid() const {
        return 0;
    }
    const std::string& br::getTagName() const {
        static const std::string name = "br";
        return name;
    }
    std::unique_ptr<Node> br::shallowCopy() const {
        return std::make_unique<br>(this->getAttributes());
    }
    const std::string& button::getTagName() const {
        static const std::string name = "button";
        return name;
    }
    std::unique_ptr<Node> button::shallowCopy() const {
        return std::make_unique<button>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool button::isVoid() const {
        return 0;
    }
    const std::string& canvas::getTagName() const {
        static const std::string name = "canvas";
        return name;
    }
    std::unique_ptr<Node> canvas::shallowCopy() const {
        return std::make_unique<canvas>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool canvas::isVoid() const {
        return 0;
    }
    const std::string& caption::getTagName() const {
        static const std::string name = "caption";
        return name;
    }
    std::unique_ptr<Node> caption::shallowCopy() const {
        return std::make_unique<caption>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool caption::isVoid() const {
        return 0;
    }
    const std::string& cite::getTagName() const {
        static const std::string name = "cite";
        return name;
    }
    std::unique_ptr<Node> cite::shallowCopy() const {
        return std::make_unique<cite>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool cite::isVoid() const {
        return 0;
    }
    const std::string& code::getTagName() const {
        static const std::string name = "code";
        return name;
    }
    std::unique_ptr<Node> code::shallowCopy() const {
        return std::make_unique<code>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool code::isVoid() const {
        return 0;
    }
    const std::string& col::getTagName() const {
        static const std::string name = "col";
        return name;
    }
    std::unique_ptr<Node> col::shallowCopy() const {
        return std::make_unique<col>(this->getAttributes());
    }
    const std::string& colgroup::getTagName() const {
        static const std::string name = "colgroup";
        return name;
    }
    std::unique_ptr<Node> colgroup::shallowCopy() const {
        return std::make_unique<colgroup>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool colgroup::isVoid() const {
        return 0;
    }
    const std::string& data::getTagName() const {
        static const std::string name = "data";
        return name;
    }
    std::unique_ptr<Node> data::shallowCopy() const {
        return std::make_unique<data>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool data::isVoid() const {
        return 0;
    }
    const std::string& datalist::getTagName() const {
        static const std::string name = "datalist";
        return name;
    }
    std::unique_ptr<Node> datalist::shallowCopy() const {
        return std::make_unique<datalist>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool datalist::isVoid() const {
        return 0;
    }
    const std::string& dd::getTagName() const {
        static const std::string name = "dd";
        return name;
    }
    std::unique_ptr<Node> dd::shallowCopy() const {
        return std::make_unique<dd>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool dd::isVoid() const {
        return 0;
    }
    const std::string& del::getTagName() const {
        static const std::string name = "del";
        return name;
    }
    std::unique_ptr<Node> del::shallowCopy() const {
        return std::make_unique<del>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool del::isVoid() const {
        return 0;
    }
    const std::string& details::getTagName() const {
        static const std::string name = "details";
        return name;
    }
    std::unique_ptr<Node> details::shallowCopy() const {
        return std::make_unique<details>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool details::isVoid() const {
        return 0;
    }
    const std::string& dfn::getTagName() const {
        static const std::string name = "dfn";
        return name;
    }
    std::unique_ptr<Node> dfn::shallowCopy() const {
        return std::make_unique<dfn>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool dfn::isVoid() const {
        return 0;
    }
    const std::string& dialog::getTagName() const {
        static const std::string name = "dialog";
        return name;
    }
    std::unique_ptr<Node> dialog::shallowCopy() const {
        return std::make_unique<dialog>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool dialog::isVoid() const {
        return 0;
    }
    const std::string& div::getTagName() const {
        static const std::string name = "div";
        return name;
    }
    std::unique_ptr<Node> div::shallowCopy() const {
        return std::make_unique<div>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool div::isVoid() const {
        return 0;
    }
    const std::string& dl::getTagName() const {
        static const std::string name = "dl";
        return name;
    }
    std::unique_ptr<Node> dl::shallowCopy() const {
        return std::make_unique<dl>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool dl::isVoid() const {
        return 0;
    }
    const std::string& dt::getTagName() const {
        static const std::string name = "dt";
        return name;
    }
    std::unique_ptr<Node> dt::shallowCopy() const {
        return std::make_unique<dt>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool dt::isVoid() const {
        return 0;
    }
    const std::string& em::getTagName() const {
        static const std::string name = "em";
        return name;
    }
    std::unique_ptr<Node> em::shallowCopy() const {
        return std::make_unique<em>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool em::isVoid() const {
        return 0;
    }
    const std::string& embed::getTagName() const {
        static const std::string name = "embed";
        return name;
    }
    std::unique_ptr<Node> embed::shallowCopy() const {
        return std::make_unique<embed>(this->getAttributes());
    }
    const std::string& fieldset::getTagName() const {
        static const std::string name = "fieldset";
        return name;
    }
    std::unique_ptr<Node> fieldset::shallowCopy() const {
        return std::make_unique<fieldset>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool fieldset::isVoid() const {
        return 0;
    }
    const std::string& figcaption::getTagName() const {
        static const std::string name = "figcaption";
        return name;
    }
    std::unique_ptr<Node> figcaption::shallowCopy() const {
        return std::make_unique<figcaption>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool figcaption::isVoid() const {
        return 0;
    }
    const std::string& figure::getTagName() const {
        static const std::string name = "figure";
        return name;
    }
    std::unique_ptr<Node> figure::shallowCopy() const {
        return std::make_unique<figure>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool figure::isVoid() const {
        return 0;
    }
    const std::string& footer::getTagName() const {
        static const std::string name = "footer";
        return name;
    }
    std::unique_ptr<Node> footer::shallowCopy() const {
        return std::make_unique<footer>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool footer::isVoid() const {
        return 0;
    }
    const std::string& form::getTagName() const {
        static const std::string name = "form";
        return name;
    }
    std::unique_ptr<Node> form::shallowCopy() const {
        return std::make_unique<form>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool form::isVoid() const {
        return 0;
    }
    const std::string& h1::getTagName() const {
        static const std::string name = "h1";
        return name;
    }
    std::unique_ptr<Node> h1::shallowCopy() const {
        return std::make_unique<h1>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool h1::isVoid() const {
        return 0;
    }
    const std::string& h2::getTagName() const {
        static const std::string name = "h2";
        return name;
    }
    std::unique_ptr<Node> h2::shallowCopy() const {
        return std::make_unique<h2>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool h2::isVoid() const {
        return 0;
    }
    const std::string& h3::getTagName() const {
        static const std::string name = "h3";
        return name;
    }
    std::unique_ptr<Node> h3::shallowCopy() const {
        return std::make_unique<h3>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool h3::isVoid() const {
        return 0;
    }
    const std::string& h4::getTagName() const {
        static const std::string name = "h4";
        return name;
    }
    std::unique_ptr<Node> h4::shallowCopy() const {
        return std::make_unique<h4>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool h4::isVoid() const {
        return 0;
    }
    const std::string& h5::getTagName() const {
        static const std::string name = "h5";
        return name;
    }
    std::unique_ptr<Node> h5::shallowCopy() const {
        return std::make_unique<h5>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool h5::isVoid() const {
        return 0;
    }
    const std::string& h6::getTagName() const {
        static const std::string name = "h6";
        return name;
    }
    std::unique_ptr<Node> h6::shallowCopy() const {
        return std::make_unique<h6>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool h6::isVoid() const {
        return 0;
    }
    const std::string& head::getTagName() const {
        static const std::string name = "head";
        return name;
    }
    std::unique_ptr<Node> head::shallowCopy() const {
        return std::make_unique<head>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool head::isVoid() const {
        return 0;
    }
    const std::string& header::getTagName() const {
        static const std::string name = "header";
        return name;
    }
    std::unique_ptr<Node> header::shallowCopy() const {
        return std::make_unique<header>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool header::isVoid() const {
        return 0;
    }
    const std::string& hgroup::getTagName() const {
        static const std::string name = "hgroup";
        return name;
    }
    std::unique_ptr<Node> hgroup::shallowCopy() const {
        return std::make_unique<hgroup>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool hgroup::isVoid() const {
        return 0;
    }
    const std::string& hr::getTagName() const {
        static const std::string name = "hr";
        return name;
    }
    std::unique_ptr<Node> hr::shallowCopy() const {
        return std::make_unique<hr>(this->getAttributes());
    }
    const std::string& html::getTagName() const {
        static const std::string name = "html";
        return name;
    }
    std::unique_ptr<Node> html::shallowCopy() const {
        return std::make_unique<html>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool html::isVoid() const {
        return 0;
    }
    const std::string& i::getTagName() const {
        static const std::string name = "i";
        return name;
    }
    std::unique_ptr<Node> i::shallowCopy() const {
        return std::make_unique<i>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool i::isVoid() const {
        return 0;
    }
    const std::string& iframe::getTagName() const {
        static const std::string name = "iframe";
        return name;
    }
    std::unique_ptr<Node> iframe::shallowCopy() const {
        return std::make_unique<iframe>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool iframe::isVoid() const {
        return 0;
    }
    const std::string& img::getTagName() const {
        static const std::string name = "img";
        return name;
    }
    std::unique_ptr<Node> img::shallowCopy() const {
        return std::make_unique<img>(this->getAttributes());
    }
    const std::string& input::getTagName() const {
        static const std::string name = "input";
        return name;
    }
    std::unique_ptr<Node> input::shallowCopy() const {
        return std::make_unique<input>(this->getAttributes());
    }
    const std::string& ins::getTagName() const {
        static const std::string name = "ins";
        return name;
    }
    std::unique_ptr<Node> ins::shallowCopy() const {
        return std::make_unique<ins>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool ins::isVoid() const {
        return 0;
    }
    const std::string& kbd::getTagName() const {
        static const std::string name = "kbd";
        return name;
    }
    std::unique_ptr<Node> kbd::shallowCopy() const {
        return std::make_unique<kbd>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool kbd::isVoid() const {
        return 0;
    }
    const std::string& label::getTagName() const {
        static const std::string name = "label";
        return name;
    }
    std::unique_ptr<Node> label::shallowCopy() const {
        return std::make_unique<label>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool label::isVoid() const {
        return 0;
    }
    const std::string& legend::getTagName() const {
        static const std::string name = "legend";
        return name;
    }
    std::unique_ptr<Node> legend::shallowCopy() const {
        return std::make_unique<legend>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool legend::isVoid() const {
        return 0;
    }
    const std::string& li::getTagName() const {
        static const std::string name = "li";
        return name;
    }
    std::unique_ptr<Node> li::shallowCopy() const {
        return std::make_unique<li>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool li::isVoid() const {
        return 0;
    }
    const std::string& clink::getTagName() const {
        static const std::string name = "link";
        return name;
    }
    std::unique_ptr<Node> clink::shallowCopy() const {
        return std::make_unique<clink>(this->getAttributes());
    }
    const std::string& main::getTagName() const {
        static const std::string name = "main";
        return name;
    }
    std::unique_ptr<Node> main::shallowCopy() const {
        return std::make_unique<main>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool main::isVoid() const {
        return 0;
    }
    const std::string& map::getTagName() const {
        static const std::string name = "map";
        return name;
    }
    std::unique_ptr<Node> map::shallowCopy() const {
        return std::make_unique<map>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool map::isVoid() const {
        return 0;
    }
    const std::string& mark::getTagName() const {
        static const std::string name = "mark";
        return name;
    }
    std::unique_ptr<Node> mark::shallowCopy() const {
        return std::make_unique<mark>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool mark::isVoid() const {
        return 0;
    }
    const std::string& math::getTagName() const {
        static const std::string name = "math";
        return name;
    }
    std::unique_ptr<Node> math::shallowCopy() const {
        return std::make_unique<math>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool math::isVoid() const {
        return 0;
    }
    const std::string& menu::getTagName() const {
        static const std::string name = "menu";
        return name;
    }
    std::unique_ptr<Node> menu::shallowCopy() const {
        return std::make_unique<menu>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool menu::isVoid() const {
        return 0;
    }
    const std::string& menuitem::getTagName() const {
        static const std::string name = "menuitem";
        return name;
    }
    std::unique_ptr<Node> menuitem::shallowCopy() const {
        return std::make_unique<menuitem>(this->getAttributes());
    }
    const std::string& meta::getTagName() const {
        static const std::string name = "meta";
        return name;
    }
    std::unique_ptr<Node> meta::shallowCopy() const {
        return std::make_unique<meta>(this->getAttributes());
    }
    const std::string& meter::getTagName() const {
        static const std::string name = "meter";
        return name;
    }
    std::unique_ptr<Node> meter::shallowCopy() const {
        return std::make_unique<meter>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool meter::isVoid() const {
        return 0;
    }
    const std::string& nav::getTagName() const {
        static const std::string name = "nav";
        return name;
    }
    std::unique_ptr<Node> nav::shallowCopy() const {
        return std::make_unique<nav>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool nav::isVoid() const {
        return 0;
    }
    const std::string& noscript::getTagName() const {
        static const std::string name = "noscript";
        return name;
    }
    std::unique_ptr<Node> noscript::shallowCopy() const {
        return std::make_unique<noscript>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool noscript::isVoid() const {
        return 0;
    }
    const std::string& object::getTagName() const {
        static const std::string name = "object";
        return name;
    }
    std::unique_ptr<Node> object::shallowCopy() const {
        return std::make_unique<object>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool object::isVoid() const {
        return 0;
    }
    const std::string& ol::getTagName() const {
        static const std::string name = "ol";
        return name;
    }
    std::unique_ptr<Node> ol::shallowCopy() const {
        return std::make_unique<ol>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool ol::isVoid() const {
        return 0;
    }
    const std::string& optgroup::getTagName() const {
        static const std::string name = "optgroup";
        return name;
    }
    std::unique_ptr<Node> optgroup::shallowCopy() const {
        return std::make_unique<optgroup>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool optgroup::isVoid() const {
        return 0;
    }
    const std::string& option::getTagName() const {
        static const std::string name = "option";
        return name;
    }
    std::unique_ptr<Node> option::shallowCopy() const {
        return std::make_unique<option>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool option::isVoid() const {
        return 0;
    }
    const std::string& output::getTagName() const {
        static const std::string name = "output";
        return name;
    }
    std::unique_ptr<Node> output::shallowCopy() const {
        return std::make_unique<output>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool output::isVoid() const {
        return 0;
    }
    const std::string& p::getTagName() const {
        static const std::string name = "p";
        return name;
    }
    std::unique_ptr<Node> p::shallowCopy() const {
        return std::make_unique<p>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool p::isVoid() const {
        return 0;
    }
    const std::string& param::getTagName() const {
        static const std::string name = "param";
        return name;
    }
    std::unique_ptr<Node> param::shallowCopy() const {
        return std::make_unique<param>(this->getAttributes());
    }
    const std::string& picture::getTagName() const {
        static const std::string name = "picture";
        return name;
    }
    std::unique_ptr<Node> picture::shallowCopy() const {
        return std::make_unique<picture>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool picture::isVoid() const {
        return 0;
    }
    const std::string& pre::getTagName() const {
        static const std::string name = "pre";
        return name;
    }
    std::unique_ptr<Node> pre::shallowCopy() const {
        return std::make_unique<pre>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool pre::isVoid() const {
        return 0;
    }
    const std::string& progress::getTagName() const {
        static const std::string name = "progress";
        return name;
    }
    std::unique_ptr<Node> progress::shallowCopy() const {
        return std::make_unique<progress>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool progress::isVoid() const {
        return 0;
    }
    const std::string& q::getTagName() const {
        static const std::string name = "q";
        return name;
    }
    std::unique_ptr<Node> q::shallowCopy() const {
        return std::make_unique<q>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool q::isVoid() const {
        return 0;
    }
    const std::string& rb::getTagName() const {
        static const std::string name = "rb";
        return name;
    }
    std::unique_ptr<Node> rb::shallowCopy() const {
        return std::make_unique<rb>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool rb::isVoid() const {
        return 0;
    }
    const std::string& rp::getTagName() const {
        static const std::string name = "rp";
        return name;
    }
    std::unique_ptr<Node> rp::shallowCopy() const {
        return std::make_unique<rp>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool rp::isVoid() const {
        return 0;
    }
    const std::string& rt::getTagName() const {
        static const std::string name = "rt";
        return name;
    }
    std::unique_ptr<Node> rt::shallowCopy() const {
        return std::make_unique<rt>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool rt::isVoid() const {
        return 0;
    }
    const std::string& rtc::getTagName() const {
        static const std::string name = "rtc";
        return name;
    }
    std::unique_ptr<Node> rtc::shallowCopy() const {
        return std::make_unique<rtc>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool rtc::isVoid() const {
        return 0;
    }
    const std::string& ruby::getTagName() const {
        static const std::string name = "ruby";
        return name;
    }
    std::unique_ptr<Node> ruby::shallowCopy() const {
        return std::make_unique<ruby>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool ruby::isVoid() const {
        return 0;
    }
    const std::string& s::getTagName() const {
        static const std::string name = "s";
        return name;
    }
    std::unique_ptr<Node> s::shallowCopy() const {
        return std::make_unique<s>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool s::isVoid() const {
        return 0;
    }
    const std::string& samp::getTagName() const {
        static const std::string name = "samp";
        return name;
    }
    std::unique_ptr<Node> samp::shallowCopy() const {
        return std::make_unique<samp>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool samp::isVoid() const {
        return 0;
    }
    const std::string& script::getTagName() const {
        static const std::string name = "script";
        return name;
    }
    std::unique_ptr<Node> script::shallowCopy() const {
        return std::make_unique<script>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool script::isVoid() const {
        return 0;
    }
    const std::string& search::getTagName() const {
        static const std::string name = "search";
        return name;
    }
    std::unique_ptr<Node> search::shallowCopy() const {
        return std::make_unique<search>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool search::isVoid() const {
        return 0;
    }
    const std::string& section::getTagName() const {
        static const std::string name = "section";
        return name;
    }
    std::unique_ptr<Node> section::shallowCopy() const {
        return std::make_unique<section>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool section::isVoid() const {
        return 0;
    }
    const std::string& select::getTagName() const {
        static const std::string name = "select";
        return name;
    }
    std::unique_ptr<Node> select::shallowCopy() const {
        return std::make_unique<select>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool select::isVoid() const {
        return 0;
    }
    const std::string& slot::getTagName() const {
        static const std::string name = "slot";
        return name;
    }
    std::unique_ptr<Node> slot::shallowCopy() const {
        return std::make_unique<slot>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool slot::isVoid() const {
        return 0;
    }
    const std::string& small::getTagName() const {
        static const std::string name = "small";
        return name;
    }
    std::unique_ptr<Node> small::shallowCopy() const {
        return std::make_unique<small>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool small::isVoid() const {
        return 0;
    }
    const std::string& source::getTagName() const {
        static const std::string name = "source";
        return name;
    }
    std::unique_ptr<Node> source::shallowCopy() const {
        return std::make_unique<source>(this->getAttributes());
    }
    const std::string& span::getTagName() const {
        static const std::string name = "span";
        return name;
    }
    std::unique_ptr<Node> span::shallowCopy() const {
        return std::make_unique<span>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool span::isVoid() const {
        return 0;
    }
    const std::string& strong::getTagName() const {
        static const std::string name = "strong";
        return name;
    }
    std::unique_ptr<Node> strong::shallowCopy() const {
        return std::make_unique<strong>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool strong::isVoid() const {
        return 0;
    }
    const std::string& style::getTagName() const {
        static const std::string name = "style";
        return name;
    }
    std::unique_ptr<Node> style::shallowCopy() const {
        return std::make_unique<style>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool style::isVoid() const {
        return 0;
    }
    const std::string& sub::getTagName() const {
        static const std::string name = "sub";
        return name;
    }
    std::unique_ptr<Node> sub::shallowCopy() const {
        return std::make_unique<sub>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool sub::isVoid() const {
        return 0;
    }
    const std::string& summary::getTagName() const {
        static const std::string name = "summary";
        return name;
    }
    std::unique_ptr<Node> summary::shallowCopy() const {
        return std::make_unique<summary>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool summary::isVoid() const {
        return 0;
    }
    const std::string& sup::getTagName() const {
        static const std::string name = "sup";
        return name;
    }
    std::unique_ptr<Node> sup::shallowCopy() const {
        return std::make_unique<sup>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool sup::isVoid() const {
        return 0;
    }
    const std::string& svg::getTagName() const {
        static const std::string name = "svg";
        return name;
    }
    std::unique_ptr<Node> svg::shallowCopy() const {
        return std::make_unique<svg>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool svg::isVoid() const {
        return 0;
    }
    const std::string& table::getTagName() const {
        static const std::string name = "table";
        return name;
    }
    std::unique_ptr<Node> table::shallowCopy() const {
        return std::make_unique<table>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool table::isVoid() const {
        return 0;
    }
    const std::string& tbody::getTagName() const {
        static const std::string name = "tbody";
        return name;
    }
    std::unique_ptr<Node> tbody::shallowCopy() const {
        return std::make_unique<tbody>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool tbody::isVoid() const {
        return 0;
    }
    const std::string& td::getTagName() const {
        static const std::string name = "td";
        return name;
    }
    std::unique_ptr<Node> td::shallowCopy() const {
        return std::make_unique<td>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool td::isVoid() const {
        return 0;
    }
    const std::string& ctemplate::getTagName() const {
        static const std::string name = "template";
        return name;
    }
    std::unique_ptr<Node> ctemplate::shallowCopy() const {
        return std::make_unique<ctemplate>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool ctemplate::isVoid() const {
        return 0;
    }
    const std::string& textarea::getTagName() const {
        static const std::string name = "textarea";
        return name;
    }
    std::unique_ptr<Node> textarea::shallowCopy() const {
        return std::make_unique<textarea>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool textarea::isVoid() const {
        return 0;
    }
    const std::string& tfoot::getTagName() const {
        static const std::string name = "tfoot";
        return name;
    }
    std::unique_ptr<Node> tfoot::shallowCopy() const {
        return std::make_unique<tfoot>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool tfoot::isVoid() const {
        return 0;
    }
    const std::string& th::getTagName() const {
        static const std::string name = "th";
        return name;
    }
    std::unique_ptr<Node> th::shallowCopy() const {
        return std::make_unique<th>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool th::isVoid() const {
        return 0;
    }
    const std::string& thead::getTagName() const {
        static const std::string name = "thead";
        return name;
    }
    std::unique_ptr<Node> thead::shallowCopy() const {
        return std::make_unique<thead>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool thead::isVoid() const {
        return 0;
    }
    const std::string& time::getTagName() const {
        static const std::string name = "time";
        return name;
    }
    std::unique_ptr<Node> time::shallowCopy() const {
        return std::make_unique<time>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool time::isVoid() const {
        return 0;
    }
    const std::string& title::getTagName() const {
        static const std::string name = "title";
        return name;
    }
    std::unique_ptr<Node> title::shallowCopy() const {
        return std::make_unique<title>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool title::isVoid() const {
        return 0;
    }
    const std::string& tr::getTagName() const {
        static const std::string name = "tr";
        return name;
    }
    std::unique_ptr<Node> tr::shallowCopy() const {
        return std::make_unique<tr>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool tr::isVoid() const {
        return 0;
    }
    const std::string& track::getTagName() const {
        static const std::string name = "track";
        return name;
    }
    std::unique_ptr<Node> track::shallowCopy() const {
        return std::make_unique<track>(this->getAttributes());
    }
    const std::string& u::getTagName() const {
        static const std::string name = "u";
        return name;
    }
    std::unique_ptr<Node> u::shallowCopy() const {
        return std::make_unique<u>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool u::isVoid() const {
        return 0;
    }
    const std::string& ul::getTagName() const {
        static const std::string name = "ul";
        return name;
    }
    std::unique_ptr<Node> ul::shallowCopy() const {
        return std::make_unique<ul>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool ul::isVoid() const {
        return 0;
    }
    const std::string& var::getTagName() const {
        static const std::string name = "var";
        return name;
    }
    std::unique_ptr<Node> var::shallowCopy() const {
        return std::make_unique<var>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool var::isVoid() const {
        return 0;
    }
    const std::string& video::getTagName() const {
        static const std::string name = "video";
        return name;
    }
    std::unique_ptr<Node> video::shallowCopy() const {
        return std::make_unique<video>(this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
    bool video::isVoid() const {
        return 0;
    }
    const std::string& wbr::getTagName() const {
        static const std::string name = "wbr";
        return name;
    }
    std::unique_ptr<Node> wbr::shallowCopy() const {
        return std::make_unique<wbr>(this->getAttributes());
    }
}

