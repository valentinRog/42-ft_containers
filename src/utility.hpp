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

    bool operator==( const pair &other ) const {
        return ( first == other.first && second == other.second );
    }
    bool operator<( const pair &other ) const {
        return first < other.first
               || ( !( first < other.first ) && second < other.second );
    }
    bool operator!=( const pair &other ) const { return !( *this == other ); }
    bool operator<=( const pair &other ) const { return !( *this < other ); }
    bool operator>( const pair &other ) const { return ( other < *this ); }
    bool operator>=( const pair &other ) const { return !( *this < other ); }
};

template < class T1, class T2 > pair< T1, T2 > make_pair( T1 x, T2 y ) {
    return ( pair< T1, T2 >( x, y ) );
}

}
