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

template < typename K, typename V > struct getter {
    typedef V return_type;
    V         operator()( std::pair< K, V > p ) const { return p.second; }
};

int main( void ) {

    ft::map<int, std::string> mp;

    mp[42] = "fgzgxfn";
    mp[25] = "funny";
    mp[80] = "hey";
    mp[12] = "no";
    mp[27] = "bee";
    mp[90] = "8";

    auto it = mp.end();

    auto it2 = mp.end();
    it2--;

    std::cout << (it == mp.find(35)) << std::endl;

    // ft_find( 90 );
    // ft_find( 100 );

    return ( 0 );
}
