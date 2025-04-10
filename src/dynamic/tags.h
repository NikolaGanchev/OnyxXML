#pragma once
#include "node.h" 
#include "void_node.h" 

namespace Templater::dynamic::dtags {
    using namespace Templater::dynamic; 
    class a: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class abbr: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class address: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class area: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class article: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class aside: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class audio: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class b: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class base: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class bdi: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class bdo: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class blockquote: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class body: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class br: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class button: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class canvas: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class caption: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class cite: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class code: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class col: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class colgroup: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class data: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class datalist: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class dd: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class del: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class details: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class dfn: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class dialog: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class div: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class dl: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class dt: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class em: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class embed: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class fieldset: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class figcaption: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class figure: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class footer: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class form: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class h1: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class h2: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class h3: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class h4: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class h5: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class h6: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class head: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class header: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class hgroup: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class hr: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class html: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class i: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class iframe: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class img: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class input: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class ins: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class kbd: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class label: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class legend: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class li: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class clink: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class main: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class map: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class mark: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class math: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class menu: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class menuitem: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class meta: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class meter: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class nav: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class noscript: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class object: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class ol: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class optgroup: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class option: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class output: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class p: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class param: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class picture: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class pre: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class progress: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class q: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class rb: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class rp: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class rt: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class rtc: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class ruby: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class s: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class samp: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class script: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class search: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class section: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class select: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class slot: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class small: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class source: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class span: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class strong: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class style: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class sub: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class summary: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class sup: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class svg: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class table: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class tbody: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class td: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class ctemplate: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class textarea: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class tfoot: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class th: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class thead: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class time: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class title: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class tr: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class track: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class u: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class ul: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class var: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class video: public Node {
        public:
            using Node::Node;
            bool isVoid() const override;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    class wbr: public VoidNode {
        public:
            using VoidNode::VoidNode;
            const std::string& getTagName() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
}

