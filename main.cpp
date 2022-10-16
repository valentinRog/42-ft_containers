#include "src/vector.hpp"
#include "src/stack.hpp"
#include "src/rb_tree.hpp"

#include <iostream>
#include <vector>
#include <string>

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
    tree.insert(3);
    tree.insert(21);
    tree.insert(32);
    tree.insert(15);
    std::cout << tree << std::endl;
}
