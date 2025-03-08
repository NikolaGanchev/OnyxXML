#include "tags.h"

const std::string& Templater::html::dtags::a::getTagName() const {
    static const std::string name = "a";
    return name;
}

Templater::html::dtags::a::a(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::a::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::a::clone() const {
    return std::make_shared<a>(*this);
}
const std::string& Templater::html::dtags::abbr::getTagName() const {
    static const std::string name = "abbr";
    return name;
}

Templater::html::dtags::abbr::abbr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::abbr::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::abbr::clone() const {
    return std::make_shared<abbr>(*this);
}
const std::string& Templater::html::dtags::address::getTagName() const {
    static const std::string name = "address";
    return name;
}

Templater::html::dtags::address::address(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::address::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::address::clone() const {
    return std::make_shared<address>(*this);
}
const std::string& Templater::html::dtags::area::getTagName() const {
    static const std::string name = "area";
    return name;
}

Templater::html::dtags::area::area(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::area::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::area::clone() const {
    return std::make_shared<area>(*this);
}
const std::string& Templater::html::dtags::article::getTagName() const {
    static const std::string name = "article";
    return name;
}

Templater::html::dtags::article::article(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::article::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::article::clone() const {
    return std::make_shared<article>(*this);
}
const std::string& Templater::html::dtags::aside::getTagName() const {
    static const std::string name = "aside";
    return name;
}

Templater::html::dtags::aside::aside(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::aside::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::aside::clone() const {
    return std::make_shared<aside>(*this);
}
const std::string& Templater::html::dtags::audio::getTagName() const {
    static const std::string name = "audio";
    return name;
}

Templater::html::dtags::audio::audio(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::audio::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::audio::clone() const {
    return std::make_shared<audio>(*this);
}
const std::string& Templater::html::dtags::b::getTagName() const {
    static const std::string name = "b";
    return name;
}

Templater::html::dtags::b::b(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::b::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::b::clone() const {
    return std::make_shared<b>(*this);
}
const std::string& Templater::html::dtags::base::getTagName() const {
    static const std::string name = "base";
    return name;
}

Templater::html::dtags::base::base(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::base::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::base::clone() const {
    return std::make_shared<base>(*this);
}
const std::string& Templater::html::dtags::bdi::getTagName() const {
    static const std::string name = "bdi";
    return name;
}

Templater::html::dtags::bdi::bdi(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::bdi::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::bdi::clone() const {
    return std::make_shared<bdi>(*this);
}
const std::string& Templater::html::dtags::bdo::getTagName() const {
    static const std::string name = "bdo";
    return name;
}

Templater::html::dtags::bdo::bdo(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::bdo::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::bdo::clone() const {
    return std::make_shared<bdo>(*this);
}
const std::string& Templater::html::dtags::blockquote::getTagName() const {
    static const std::string name = "blockquote";
    return name;
}

Templater::html::dtags::blockquote::blockquote(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::blockquote::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::blockquote::clone() const {
    return std::make_shared<blockquote>(*this);
}
const std::string& Templater::html::dtags::body::getTagName() const {
    static const std::string name = "body";
    return name;
}

Templater::html::dtags::body::body(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::body::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::body::clone() const {
    return std::make_shared<body>(*this);
}
const std::string& Templater::html::dtags::br::getTagName() const {
    static const std::string name = "br";
    return name;
}

Templater::html::dtags::br::br(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::br::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::br::clone() const {
    return std::make_shared<br>(*this);
}
const std::string& Templater::html::dtags::button::getTagName() const {
    static const std::string name = "button";
    return name;
}

Templater::html::dtags::button::button(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::button::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::button::clone() const {
    return std::make_shared<button>(*this);
}
const std::string& Templater::html::dtags::canvas::getTagName() const {
    static const std::string name = "canvas";
    return name;
}

Templater::html::dtags::canvas::canvas(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::canvas::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::canvas::clone() const {
    return std::make_shared<canvas>(*this);
}
const std::string& Templater::html::dtags::caption::getTagName() const {
    static const std::string name = "caption";
    return name;
}

Templater::html::dtags::caption::caption(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::caption::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::caption::clone() const {
    return std::make_shared<caption>(*this);
}
const std::string& Templater::html::dtags::cite::getTagName() const {
    static const std::string name = "cite";
    return name;
}

Templater::html::dtags::cite::cite(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::cite::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::cite::clone() const {
    return std::make_shared<cite>(*this);
}
const std::string& Templater::html::dtags::code::getTagName() const {
    static const std::string name = "code";
    return name;
}

Templater::html::dtags::code::code(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::code::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::code::clone() const {
    return std::make_shared<code>(*this);
}
const std::string& Templater::html::dtags::col::getTagName() const {
    static const std::string name = "col";
    return name;
}

Templater::html::dtags::col::col(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::col::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::col::clone() const {
    return std::make_shared<col>(*this);
}
const std::string& Templater::html::dtags::colgroup::getTagName() const {
    static const std::string name = "colgroup";
    return name;
}

Templater::html::dtags::colgroup::colgroup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::colgroup::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::colgroup::clone() const {
    return std::make_shared<colgroup>(*this);
}
const std::string& Templater::html::dtags::data::getTagName() const {
    static const std::string name = "data";
    return name;
}

Templater::html::dtags::data::data(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::data::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::data::clone() const {
    return std::make_shared<data>(*this);
}
const std::string& Templater::html::dtags::datalist::getTagName() const {
    static const std::string name = "datalist";
    return name;
}

Templater::html::dtags::datalist::datalist(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::datalist::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::datalist::clone() const {
    return std::make_shared<datalist>(*this);
}
const std::string& Templater::html::dtags::dd::getTagName() const {
    static const std::string name = "dd";
    return name;
}

Templater::html::dtags::dd::dd(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::dd::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::dd::clone() const {
    return std::make_shared<dd>(*this);
}
const std::string& Templater::html::dtags::del::getTagName() const {
    static const std::string name = "del";
    return name;
}

Templater::html::dtags::del::del(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::del::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::del::clone() const {
    return std::make_shared<del>(*this);
}
const std::string& Templater::html::dtags::details::getTagName() const {
    static const std::string name = "details";
    return name;
}

Templater::html::dtags::details::details(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::details::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::details::clone() const {
    return std::make_shared<details>(*this);
}
const std::string& Templater::html::dtags::dfn::getTagName() const {
    static const std::string name = "dfn";
    return name;
}

Templater::html::dtags::dfn::dfn(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::dfn::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::dfn::clone() const {
    return std::make_shared<dfn>(*this);
}
const std::string& Templater::html::dtags::dialog::getTagName() const {
    static const std::string name = "dialog";
    return name;
}

Templater::html::dtags::dialog::dialog(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::dialog::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::dialog::clone() const {
    return std::make_shared<dialog>(*this);
}
const std::string& Templater::html::dtags::div::getTagName() const {
    static const std::string name = "div";
    return name;
}

Templater::html::dtags::div::div(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::div::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::div::clone() const {
    return std::make_shared<div>(*this);
}
const std::string& Templater::html::dtags::dl::getTagName() const {
    static const std::string name = "dl";
    return name;
}

Templater::html::dtags::dl::dl(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::dl::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::dl::clone() const {
    return std::make_shared<dl>(*this);
}
const std::string& Templater::html::dtags::dt::getTagName() const {
    static const std::string name = "dt";
    return name;
}

Templater::html::dtags::dt::dt(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::dt::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::dt::clone() const {
    return std::make_shared<dt>(*this);
}
const std::string& Templater::html::dtags::em::getTagName() const {
    static const std::string name = "em";
    return name;
}

Templater::html::dtags::em::em(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::em::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::em::clone() const {
    return std::make_shared<em>(*this);
}
const std::string& Templater::html::dtags::embed::getTagName() const {
    static const std::string name = "embed";
    return name;
}

Templater::html::dtags::embed::embed(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::embed::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::embed::clone() const {
    return std::make_shared<embed>(*this);
}
const std::string& Templater::html::dtags::fieldset::getTagName() const {
    static const std::string name = "fieldset";
    return name;
}

Templater::html::dtags::fieldset::fieldset(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::fieldset::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::fieldset::clone() const {
    return std::make_shared<fieldset>(*this);
}
const std::string& Templater::html::dtags::figcaption::getTagName() const {
    static const std::string name = "figcaption";
    return name;
}

Templater::html::dtags::figcaption::figcaption(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::figcaption::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::figcaption::clone() const {
    return std::make_shared<figcaption>(*this);
}
const std::string& Templater::html::dtags::figure::getTagName() const {
    static const std::string name = "figure";
    return name;
}

Templater::html::dtags::figure::figure(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::figure::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::figure::clone() const {
    return std::make_shared<figure>(*this);
}
const std::string& Templater::html::dtags::footer::getTagName() const {
    static const std::string name = "footer";
    return name;
}

Templater::html::dtags::footer::footer(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::footer::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::footer::clone() const {
    return std::make_shared<footer>(*this);
}
const std::string& Templater::html::dtags::form::getTagName() const {
    static const std::string name = "form";
    return name;
}

Templater::html::dtags::form::form(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::form::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::form::clone() const {
    return std::make_shared<form>(*this);
}
const std::string& Templater::html::dtags::h1::getTagName() const {
    static const std::string name = "h1";
    return name;
}

Templater::html::dtags::h1::h1(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::h1::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::h1::clone() const {
    return std::make_shared<h1>(*this);
}
const std::string& Templater::html::dtags::h2::getTagName() const {
    static const std::string name = "h2";
    return name;
}

Templater::html::dtags::h2::h2(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::h2::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::h2::clone() const {
    return std::make_shared<h2>(*this);
}
const std::string& Templater::html::dtags::h3::getTagName() const {
    static const std::string name = "h3";
    return name;
}

Templater::html::dtags::h3::h3(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::h3::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::h3::clone() const {
    return std::make_shared<h3>(*this);
}
const std::string& Templater::html::dtags::h4::getTagName() const {
    static const std::string name = "h4";
    return name;
}

Templater::html::dtags::h4::h4(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::h4::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::h4::clone() const {
    return std::make_shared<h4>(*this);
}
const std::string& Templater::html::dtags::h5::getTagName() const {
    static const std::string name = "h5";
    return name;
}

Templater::html::dtags::h5::h5(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::h5::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::h5::clone() const {
    return std::make_shared<h5>(*this);
}
const std::string& Templater::html::dtags::h6::getTagName() const {
    static const std::string name = "h6";
    return name;
}

Templater::html::dtags::h6::h6(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::h6::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::h6::clone() const {
    return std::make_shared<h6>(*this);
}
const std::string& Templater::html::dtags::head::getTagName() const {
    static const std::string name = "head";
    return name;
}

Templater::html::dtags::head::head(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::head::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::head::clone() const {
    return std::make_shared<head>(*this);
}
const std::string& Templater::html::dtags::header::getTagName() const {
    static const std::string name = "header";
    return name;
}

Templater::html::dtags::header::header(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::header::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::header::clone() const {
    return std::make_shared<header>(*this);
}
const std::string& Templater::html::dtags::hgroup::getTagName() const {
    static const std::string name = "hgroup";
    return name;
}

Templater::html::dtags::hgroup::hgroup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::hgroup::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::hgroup::clone() const {
    return std::make_shared<hgroup>(*this);
}
const std::string& Templater::html::dtags::hr::getTagName() const {
    static const std::string name = "hr";
    return name;
}

Templater::html::dtags::hr::hr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::hr::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::hr::clone() const {
    return std::make_shared<hr>(*this);
}
const std::string& Templater::html::dtags::html::getTagName() const {
    static const std::string name = "html";
    return name;
}

Templater::html::dtags::html::html(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::html::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::html::clone() const {
    return std::make_shared<html>(*this);
}
const std::string& Templater::html::dtags::i::getTagName() const {
    static const std::string name = "i";
    return name;
}

Templater::html::dtags::i::i(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::i::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::i::clone() const {
    return std::make_shared<i>(*this);
}
const std::string& Templater::html::dtags::iframe::getTagName() const {
    static const std::string name = "iframe";
    return name;
}

Templater::html::dtags::iframe::iframe(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::iframe::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::iframe::clone() const {
    return std::make_shared<iframe>(*this);
}
const std::string& Templater::html::dtags::img::getTagName() const {
    static const std::string name = "img";
    return name;
}

Templater::html::dtags::img::img(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::img::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::img::clone() const {
    return std::make_shared<img>(*this);
}
const std::string& Templater::html::dtags::input::getTagName() const {
    static const std::string name = "input";
    return name;
}

Templater::html::dtags::input::input(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::input::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::input::clone() const {
    return std::make_shared<input>(*this);
}
const std::string& Templater::html::dtags::ins::getTagName() const {
    static const std::string name = "ins";
    return name;
}

Templater::html::dtags::ins::ins(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::ins::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::ins::clone() const {
    return std::make_shared<ins>(*this);
}
const std::string& Templater::html::dtags::kbd::getTagName() const {
    static const std::string name = "kbd";
    return name;
}

Templater::html::dtags::kbd::kbd(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::kbd::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::kbd::clone() const {
    return std::make_shared<kbd>(*this);
}
const std::string& Templater::html::dtags::label::getTagName() const {
    static const std::string name = "label";
    return name;
}

Templater::html::dtags::label::label(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::label::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::label::clone() const {
    return std::make_shared<label>(*this);
}
const std::string& Templater::html::dtags::legend::getTagName() const {
    static const std::string name = "legend";
    return name;
}

Templater::html::dtags::legend::legend(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::legend::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::legend::clone() const {
    return std::make_shared<legend>(*this);
}
const std::string& Templater::html::dtags::li::getTagName() const {
    static const std::string name = "li";
    return name;
}

Templater::html::dtags::li::li(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::li::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::li::clone() const {
    return std::make_shared<li>(*this);
}
const std::string& Templater::html::dtags::link::getTagName() const {
    static const std::string name = "link";
    return name;
}

Templater::html::dtags::link::link(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::link::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::link::clone() const {
    return std::make_shared<link>(*this);
}
const std::string& Templater::html::dtags::main::getTagName() const {
    static const std::string name = "main";
    return name;
}

Templater::html::dtags::main::main(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::main::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::main::clone() const {
    return std::make_shared<main>(*this);
}
const std::string& Templater::html::dtags::map::getTagName() const {
    static const std::string name = "map";
    return name;
}

Templater::html::dtags::map::map(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::map::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::map::clone() const {
    return std::make_shared<map>(*this);
}
const std::string& Templater::html::dtags::mark::getTagName() const {
    static const std::string name = "mark";
    return name;
}

Templater::html::dtags::mark::mark(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::mark::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::mark::clone() const {
    return std::make_shared<mark>(*this);
}
const std::string& Templater::html::dtags::math::getTagName() const {
    static const std::string name = "math";
    return name;
}

Templater::html::dtags::math::math(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::math::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::math::clone() const {
    return std::make_shared<math>(*this);
}
const std::string& Templater::html::dtags::menu::getTagName() const {
    static const std::string name = "menu";
    return name;
}

Templater::html::dtags::menu::menu(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::menu::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::menu::clone() const {
    return std::make_shared<menu>(*this);
}
const std::string& Templater::html::dtags::menuitem::getTagName() const {
    static const std::string name = "menuitem";
    return name;
}

Templater::html::dtags::menuitem::menuitem(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::menuitem::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::menuitem::clone() const {
    return std::make_shared<menuitem>(*this);
}
const std::string& Templater::html::dtags::meta::getTagName() const {
    static const std::string name = "meta";
    return name;
}

Templater::html::dtags::meta::meta(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::meta::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::meta::clone() const {
    return std::make_shared<meta>(*this);
}
const std::string& Templater::html::dtags::meter::getTagName() const {
    static const std::string name = "meter";
    return name;
}

Templater::html::dtags::meter::meter(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::meter::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::meter::clone() const {
    return std::make_shared<meter>(*this);
}
const std::string& Templater::html::dtags::nav::getTagName() const {
    static const std::string name = "nav";
    return name;
}

Templater::html::dtags::nav::nav(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::nav::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::nav::clone() const {
    return std::make_shared<nav>(*this);
}
const std::string& Templater::html::dtags::noscript::getTagName() const {
    static const std::string name = "noscript";
    return name;
}

Templater::html::dtags::noscript::noscript(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::noscript::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::noscript::clone() const {
    return std::make_shared<noscript>(*this);
}
const std::string& Templater::html::dtags::object::getTagName() const {
    static const std::string name = "object";
    return name;
}

Templater::html::dtags::object::object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::object::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::object::clone() const {
    return std::make_shared<object>(*this);
}
const std::string& Templater::html::dtags::ol::getTagName() const {
    static const std::string name = "ol";
    return name;
}

Templater::html::dtags::ol::ol(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::ol::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::ol::clone() const {
    return std::make_shared<ol>(*this);
}
const std::string& Templater::html::dtags::optgroup::getTagName() const {
    static const std::string name = "optgroup";
    return name;
}

Templater::html::dtags::optgroup::optgroup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::optgroup::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::optgroup::clone() const {
    return std::make_shared<optgroup>(*this);
}
const std::string& Templater::html::dtags::option::getTagName() const {
    static const std::string name = "option";
    return name;
}

Templater::html::dtags::option::option(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::option::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::option::clone() const {
    return std::make_shared<option>(*this);
}
const std::string& Templater::html::dtags::output::getTagName() const {
    static const std::string name = "output";
    return name;
}

Templater::html::dtags::output::output(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::output::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::output::clone() const {
    return std::make_shared<output>(*this);
}
const std::string& Templater::html::dtags::p::getTagName() const {
    static const std::string name = "p";
    return name;
}

Templater::html::dtags::p::p(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::p::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::p::clone() const {
    return std::make_shared<p>(*this);
}
const std::string& Templater::html::dtags::param::getTagName() const {
    static const std::string name = "param";
    return name;
}

Templater::html::dtags::param::param(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::param::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::param::clone() const {
    return std::make_shared<param>(*this);
}
const std::string& Templater::html::dtags::picture::getTagName() const {
    static const std::string name = "picture";
    return name;
}

Templater::html::dtags::picture::picture(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::picture::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::picture::clone() const {
    return std::make_shared<picture>(*this);
}
const std::string& Templater::html::dtags::pre::getTagName() const {
    static const std::string name = "pre";
    return name;
}

Templater::html::dtags::pre::pre(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::pre::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::pre::clone() const {
    return std::make_shared<pre>(*this);
}
const std::string& Templater::html::dtags::progress::getTagName() const {
    static const std::string name = "progress";
    return name;
}

Templater::html::dtags::progress::progress(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::progress::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::progress::clone() const {
    return std::make_shared<progress>(*this);
}
const std::string& Templater::html::dtags::q::getTagName() const {
    static const std::string name = "q";
    return name;
}

Templater::html::dtags::q::q(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::q::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::q::clone() const {
    return std::make_shared<q>(*this);
}
const std::string& Templater::html::dtags::rb::getTagName() const {
    static const std::string name = "rb";
    return name;
}

Templater::html::dtags::rb::rb(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::rb::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::rb::clone() const {
    return std::make_shared<rb>(*this);
}
const std::string& Templater::html::dtags::rp::getTagName() const {
    static const std::string name = "rp";
    return name;
}

Templater::html::dtags::rp::rp(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::rp::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::rp::clone() const {
    return std::make_shared<rp>(*this);
}
const std::string& Templater::html::dtags::rt::getTagName() const {
    static const std::string name = "rt";
    return name;
}

Templater::html::dtags::rt::rt(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::rt::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::rt::clone() const {
    return std::make_shared<rt>(*this);
}
const std::string& Templater::html::dtags::rtc::getTagName() const {
    static const std::string name = "rtc";
    return name;
}

Templater::html::dtags::rtc::rtc(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::rtc::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::rtc::clone() const {
    return std::make_shared<rtc>(*this);
}
const std::string& Templater::html::dtags::ruby::getTagName() const {
    static const std::string name = "ruby";
    return name;
}

Templater::html::dtags::ruby::ruby(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::ruby::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::ruby::clone() const {
    return std::make_shared<ruby>(*this);
}
const std::string& Templater::html::dtags::s::getTagName() const {
    static const std::string name = "s";
    return name;
}

Templater::html::dtags::s::s(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::s::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::s::clone() const {
    return std::make_shared<s>(*this);
}
const std::string& Templater::html::dtags::samp::getTagName() const {
    static const std::string name = "samp";
    return name;
}

Templater::html::dtags::samp::samp(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::samp::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::samp::clone() const {
    return std::make_shared<samp>(*this);
}
const std::string& Templater::html::dtags::script::getTagName() const {
    static const std::string name = "script";
    return name;
}

Templater::html::dtags::script::script(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::script::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::script::clone() const {
    return std::make_shared<script>(*this);
}
const std::string& Templater::html::dtags::search::getTagName() const {
    static const std::string name = "search";
    return name;
}

Templater::html::dtags::search::search(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::search::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::search::clone() const {
    return std::make_shared<search>(*this);
}
const std::string& Templater::html::dtags::section::getTagName() const {
    static const std::string name = "section";
    return name;
}

Templater::html::dtags::section::section(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::section::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::section::clone() const {
    return std::make_shared<section>(*this);
}
const std::string& Templater::html::dtags::select::getTagName() const {
    static const std::string name = "select";
    return name;
}

Templater::html::dtags::select::select(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::select::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::select::clone() const {
    return std::make_shared<select>(*this);
}
const std::string& Templater::html::dtags::slot::getTagName() const {
    static const std::string name = "slot";
    return name;
}

Templater::html::dtags::slot::slot(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::slot::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::slot::clone() const {
    return std::make_shared<slot>(*this);
}
const std::string& Templater::html::dtags::small::getTagName() const {
    static const std::string name = "small";
    return name;
}

Templater::html::dtags::small::small(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::small::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::small::clone() const {
    return std::make_shared<small>(*this);
}
const std::string& Templater::html::dtags::source::getTagName() const {
    static const std::string name = "source";
    return name;
}

Templater::html::dtags::source::source(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::source::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::source::clone() const {
    return std::make_shared<source>(*this);
}
const std::string& Templater::html::dtags::span::getTagName() const {
    static const std::string name = "span";
    return name;
}

Templater::html::dtags::span::span(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::span::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::span::clone() const {
    return std::make_shared<span>(*this);
}
const std::string& Templater::html::dtags::strong::getTagName() const {
    static const std::string name = "strong";
    return name;
}

Templater::html::dtags::strong::strong(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::strong::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::strong::clone() const {
    return std::make_shared<strong>(*this);
}
const std::string& Templater::html::dtags::style::getTagName() const {
    static const std::string name = "style";
    return name;
}

Templater::html::dtags::style::style(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::style::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::style::clone() const {
    return std::make_shared<style>(*this);
}
const std::string& Templater::html::dtags::sub::getTagName() const {
    static const std::string name = "sub";
    return name;
}

Templater::html::dtags::sub::sub(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::sub::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::sub::clone() const {
    return std::make_shared<sub>(*this);
}
const std::string& Templater::html::dtags::summary::getTagName() const {
    static const std::string name = "summary";
    return name;
}

Templater::html::dtags::summary::summary(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::summary::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::summary::clone() const {
    return std::make_shared<summary>(*this);
}
const std::string& Templater::html::dtags::sup::getTagName() const {
    static const std::string name = "sup";
    return name;
}

Templater::html::dtags::sup::sup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::sup::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::sup::clone() const {
    return std::make_shared<sup>(*this);
}
const std::string& Templater::html::dtags::svg::getTagName() const {
    static const std::string name = "svg";
    return name;
}

Templater::html::dtags::svg::svg(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::svg::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::svg::clone() const {
    return std::make_shared<svg>(*this);
}
const std::string& Templater::html::dtags::table::getTagName() const {
    static const std::string name = "table";
    return name;
}

Templater::html::dtags::table::table(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::table::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::table::clone() const {
    return std::make_shared<table>(*this);
}
const std::string& Templater::html::dtags::tbody::getTagName() const {
    static const std::string name = "tbody";
    return name;
}

Templater::html::dtags::tbody::tbody(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::tbody::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::tbody::clone() const {
    return std::make_shared<tbody>(*this);
}
const std::string& Templater::html::dtags::td::getTagName() const {
    static const std::string name = "td";
    return name;
}

Templater::html::dtags::td::td(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::td::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::td::clone() const {
    return std::make_shared<td>(*this);
}
const std::string& Templater::html::dtags::htemplate::getTagName() const {
    static const std::string name = "template";
    return name;
}

Templater::html::dtags::htemplate::htemplate(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::htemplate::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::htemplate::clone() const {
    return std::make_shared<htemplate>(*this);
}
const std::string& Templater::html::dtags::textarea::getTagName() const {
    static const std::string name = "textarea";
    return name;
}

Templater::html::dtags::textarea::textarea(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::textarea::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::textarea::clone() const {
    return std::make_shared<textarea>(*this);
}
const std::string& Templater::html::dtags::tfoot::getTagName() const {
    static const std::string name = "tfoot";
    return name;
}

Templater::html::dtags::tfoot::tfoot(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::tfoot::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::tfoot::clone() const {
    return std::make_shared<tfoot>(*this);
}
const std::string& Templater::html::dtags::th::getTagName() const {
    static const std::string name = "th";
    return name;
}

Templater::html::dtags::th::th(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::th::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::th::clone() const {
    return std::make_shared<th>(*this);
}
const std::string& Templater::html::dtags::thead::getTagName() const {
    static const std::string name = "thead";
    return name;
}

Templater::html::dtags::thead::thead(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::thead::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::thead::clone() const {
    return std::make_shared<thead>(*this);
}
const std::string& Templater::html::dtags::time::getTagName() const {
    static const std::string name = "time";
    return name;
}

Templater::html::dtags::time::time(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::time::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::time::clone() const {
    return std::make_shared<time>(*this);
}
const std::string& Templater::html::dtags::title::getTagName() const {
    static const std::string name = "title";
    return name;
}

Templater::html::dtags::title::title(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::title::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::title::clone() const {
    return std::make_shared<title>(*this);
}
const std::string& Templater::html::dtags::tr::getTagName() const {
    static const std::string name = "tr";
    return name;
}

Templater::html::dtags::tr::tr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::tr::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::tr::clone() const {
    return std::make_shared<tr>(*this);
}
const std::string& Templater::html::dtags::track::getTagName() const {
    static const std::string name = "track";
    return name;
}

Templater::html::dtags::track::track(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::track::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::track::clone() const {
    return std::make_shared<track>(*this);
}
const std::string& Templater::html::dtags::u::getTagName() const {
    static const std::string name = "u";
    return name;
}

Templater::html::dtags::u::u(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::u::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::u::clone() const {
    return std::make_shared<u>(*this);
}
const std::string& Templater::html::dtags::ul::getTagName() const {
    static const std::string name = "ul";
    return name;
}

Templater::html::dtags::ul::ul(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::ul::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::ul::clone() const {
    return std::make_shared<ul>(*this);
}
const std::string& Templater::html::dtags::var::getTagName() const {
    static const std::string name = "var";
    return name;
}

Templater::html::dtags::var::var(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::var::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::var::clone() const {
    return std::make_shared<var>(*this);
}
const std::string& Templater::html::dtags::video::getTagName() const {
    static const std::string name = "video";
    return name;
}

Templater::html::dtags::video::video(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::video::isVoid() const {
    return 0;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::video::clone() const {
    return std::make_shared<video>(*this);
}
const std::string& Templater::html::dtags::wbr::getTagName() const {
    static const std::string name = "wbr";
    return name;
}

Templater::html::dtags::wbr::wbr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
   : Templater::html::Object(std::move(attributes), std::move(children)) {}

bool Templater::html::dtags::wbr::isVoid() const {
    return 1;
}

std::shared_ptr<Templater::html::Object> Templater::html::dtags::wbr::clone() const {
    return std::make_shared<wbr>(*this);
}
