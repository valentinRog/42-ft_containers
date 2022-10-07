#include "src/vector.hpp"

#include <vector>
#include <iostream>

int main() {
    ft::vector<int> v;
    ft::vector<int>::reverse_iterator it = v.rbegin();
    ft::vector<int>::reverse_iterator it2 = v.rbegin();
    // *it = 4;
    std::cout << *it << std::endl;
    it++;
    it + 1;
    it - it2;
    std::cout << (it > it) << std::endl;
}