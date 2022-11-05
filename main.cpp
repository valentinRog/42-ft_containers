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
        #define VECTYPE char
        //CONSTRUCTORS
        NS::vector<VECTYPE> v1;
        NS::vector<VECTYPE> v2(4, 97);
        v1 = v2;
        NS::vector<VECTYPE> v3(v1.begin(),v1.end());
        NS::vector<VECTYPE> v4(v3);
        //ELEMENT ACCESS
        v4[1] = 65 + 1;
        v4.at(2) = 65 + 2;
        v4.front() = 65 + 0;
        v4.back() = 65 + 3;
        //ITERATORS
        for (NS::vector<VECTYPE>::reverse_iterator it = v4.rbegin(); it != v4.rend(); it++)
                *it += 2;
        //CAPACITY
        v4.resize(6, 70);
        v4.reserve(9);
        std::cout << v4.capacity() << std::endl;
        v4.push_back(72);
        v4.push_back(72);
        v4.push_back(72);
        v4.push_back(72);
        std::cout << v4.capacity() << std::endl;
}
