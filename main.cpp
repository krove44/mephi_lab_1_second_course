#include <iostream>
#include "avl_tree.h"



int main() {
    avl_tree<int> sp(5);
    sp.add(6);
    sp.add(7);
    std::cout << sp.find(6) << std::endl;
    sp.remove(6);
    std::cout << sp.find(6) << std::endl;
    return 0;
}