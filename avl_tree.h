#pragma once
#include <memory>

template<typename T>
struct Node {
    std::unique_ptr<Node<T>> left;
    std::unique_ptr<Node<T>> right;
    T key;
    int height;
    Node(T k) : left(nullptr), right(nullptr), key(k), height(0){};
};



template <typename T>
class avl_tree {
    std::unique_ptr<Node<T>> head;

    void rotate_right(std::unique_ptr<Node<T>>& p) {
        std::unique_ptr<Node<T>> x = std::move(p->left);
        p->left = std::move(x->right);
        update_height(p);
        x->right = std::move(p);
        update_height(x);
        p = std::move(x);
    }

    void rotate_left(std::unique_ptr<Node<T>>& p) {
        std::unique_ptr<Node<T>> x = std::move(p->right);
        p->right = std::move(x->left);
        update_height(p);
        x->left = std::move(p);
        update_height(x);
        p = std::move(x);
    }

    int get_height(std::unique_ptr<Node<T>>& p) {
        if (!p) {
            return -1;
        }
        return p->height;
    }

    int balance_factor(const std::unique_ptr<Node<T>>& p) {
        if (!p) {
            return -1;
        }
        return get_height(p->left) - get_height(p->right);
    }

    void update_height(std::unique_ptr<Node<T>>& p) {
        p->height = 1 + std::max(get_height(p->left), get_height(p->right));
    }

    void balance(std::unique_ptr<Node<T>>& slot) {
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

    void rec_add(T value, std::unique_ptr<Node<T>>& slot) {
        if (!slot) {
            slot = std::make_unique<Node<T>>(value);
        }
        else if (value > slot->key) {
            rec_add(value,slot->right);
        }
        else {
            rec_add(value,slot->left);
        }
        balance(slot);
    }

public:
    avl_tree() : head(nullptr){}
    avl_tree(T k) : head(std::make_unique<Node<T>>(k)) {}

    void add(T val) {
        rec_add(val, head);
    }


};