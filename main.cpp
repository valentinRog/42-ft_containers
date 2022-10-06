#include "src/vector.hpp"

#include <vector>
#include <iostream>

int main() {
    ft::vector<int> v;
    ft::vector<int>::iterator it = v.begin();
     *it = 4;
    std::cout << *it << std::endl;
    it++;
    std::cout << *it << std::endl;
}