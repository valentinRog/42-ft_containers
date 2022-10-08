#include "src/vector.hpp"

#include <iostream>
#include <vector>

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
    ft::vector< int > v;
    v.push_back( 42 );
    v.push_back( 1 );
    v.push_back( 1 );
    v.push_back( 1 );
    // v.push_back( 1 );
    ft::vector<int>::iterator it = v.begin();
    // // it++;
    std::vector<int> v2;
    v2.push_back(1);
    v2.push_back(2);
    v2.push_back(3);
    v.assign(v2.begin(), v2.end());
    std::cout << v << std::endl;
}