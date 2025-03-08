#pragma once
#include "html_object.h" 

namespace Templater::html::dtags {
    using namespace Templater::html; 
    class a: public Object {
        public:
            template <typename... Args>
            explicit a(Args&&... args);
            explicit a(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class abbr: public Object {
        public:
            template <typename... Args>
            explicit abbr(Args&&... args);
            explicit abbr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class address: public Object {
        public:
            template <typename... Args>
            explicit address(Args&&... args);
            explicit address(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class area: public Object {
        public:
            template <typename... Args>
            explicit area(Args&&... args);
            explicit area(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class article: public Object {
        public:
            template <typename... Args>
            explicit article(Args&&... args);
            explicit article(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class aside: public Object {
        public:
            template <typename... Args>
            explicit aside(Args&&... args);
            explicit aside(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class audio: public Object {
        public:
            template <typename... Args>
            explicit audio(Args&&... args);
            explicit audio(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class b: public Object {
        public:
            template <typename... Args>
            explicit b(Args&&... args);
            explicit b(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class base: public Object {
        public:
            template <typename... Args>
            explicit base(Args&&... args);
            explicit base(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class bdi: public Object {
        public:
            template <typename... Args>
            explicit bdi(Args&&... args);
            explicit bdi(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class bdo: public Object {
        public:
            template <typename... Args>
            explicit bdo(Args&&... args);
            explicit bdo(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class blockquote: public Object {
        public:
            template <typename... Args>
            explicit blockquote(Args&&... args);
            explicit blockquote(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class body: public Object {
        public:
            template <typename... Args>
            explicit body(Args&&... args);
            explicit body(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class br: public Object {
        public:
            template <typename... Args>
            explicit br(Args&&... args);
            explicit br(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class button: public Object {
        public:
            template <typename... Args>
            explicit button(Args&&... args);
            explicit button(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class canvas: public Object {
        public:
            template <typename... Args>
            explicit canvas(Args&&... args);
            explicit canvas(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class caption: public Object {
        public:
            template <typename... Args>
            explicit caption(Args&&... args);
            explicit caption(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class cite: public Object {
        public:
            template <typename... Args>
            explicit cite(Args&&... args);
            explicit cite(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class code: public Object {
        public:
            template <typename... Args>
            explicit code(Args&&... args);
            explicit code(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class col: public Object {
        public:
            template <typename... Args>
            explicit col(Args&&... args);
            explicit col(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class colgroup: public Object {
        public:
            template <typename... Args>
            explicit colgroup(Args&&... args);
            explicit colgroup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class data: public Object {
        public:
            template <typename... Args>
            explicit data(Args&&... args);
            explicit data(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class datalist: public Object {
        public:
            template <typename... Args>
            explicit datalist(Args&&... args);
            explicit datalist(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class dd: public Object {
        public:
            template <typename... Args>
            explicit dd(Args&&... args);
            explicit dd(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class del: public Object {
        public:
            template <typename... Args>
            explicit del(Args&&... args);
            explicit del(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class details: public Object {
        public:
            template <typename... Args>
            explicit details(Args&&... args);
            explicit details(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class dfn: public Object {
        public:
            template <typename... Args>
            explicit dfn(Args&&... args);
            explicit dfn(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class dialog: public Object {
        public:
            template <typename... Args>
            explicit dialog(Args&&... args);
            explicit dialog(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class div: public Object {
        public:
            template <typename... Args>
            explicit div(Args&&... args);
            explicit div(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class dl: public Object {
        public:
            template <typename... Args>
            explicit dl(Args&&... args);
            explicit dl(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class dt: public Object {
        public:
            template <typename... Args>
            explicit dt(Args&&... args);
            explicit dt(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class em: public Object {
        public:
            template <typename... Args>
            explicit em(Args&&... args);
            explicit em(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class embed: public Object {
        public:
            template <typename... Args>
            explicit embed(Args&&... args);
            explicit embed(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class fieldset: public Object {
        public:
            template <typename... Args>
            explicit fieldset(Args&&... args);
            explicit fieldset(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class figcaption: public Object {
        public:
            template <typename... Args>
            explicit figcaption(Args&&... args);
            explicit figcaption(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class figure: public Object {
        public:
            template <typename... Args>
            explicit figure(Args&&... args);
            explicit figure(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class footer: public Object {
        public:
            template <typename... Args>
            explicit footer(Args&&... args);
            explicit footer(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class form: public Object {
        public:
            template <typename... Args>
            explicit form(Args&&... args);
            explicit form(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class h1: public Object {
        public:
            template <typename... Args>
            explicit h1(Args&&... args);
            explicit h1(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class h2: public Object {
        public:
            template <typename... Args>
            explicit h2(Args&&... args);
            explicit h2(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class h3: public Object {
        public:
            template <typename... Args>
            explicit h3(Args&&... args);
            explicit h3(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class h4: public Object {
        public:
            template <typename... Args>
            explicit h4(Args&&... args);
            explicit h4(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class h5: public Object {
        public:
            template <typename... Args>
            explicit h5(Args&&... args);
            explicit h5(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class h6: public Object {
        public:
            template <typename... Args>
            explicit h6(Args&&... args);
            explicit h6(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class head: public Object {
        public:
            template <typename... Args>
            explicit head(Args&&... args);
            explicit head(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class header: public Object {
        public:
            template <typename... Args>
            explicit header(Args&&... args);
            explicit header(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class hgroup: public Object {
        public:
            template <typename... Args>
            explicit hgroup(Args&&... args);
            explicit hgroup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class hr: public Object {
        public:
            template <typename... Args>
            explicit hr(Args&&... args);
            explicit hr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class html: public Object {
        public:
            template <typename... Args>
            explicit html(Args&&... args);
            explicit html(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class i: public Object {
        public:
            template <typename... Args>
            explicit i(Args&&... args);
            explicit i(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class iframe: public Object {
        public:
            template <typename... Args>
            explicit iframe(Args&&... args);
            explicit iframe(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class img: public Object {
        public:
            template <typename... Args>
            explicit img(Args&&... args);
            explicit img(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class input: public Object {
        public:
            template <typename... Args>
            explicit input(Args&&... args);
            explicit input(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class ins: public Object {
        public:
            template <typename... Args>
            explicit ins(Args&&... args);
            explicit ins(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class kbd: public Object {
        public:
            template <typename... Args>
            explicit kbd(Args&&... args);
            explicit kbd(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class label: public Object {
        public:
            template <typename... Args>
            explicit label(Args&&... args);
            explicit label(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class legend: public Object {
        public:
            template <typename... Args>
            explicit legend(Args&&... args);
            explicit legend(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class li: public Object {
        public:
            template <typename... Args>
            explicit li(Args&&... args);
            explicit li(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class link: public Object {
        public:
            template <typename... Args>
            explicit link(Args&&... args);
            explicit link(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class main: public Object {
        public:
            template <typename... Args>
            explicit main(Args&&... args);
            explicit main(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class map: public Object {
        public:
            template <typename... Args>
            explicit map(Args&&... args);
            explicit map(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class mark: public Object {
        public:
            template <typename... Args>
            explicit mark(Args&&... args);
            explicit mark(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class math: public Object {
        public:
            template <typename... Args>
            explicit math(Args&&... args);
            explicit math(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class menu: public Object {
        public:
            template <typename... Args>
            explicit menu(Args&&... args);
            explicit menu(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class menuitem: public Object {
        public:
            template <typename... Args>
            explicit menuitem(Args&&... args);
            explicit menuitem(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class meta: public Object {
        public:
            template <typename... Args>
            explicit meta(Args&&... args);
            explicit meta(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class meter: public Object {
        public:
            template <typename... Args>
            explicit meter(Args&&... args);
            explicit meter(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class nav: public Object {
        public:
            template <typename... Args>
            explicit nav(Args&&... args);
            explicit nav(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class noscript: public Object {
        public:
            template <typename... Args>
            explicit noscript(Args&&... args);
            explicit noscript(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class object: public Object {
        public:
            template <typename... Args>
            explicit object(Args&&... args);
            explicit object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class ol: public Object {
        public:
            template <typename... Args>
            explicit ol(Args&&... args);
            explicit ol(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class optgroup: public Object {
        public:
            template <typename... Args>
            explicit optgroup(Args&&... args);
            explicit optgroup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class option: public Object {
        public:
            template <typename... Args>
            explicit option(Args&&... args);
            explicit option(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class output: public Object {
        public:
            template <typename... Args>
            explicit output(Args&&... args);
            explicit output(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class p: public Object {
        public:
            template <typename... Args>
            explicit p(Args&&... args);
            explicit p(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class param: public Object {
        public:
            template <typename... Args>
            explicit param(Args&&... args);
            explicit param(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class picture: public Object {
        public:
            template <typename... Args>
            explicit picture(Args&&... args);
            explicit picture(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class pre: public Object {
        public:
            template <typename... Args>
            explicit pre(Args&&... args);
            explicit pre(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class progress: public Object {
        public:
            template <typename... Args>
            explicit progress(Args&&... args);
            explicit progress(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class q: public Object {
        public:
            template <typename... Args>
            explicit q(Args&&... args);
            explicit q(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class rb: public Object {
        public:
            template <typename... Args>
            explicit rb(Args&&... args);
            explicit rb(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class rp: public Object {
        public:
            template <typename... Args>
            explicit rp(Args&&... args);
            explicit rp(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class rt: public Object {
        public:
            template <typename... Args>
            explicit rt(Args&&... args);
            explicit rt(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class rtc: public Object {
        public:
            template <typename... Args>
            explicit rtc(Args&&... args);
            explicit rtc(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class ruby: public Object {
        public:
            template <typename... Args>
            explicit ruby(Args&&... args);
            explicit ruby(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class s: public Object {
        public:
            template <typename... Args>
            explicit s(Args&&... args);
            explicit s(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class samp: public Object {
        public:
            template <typename... Args>
            explicit samp(Args&&... args);
            explicit samp(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class script: public Object {
        public:
            template <typename... Args>
            explicit script(Args&&... args);
            explicit script(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class search: public Object {
        public:
            template <typename... Args>
            explicit search(Args&&... args);
            explicit search(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class section: public Object {
        public:
            template <typename... Args>
            explicit section(Args&&... args);
            explicit section(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class select: public Object {
        public:
            template <typename... Args>
            explicit select(Args&&... args);
            explicit select(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class slot: public Object {
        public:
            template <typename... Args>
            explicit slot(Args&&... args);
            explicit slot(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class small: public Object {
        public:
            template <typename... Args>
            explicit small(Args&&... args);
            explicit small(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class source: public Object {
        public:
            template <typename... Args>
            explicit source(Args&&... args);
            explicit source(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class span: public Object {
        public:
            template <typename... Args>
            explicit span(Args&&... args);
            explicit span(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class strong: public Object {
        public:
            template <typename... Args>
            explicit strong(Args&&... args);
            explicit strong(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class style: public Object {
        public:
            template <typename... Args>
            explicit style(Args&&... args);
            explicit style(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class sub: public Object {
        public:
            template <typename... Args>
            explicit sub(Args&&... args);
            explicit sub(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class summary: public Object {
        public:
            template <typename... Args>
            explicit summary(Args&&... args);
            explicit summary(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class sup: public Object {
        public:
            template <typename... Args>
            explicit sup(Args&&... args);
            explicit sup(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class svg: public Object {
        public:
            template <typename... Args>
            explicit svg(Args&&... args);
            explicit svg(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class table: public Object {
        public:
            template <typename... Args>
            explicit table(Args&&... args);
            explicit table(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class tbody: public Object {
        public:
            template <typename... Args>
            explicit tbody(Args&&... args);
            explicit tbody(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class td: public Object {
        public:
            template <typename... Args>
            explicit td(Args&&... args);
            explicit td(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class htemplate: public Object {
        public:
            template <typename... Args>
            explicit htemplate(Args&&... args);
            explicit htemplate(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class textarea: public Object {
        public:
            template <typename... Args>
            explicit textarea(Args&&... args);
            explicit textarea(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class tfoot: public Object {
        public:
            template <typename... Args>
            explicit tfoot(Args&&... args);
            explicit tfoot(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class th: public Object {
        public:
            template <typename... Args>
            explicit th(Args&&... args);
            explicit th(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class thead: public Object {
        public:
            template <typename... Args>
            explicit thead(Args&&... args);
            explicit thead(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class time: public Object {
        public:
            template <typename... Args>
            explicit time(Args&&... args);
            explicit time(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class title: public Object {
        public:
            template <typename... Args>
            explicit title(Args&&... args);
            explicit title(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class tr: public Object {
        public:
            template <typename... Args>
            explicit tr(Args&&... args);
            explicit tr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class track: public Object {
        public:
            template <typename... Args>
            explicit track(Args&&... args);
            explicit track(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class u: public Object {
        public:
            template <typename... Args>
            explicit u(Args&&... args);
            explicit u(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class ul: public Object {
        public:
            template <typename... Args>
            explicit ul(Args&&... args);
            explicit ul(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class var: public Object {
        public:
            template <typename... Args>
            explicit var(Args&&... args);
            explicit var(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class video: public Object {
        public:
            template <typename... Args>
            explicit video(Args&&... args);
            explicit video(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
    class wbr: public Object {
        public:
            template <typename... Args>
            explicit wbr(Args&&... args);
            explicit wbr(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            bool isVoid() const override;
            std::shared_ptr<Object> clone() const override;
            const std::string& getTagName() const override;
    };
}

template <typename... Args>
Templater::html::dtags::a::a(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::abbr::abbr(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::address::address(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::area::area(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::article::article(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::aside::aside(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::audio::audio(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::b::b(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::base::base(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::bdi::bdi(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::bdo::bdo(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::blockquote::blockquote(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::body::body(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::br::br(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::button::button(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::canvas::canvas(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::caption::caption(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::cite::cite(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::code::code(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::col::col(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::colgroup::colgroup(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::data::data(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::datalist::datalist(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::dd::dd(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::del::del(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::details::details(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::dfn::dfn(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::dialog::dialog(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::div::div(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::dl::dl(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::dt::dt(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::em::em(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::embed::embed(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::fieldset::fieldset(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::figcaption::figcaption(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::figure::figure(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::footer::footer(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::form::form(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::h1::h1(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::h2::h2(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::h3::h3(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::h4::h4(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::h5::h5(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::h6::h6(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::head::head(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::header::header(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::hgroup::hgroup(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::hr::hr(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::html::html(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::i::i(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::iframe::iframe(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::img::img(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::input::input(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::ins::ins(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::kbd::kbd(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::label::label(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::legend::legend(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::li::li(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::link::link(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::main::main(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::map::map(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::mark::mark(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::math::math(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::menu::menu(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::menuitem::menuitem(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::meta::meta(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::meter::meter(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::nav::nav(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::noscript::noscript(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::object::object(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::ol::ol(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::optgroup::optgroup(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::option::option(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::output::output(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::p::p(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::param::param(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::picture::picture(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::pre::pre(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::progress::progress(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::q::q(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::rb::rb(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::rp::rp(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::rt::rt(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::rtc::rtc(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::ruby::ruby(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::s::s(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::samp::samp(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::script::script(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::search::search(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::section::section(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::select::select(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::slot::slot(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::small::small(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::source::source(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::span::span(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::strong::strong(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::style::style(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::sub::sub(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::summary::summary(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::sup::sup(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::svg::svg(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::table::table(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::tbody::tbody(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::td::td(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::htemplate::htemplate(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::textarea::textarea(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::tfoot::tfoot(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::th::th(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::thead::thead(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::time::time(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::title::title(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::tr::tr(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::track::track(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::u::u(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::ul::ul(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::var::var(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::video::video(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::html::dtags::wbr::wbr(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}

