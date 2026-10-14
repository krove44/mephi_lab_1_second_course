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
public:
    avl_tree() : head(nullptr){};
    avl_tree(Node<T> node) : head(node){};
    avl_tree(T k) : head(std::make_unique<Node<T>>(k)) {};

    void add(T val) {
        Node<T>* bk = head.get();
        Node<T>* start = head.get();
        while (start != nullptr) {
            if (val > start->key) {
                bk = start;
                start = start->right.get();

            }
            else {
                bk = start;
                start = start->left.get();
            }
        }
        if (val > bk->key) {
            bk->right = std::make_unique<Node<T>>(val);
            //тут надо высоту считать и перестраивать
        }
        else {
            bk->left = std::make_unique<Node<T>>(val);
        }
    }


};