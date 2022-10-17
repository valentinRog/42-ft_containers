#include "src/vector.hpp"
#include "src/stack.hpp"
#include "src/rb_tree.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

template < typename T > std::ostream &operator<<( std::ostream &os, const ft::vector< T > &v ) {
    os << "size: " << v.size() << std::endl;
    os << "capacity: " << v.capacity() << std::endl;
    os << "[";
    for ( typename ft::vector< T >::const_iterator it = v.begin(); it != v.end(); it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]";
    return os;
}

template < typename T > std::ostream &operator<<( std::ostream &os, const std::vector< T > &v ) {
    os << "size: " << v.size() << std::endl;
    os << "capacity: " << v.capacity() << std::endl;
    os << "[";
    for ( typename std::vector< T >::const_iterator it = v.begin(); it != v.end(); it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]";
    return os;
}

int main() {
    ft::rb_tree<int> tree;
    //while (true) {
    //    std::stringstream ss;
    //    int n;
    //    std::string line;
    //    std::getline(std::cin, line);
    //    std::cout << "------------------------------------------------------" << std::endl;
    //    ss << line;
    //    ss >> n;
    //    tree.insert(n);
    //    std::cout << tree << std::endl;
    //    std::cout << "------------------------------------------------------" << std::endl;
    //}
    tree.insert(6);
    tree.insert(8);
    tree.insert(90);
    tree.insert(-36);
    std::cout << tree << std::endl;
    tree.remove(8);
    tree.remove(6);
    std::cout << tree << std::endl;
}
