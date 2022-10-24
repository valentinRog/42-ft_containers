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
//#include <type_traits>
#include <utility>
#include <vector>

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
std::ostream &operator<<( std::ostream &os, const ft::pair< T, U > &p ) {
    os << p.first << " " << p.second;
    return os;
}
template < typename T, typename U >
std::ostream &operator<<( std::ostream &os, const std::pair< T, U > &p ) {
    os << p.first << " " << p.second;
    return os;
}

template < typename K, typename V > struct getter {
    typedef V return_type;
    V         operator()( std::pair< K, V > p ) const { return p.second; }
};

#define TESTED_NAMESPACE std

int main( void ) {

    TESTED_NAMESPACE::map< int, int > t1;
    TESTED_NAMESPACE::map< int, int > t2;


    t1[4] = 19;
    t2[2] = 9;

    auto it1 = t1.begin();
    auto it2 = t2.begin();

    t1.swap(t2);

    std::cout << *t1.begin() << std::endl;

}
