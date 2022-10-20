#include "src/rb_tree.hpp"
#include "src/stack.hpp"
#include "src/vector.hpp"
#include "src/map.hpp"

#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
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

int main() {
    srand( time( 0 ) );

    ft::rb_tree< std::pair< std::string, int >, cmp< std::string, int > > map;
    map.insert( std::make_pair( "E", 6 ) );
    map.insert( std::make_pair( "A", 9 ) );
    map.insert( std::make_pair( "AA", 9 ) );
    map.insert( std::make_pair( "B", 9 ) );
    map.insert( std::make_pair( "IIIIIIIII", 9 ) );
    for ( ft::rb_tree< std::pair< std::string, int >,
                       cmp< std::string, int > >::const_reverse_iterator rit
          = map.rbegin();
           rit != map.rend() ;
          rit++ ) {
        std::cout << rit->first << std::endl;
    }
    std::cout << map.find(std::make_pair("A", int()))->second << std::endl;

    ft::rb_tree< std::pair< std::string, int >, cmp< std::string, int > >::iterator cit = map.find(std::make_pair("A", int()));
    cit->second = 42;
        std::cout << map.find(std::make_pair("A", int()))->second << std::endl;

}
