#include "src/map.hpp"
#include "src/set.hpp"
#include "src/stack.hpp"
#include "src/vector.hpp"

#include <iostream>
#include <map>
#include <set>
#include <vector>

#ifndef NS
#define NS std
#endif

/* ------------------------ ostream operator overload ----------------------- */

template < typename T >
std::ostream &operator<<( std::ostream &os, const NS::vector< T > &v ) {
    os << "[";
    for ( typename NS::vector< T >::const_iterator it = v.begin();
          it != v.end();
          it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]";
    return os;
}

template < typename T1, typename T2 >
std::ostream &operator<<( std::ostream &os, const NS::pair< T1, T2 > &p ) {
    os << p.first << ": " << p.second;
    return os;
}

template < typename K, typename V >
std::ostream &operator<<( std::ostream &os, const NS::map< K, V > &m ) {
    os << "{";
    for ( typename NS::map< K, V >::const_iterator it = m.begin();
          it != m.end();
          it++ ) {
        if ( it != m.begin() ) { os << ", "; }
        os << *it;
    }
    os << "}";
    return os;
}

template < typename T >
std::ostream &operator<<( std::ostream &os, const NS::set< T > s ) {
    os << "{";
    for ( typename NS::set< T >::const_iterator it = s.begin(); it != s.end();
          it++ ) {
        if ( it != s.begin() ) { os << ", "; }
        os << *it;
    }
    os << "}";
    return os;
}

/* -------------------------------------------------------------------------- */

int main() {
    /* --------------------------------- Vector --------------------------------- */
    {
        /* ------------------------------ Construction ------------------------------ */
        NS::vector< int > v1;
        NS::vector< int > v2( 15 );
        NS::vector< int > v3( 8, 42 );
        int               arr[] = { 1, 2, 3 };
        NS::vector< int > v4( arr, arr + sizeof( arr ) / sizeof( int ) );
        NS::vector< int > v5( 4 );

        std::cout << v1 << std::endl;
        std::cout << v2 << std::endl;
        std::cout << v3 << std::endl;
        std::cout << v4 << std::endl;
        std::cout << v5 << std::endl;

        v1 = v3;
        v2 = v1;

        std::cout << v1 << std::endl;
        std::cout << v2 << std::endl;
    }
    /* ----------------------------------- Map ---------------------------------- */
    {

    }
    /* ----------------------------------- Set ---------------------------------- */
    {}
}
