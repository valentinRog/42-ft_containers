#pragma once

namespace ft {
template < typename T, typename V > struct pair {
    typedef T first_type;
    typedef V second_type;

    first_type  first;
    second_type second;

    pair( void ) : first(), second(){};
    template < typename W, typename X >
    pair( const pair< W, X > &other )
        : first( other.first ),
          second( other.second ){};
    pair( const first_type &first, const second_type &second )
        : first( first ),
          second( second ){};

    pair &operator=( const pair &other ) {
        first  = other.first;
        second = other.second;
        return ( *this );
    }
};

template < typename T, typename U >
bool operator==( const pair< T, U > &lhs, const pair< T, U > &rhs ) {
    return ( lhs.first == rhs.first && lhs.second == rhs.second );
}

template < typename T, typename U >
bool operator!=( const pair< T, U > &lhs, const pair< T, U > &rhs ) {
    return !( lhs == rhs );
}

template < typename T, typename U >
bool operator<( const pair< T, U > &lhs, const pair< T, U > &rhs ) {
    return lhs.first < rhs.first
           || ( !( rhs.first < lhs.first ) && lhs.second < rhs.second );
}

template < typename T, typename U >
bool operator<=( const pair< T, U > &lhs, const pair< T, U > &rhs ) {
    return !( rhs < lhs );
}

template < typename T, typename U >
bool operator>( const pair< T, U > &lhs, const pair< T, U > &rhs ) {
    return ( rhs < lhs );
}

template < typename T, typename U >
bool operator>=( const pair< T, U > &lhs, const pair< T, U > &rhs ) {
    return !( lhs < rhs );
}

}
