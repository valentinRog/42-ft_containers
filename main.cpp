#include "src/map.hpp"
#include "src/rb_tree.hpp"
#include "src/stack.hpp"
#include "src/vector.hpp"

#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <type_traits>

template < typename T >
std::ostream &operator<<( std::ostream &os, const ft::vector< T > &v ) {
    os << "size: " << v.size() << std::endl;
    os << "capacity: " << v.capacity() << std::endl;
    os << "[";
    for ( typename ft::vector< T >::const_iterator it = v.begin();
          it != v.end();
          it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]";
    return os;
}

template < typename T >
std::ostream &operator<<( std::ostream &os, const std::vector< T > &v ) {
    os << "size: " << v.size() << std::endl;
    os << "capacity: " << v.capacity() << std::endl;
    os << "[";
    for ( typename std::vector< T >::const_iterator it = v.begin();
          it != v.end();
          it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]";
    return os;
}

template < typename T, typename U >
std::ostream &operator<<( std::ostream &os, const std::pair< T, U > &p ) {
    os << p.first << " " << p.second;
    return os;
}

template < typename T, typename U > struct cmp {
    bool operator()( const std::pair< T, U > &a,
                     const std::pair< T, U > &b ) const {
        return a.first < b.first;
    }
};

template < typename K, typename V > struct getter {
    typedef V return_type;
    V         operator()( std::pair< K, V > p ) const { return p.second; }
};

template < typename T, typename = typename std::enable_if< true >::type >
struct Yo {
    T a;

    Yo( T a ) : a( a ) {}
};

int main() {
    srand( time( 0 ) );

    typedef typename ft::rb_tree< int, int > tree_type;

    tree_type tree;
    tree.insert(tree_type::value_type(4, 5));
    tree.insert(tree_type::value_type(4, 8));
    for (tree_type::iterator it = tree.begin(); it != tree.end(); it++) {
        std::cout << *it << std::endl;
    }
}
