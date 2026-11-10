#pragma once
#include <memory>
#include <string>
#include <type_traits>
#include <stdexcept>
#include <algorithm>
#include <utility>

template<typename T, typename V>
struct Node_for_avl {
    std::shared_ptr<Node_for_avl<T, V>> left;
    std::shared_ptr<Node_for_avl<T, V>> right;
    T key;
    V value;
    int height;

    Node_for_avl(T k, V v)
        : left(nullptr), right(nullptr), key(std::move(k)), value(std::move(v)), height(0) {}
};

template <typename T, typename V>
class avl_tree {
    std::shared_ptr<Node_for_avl<T, V>> head;

    void rotate_right(std::shared_ptr<Node_for_avl<T, V>>& p) {
        std::shared_ptr<Node_for_avl<T, V>> x = std::move(p->left);
        p->left = std::move(x->right);
        update_height(p);
        x->right = std::move(p);
        update_height(x);
        p = std::move(x);
    }

    void rotate_left(std::shared_ptr<Node_for_avl<T, V>>& p) {
        std::shared_ptr<Node_for_avl<T, V>> x = std::move(p->right);
        p->right = std::move(x->left);
        update_height(p);
        x->left = std::move(p);
        update_height(x);
        p = std::move(x);
    }

    int get_height(const std::shared_ptr<Node_for_avl<T, V>>& p) const {
        if (!p) {
            return -1;
        }
        return p->height;
    }

    int balance_factor(const std::shared_ptr<Node_for_avl<T, V>>& p) const {
        if (!p) {
            return -1;
        }
        return get_height(p->left) - get_height(p->right);
    }

    void update_height(std::shared_ptr<Node_for_avl<T, V>>& p) {
        p->height = 1 + std::max(get_height(p->left), get_height(p->right));
    }

    void balance(std::shared_ptr<Node_for_avl<T, V>>& slot) {
        update_height(slot);
        int bf = balance_factor(slot);
        if (bf > 1) {
            if (balance_factor(slot->left) < 0) {
                rotate_left(slot->left);
            }
            rotate_right(slot);
        }
        else if (bf < -1) {
            if (balance_factor(slot->right) > 0) {
                rotate_right(slot->right);
            }
            rotate_left(slot);
        }
    }

    void rec_add(T key, V val, std::shared_ptr<Node_for_avl<T, V>>& slot) {
        if (!slot) {
            slot = std::make_shared<Node_for_avl<T, V>>(key, val);
        }
        else if (key > slot->key) {
            rec_add(key, val, slot->right);
        }
        else if (key < slot->key) {
            rec_add(key, val, slot->left);
        }
        else {
            slot->value = val;
        }
        balance(slot);
    }

    void rec_remove(T key, std::shared_ptr<Node_for_avl<T, V>>& slot) {
        if (!slot) {
            return;
        }
        if (key > slot->key) {
            rec_remove(key, slot->right);
        }
        else if (key < slot->key) {
            rec_remove(key, slot->left);
        }
        else {
            if (!slot->left) {
                slot = std::move(slot->right);
            }
            else if (!slot->right) {
                slot = std::move(slot->left);
            }
            else {
                std::shared_ptr<Node_for_avl<T, V>> start = slot->right;
                while (start->left) {
                    start = start->left;
                }
                std::swap(start->key, slot->key);
                std::swap(start->value, slot->value);
                rec_remove(key, slot->right);
            }
        }
        if (slot) balance(slot);
    }

    template <typename F>
    void in_order(const std::shared_ptr<Node_for_avl<T, V>>& n, F&& f) const {
        if (!n) {
            return;
        }
        in_order(n->left, f);
        f(n->key, n->value);
        in_order(n->right, f);
    }

    std::shared_ptr<Node_for_avl<T, V>> clone(const std::shared_ptr<Node_for_avl<T, V>>& n) const {
        if (!n) {
            return nullptr;
        }
        auto c = std::make_shared<Node_for_avl<T, V>>(n->key, n->value);
        c->height = n->height;
        c->left = clone(n->left);
        c->right = clone(n->right);
        return c;
    }

public:
    avl_tree() : head(nullptr) {}
    avl_tree(T k, V v) : head(std::make_shared<Node_for_avl<T, V>>(k, v)) {}
    avl_tree(std::shared_ptr<Node_for_avl<T, V>> root) : head(std::move(root)) {}

    // Глубокое копирование, чтобы shared_ptr не разделял узлы между независимыми деревьями
    avl_tree(const avl_tree& other) : head(clone(other.head)) {}
    avl_tree(avl_tree&& other) noexcept = default;

    avl_tree& operator=(const avl_tree& other) {
        if (this != &other) {
            head = clone(other.head);
        }
        return *this;
    }
    avl_tree& operator=(avl_tree&& other) noexcept = default;

    std::shared_ptr<const Node_for_avl<T, V>> get_root() const {
        return head;
    }

    bool empty() const {
        return head == nullptr;
    }

    void add(T key, V val) {
        rec_add(key, val, head);
    }

    bool find(T key) const {
        std::shared_ptr<Node_for_avl<T, V>> start = head;
        while (start) {
            if (key == start->key) {
                return true;
            }
            if (key > start->key) {
                start = start->right;
            }
            else {
                start = start->left;
            }
        }
        return false;
    }

    V& get(T key) const {
        std::shared_ptr<Node_for_avl<T, V>> start = head;
        while (start) {
            if (key == start->key) {
                return start->value;
            }
            if (key > start->key) {
                start = start->right;
            }
            else {
                start = start->left;
            }
        }
        throw std::runtime_error("Нет такого ключа");
    }

    void remove(T key) {
        rec_remove(key, head);
    }

    template <typename F>
    void for_each(F f) const {
        in_order(head, f);
    }

    template <typename P>
    avl_tree<T, V> where(P pred) const {
        avl_tree res;
        for_each([&](T& k, V& v) {
            if (pred(k, v)) res.add(k, v);
        });
        return res;
    }

    template <typename U, typename W, typename F>
    avl_tree<U, W> map(F f) const {
        avl_tree<U, W> res;
        for_each([&](T& k, V& v) {
            auto mapped = f(k, v);
            res.add(mapped.first, mapped.second);
        });
        return res;
    }

    template <typename A, typename F>
    A reduce(A init, F f) const {
        for_each([&](T& k, V& v) {
            init = f(init, k, v);
        });
        return init;
    }

    avl_tree subtree(const T& key) const {
        std::shared_ptr<Node_for_avl<T, V>> start = head;
        while (start && !(start->key == key)) {
            if (key > start->key) {
                start = start->right;
            }
            else {
                start = start->left;
            }
        }
        return avl_tree(clone(start));
    }

    std::string to_string() const {
        std::string sany = "";
        in_order(head, [&](T& k, V& v) {
            if constexpr (std::is_same_v<T, std::string>) {
                sany += k + " ";
            } else {
                sany += std::to_string(k) + " ";
            }
        });
        if (!sany.empty()) {
            sany.erase(sany.size() - 1);
        }
        return sany;
    }

    template<typename F>
    void from_string(const std::string& st, F f) {
        std::string el;
        for (const char it : st) {
            if (it != ' ') {
                el += it;
            }
            else {
                if (!el.empty()) {
                    add(f(el), f(el));
                    el.clear();
                }
            }
        }
        if (!el.empty()) {
            add(f(el), f(el));
        }
    }
};
