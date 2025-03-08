#pragma once
#include "html_object.h" 

namespace Templater::dynamic::dtags {
    using namespace Templater::dynamic; 
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
    class ctemplate: public Object {
        public:
            template <typename... Args>
            explicit ctemplate(Args&&... args);
            explicit ctemplate(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
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
Templater::dynamic::dtags::a::a(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::abbr::abbr(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::address::address(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::area::area(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::article::article(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::aside::aside(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::audio::audio(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::b::b(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::base::base(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::bdi::bdi(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::bdo::bdo(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::blockquote::blockquote(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::body::body(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::br::br(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::button::button(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::canvas::canvas(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::caption::caption(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::cite::cite(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::code::code(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::col::col(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::colgroup::colgroup(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::data::data(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::datalist::datalist(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::dd::dd(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::del::del(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::details::details(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::dfn::dfn(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::dialog::dialog(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::div::div(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::dl::dl(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::dt::dt(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::em::em(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::embed::embed(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::fieldset::fieldset(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::figcaption::figcaption(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::figure::figure(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::footer::footer(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::form::form(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::h1::h1(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::h2::h2(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::h3::h3(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::h4::h4(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::h5::h5(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::h6::h6(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::head::head(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::header::header(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::hgroup::hgroup(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::hr::hr(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::html::html(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::i::i(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::iframe::iframe(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::img::img(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::input::input(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::ins::ins(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::kbd::kbd(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::label::label(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::legend::legend(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::li::li(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::link::link(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::main::main(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::map::map(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::mark::mark(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::math::math(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::menu::menu(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::menuitem::menuitem(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::meta::meta(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::meter::meter(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::nav::nav(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::noscript::noscript(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::object::object(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::ol::ol(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::optgroup::optgroup(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::option::option(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::output::output(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::p::p(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::param::param(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::picture::picture(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::pre::pre(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::progress::progress(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::q::q(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::rb::rb(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::rp::rp(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::rt::rt(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::rtc::rtc(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::ruby::ruby(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::s::s(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::samp::samp(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::script::script(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::search::search(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::section::section(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::select::select(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::slot::slot(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::small::small(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::source::source(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::span::span(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::strong::strong(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::style::style(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::sub::sub(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::summary::summary(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::sup::sup(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::svg::svg(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::table::table(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::tbody::tbody(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::td::td(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::ctemplate::ctemplate(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::textarea::textarea(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::tfoot::tfoot(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::th::th(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::thead::thead(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::time::time(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::title::title(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::tr::tr(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::track::track(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::u::u(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::ul::ul(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::var::var(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::video::video(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

template <typename... Args>
Templater::dynamic::dtags::wbr::wbr(Args&&... args)
    : Templater::dynamic::Object(std::forward<Args>(args)...) {}

