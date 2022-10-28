#include "src/map.hpp"
#include "src/rb_tree.hpp"
#include "src/set.hpp"
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

#define T1 int
#define TESTED_NAMESPACE std

struct ft_more {
    bool operator()( const T1 &first, const T1 &second ) const {
        return ( first > second );
    }
};

typedef TESTED_NAMESPACE::set< T1, ft_more >           ft_st;
typedef TESTED_NAMESPACE::set< T1, ft_more >::iterator ft_st_it;

int main( void ) {
    ft_st st;

    st.insert( 42 );
    st.insert( 25 );
    st.insert( 80 );
    st.insert( 12 );
    st.insert( 12 );
    st.insert( 27 );
    st.insert( 90 );
    st.insert( 25 );

    for ( ft_st::const_iterator it = st.begin(); it != st.end(); it++ ) {
        std::cout << *it << std::endl;
    }

    return ( 0 );
}