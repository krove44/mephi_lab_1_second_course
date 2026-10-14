#pragma once

template<typename T>
struct Node {
    Node* left;
    Node* right;
    T key;
    int height;
    Node(T k) : left(nullptr), right(nullptr), key(k), height(-1){};
};


template <typename T>
class avl_tree {
    Node<T>* head;
public:
    avl_tree() : head(nullptr){};
    avl_tree(Node<T> node) : head(node){};
    avl_tree(T k) : head(k){};

    void add(T val) {
        Node<T>* bk = head;
        Node<T>* start = head;
        while (start != nullptr) {
            if (val > start->key) {
                start = start->right;
            }
            else {
                start = start->left;
            }

        }
    }

};