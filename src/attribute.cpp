#include "node.h"

#include <unordered_map>

namespace Templater::dynamic {

    Attribute::InternPool Attribute::internPool{};

    Attribute::Attribute(std::string name, std::string value, bool shouldEscape) 
        : m_shouldEscape(shouldEscape) {
        m_name = internPool.intern(std::move(name));
        m_value = internPool.intern(std::move(value));
    }

    Attribute::Attribute(std::string name): m_shouldEscape(true) {
        m_name = internPool.intern(std::move(name));
        m_value = internPool.intern(std::string(""));
    }

    Attribute::Attribute(const Attribute& other): m_shouldEscape(other.m_shouldEscape) {
        m_name = internPool.intern(other.m_name);
        m_value = internPool.intern(other.m_value);
    }

    Attribute::Attribute(Attribute&& other): m_shouldEscape(other.m_shouldEscape) {
        // Here, it is safe to just take ownership of the internal values without interning
        // Interning would just increment the ref count and then instantly decrement it as the temporary is destroyed
        m_name = other.m_name;
        m_value = other.m_value;
        other.m_name = nullptr;
        other.m_value = nullptr;
    }

    Attribute& Attribute::operator=(Attribute& other) {
        if (this != &other) {
            if (m_name != other.m_name) {
                internPool.drop(m_name);
                m_name = internPool.intern(other.m_name);
            }
            if (m_value != other.m_value) {
                internPool.drop(m_value);
                m_value = internPool.intern(other.m_value);
            }
            m_shouldEscape = other.m_shouldEscape;
        }

        return *this;
    }

    Attribute& Attribute::operator=(Attribute&& other) {
        internPool.drop(m_name);
        m_name = other.m_name;
        other.m_name = nullptr;

        internPool.drop(m_value);
        m_value = other.m_value;
        other.m_value = nullptr;

        m_shouldEscape = other.m_shouldEscape;

        return *this;
    }

    Attribute::~Attribute() {
        internPool.drop(m_name);
        internPool.drop(m_value);
    }

    void Attribute::setValue(const std::string& value) {
        if (value == *m_value) return;
        Attribute::internPool.drop(m_value);
        m_value = Attribute::internPool.intern(std::string(value));
    }

    const std::string& Attribute::getName() const {
        return *m_name;
    }

    const std::string& Attribute::getValue() const {
        return *m_value;
    }

    bool Attribute::shouldEscape() const {
        return m_shouldEscape;
    }

    std::size_t Attribute::InternPool::InternPoolKeyHash::operator()(const std::string* p) const {
        return std::hash<std::string>{}(*p); 
    }

    bool Attribute::InternPool::InternPoolKeyEquals::operator()(const std::string* p1, const std::string* p2) const {
        return *p1 == *p2; 
    }

    const std::string* Attribute::InternPool::intern(std::string&& str) {
        if (!m_pool.contains(&str)) {
            const auto& pair = m_pool.emplace(new std::string(std::move(str)), 1);
            return pair.first->first;
        }

        const auto& pair = m_pool.find(&str);
        pair->second++;
        return pair->first;
    }
    
    const std::string* Attribute::InternPool::intern(const std::string* str) {
        if (!m_pool.contains(str)) {
            m_pool.emplace(new std::string(*str), 0);
        }

        const auto& pair = m_pool.find(str);
        pair->second++;
        return pair->first;
    }

    void Attribute::InternPool::drop(const std::string* str) {
        if (!str) return;

        if (!m_pool.contains(str)) {
            return;
        }

        const auto& pair = m_pool.find(str);
        if (pair->second == 1) {
            delete (pair->first);
            m_pool.erase(pair);
            return;
        }

        pair->second--;
    }
    

    bool Attribute::equalsName(const Attribute& other) const {
        return m_name == other.m_name;
    }

    bool Attribute::equalsValue(const Attribute& other) const {
        return m_value == other.m_value;
    }

    size_t Attribute::InternPool::size() const {
        return m_pool.size();
    }

    size_t Attribute::getInternPoolSize() {
        return internPool.size();
    }
}