#include "src/map.hpp"
#include "src/set.hpp"
#include "src/stack.hpp"
#include "src/vector.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#ifndef NS
#define NS std
#endif

#define STREAM std::cout << "#" << __LINE__ << "  "

/* ---------------------------------- Data ---------------------------------- */

template < typename T > class A {
    T *_data;

public:
    A( T val = T() ) : _data( new T( val ) ) {}
    A( const A &other ) : _data( new T( *other._data ) ) {}
    A &operator=( const A &other ) {
        *_data = *other._data;
        return *this;
    }
    virtual ~A() { delete _data; }

    T &      data() { return *_data; }
    const T &data() const { return *_data; }

    bool operator==( const A &other ) const { return *_data == *other._data; }
    bool operator!=( const A &other ) const { return *_data != *other._data; }
    bool operator<( const A &other ) const { return *_data < *other._data; }
    bool operator>( const A &other ) const { return *_data > *other._data; }
    bool operator<=( const A &other ) const { return *_data <= *other._data; }
    bool operator>=( const A &other ) const { return *_data >= *other._data; }
};

/* --------------------------------- Functor -------------------------------- */

struct F {
    int operator()() { return ++_a.data(); }

private:
    A< int > _a;
};

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

template < typename T >
std::ostream &operator<<( std::ostream &os, const A< T > &a ) {
    os << a.data();
    return os;
}

/* -------------------------------------------------------------------------- */

int main() {
    /* --------------------------------- Vector --------------------------------- */
    {
        F                              f;
        typedef NS::vector< A< int > > vector_type;

        /* ------------------------------ Construction ------------------------------ */
        {

            const int arr[] = { f(), f(), f(), f(), f() };

            vector_type v1;
            vector_type v2( 7, f() );
            vector_type v3( arr, arr + sizeof( arr ) / sizeof( int ) );
            vector_type v4( v3 );

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;
            STREAM << v3 << std::endl;
            STREAM << v4 << std::endl;

            v4 = v2;
            v2 = v1;

            STREAM << v4 << std::endl;
            STREAM << v2 << std::endl;
        }
        /* -------------------------------- Iterators ------------------------------- */
        {
            const int   arr[] = { f(), f(), f(), f(), f() };
            vector_type v( arr, arr + sizeof( arr ) / sizeof( int ) );

            {
                vector_type::iterator it1;
                vector_type::iterator it2 = v.begin();
                vector_type::iterator it3( it2 );

                it1 = it2;

                STREAM << *it1 << std::endl;
                STREAM << *it2 << std::endl;
                STREAM << *it3 << std::endl;

                vector_type::const_iterator cit1;
                vector_type::const_iterator cit2 = v.begin();
                vector_type::const_iterator cit3( cit2 );
                vector_type::const_iterator cit4( it1 );
                vector_type::const_iterator cit5;

                cit1 = cit2;
                cit5 = it1;

                STREAM << *cit1 << std::endl;
                STREAM << *cit2 << std::endl;
                STREAM << *cit3 << std::endl;
                STREAM << *cit4 << std::endl;
                STREAM << *cit5 << std::endl;

                vector_type::reverse_iterator rit1;
                vector_type::reverse_iterator rit2 = v.rbegin();
                vector_type::reverse_iterator rit3( rit2 );

                rit1 = rit2;

                STREAM << *rit1 << std::endl;
                STREAM << *rit2 << std::endl;
                STREAM << *rit3 << std::endl;

                vector_type::const_reverse_iterator crit1;
                vector_type::const_reverse_iterator crit2 = v.rbegin();
                vector_type::const_reverse_iterator crit3( crit2 );
                vector_type::const_reverse_iterator crit4( rit1 );
                vector_type::const_reverse_iterator crit5;

                crit1 = crit2;
                crit5 = rit1;

                STREAM << *crit1 << std::endl;
                STREAM << *crit2 << std::endl;
                STREAM << *crit3 << std::endl;
                STREAM << *crit4 << std::endl;
                STREAM << *crit5 << std::endl;
            }
            {
                vector_type::iterator it = v.begin();

                STREAM << *it << std::endl;
                STREAM << *++it << std::endl;
                STREAM << *it++ << std::endl;
                STREAM << *it << std::endl;
                STREAM << *--it << std::endl;
                STREAM << *it-- << std::endl;
                STREAM << *it << std::endl;
                STREAM << *( it++ + 3 ) << std::endl;
                STREAM << *( 2 + it ) << std::endl;
                STREAM << *( it - 1 ) << std::endl;
                STREAM << it - v.begin() << std::endl;
                STREAM << it[3] << std::endl;
                STREAM << *( it += 3 ) << std::endl;
                STREAM << *( it -= 2 ) << std::endl;
                STREAM << it->data() << std::endl;

                vector_type::const_iterator cit = v.begin();

                STREAM << *cit << std::endl;
                STREAM << *++cit << std::endl;
                STREAM << *cit++ << std::endl;
                STREAM << *cit << std::endl;
                STREAM << *--cit << std::endl;
                STREAM << *cit-- << std::endl;
                STREAM << *cit << std::endl;
                STREAM << *( cit++ + 3 ) << std::endl;
                STREAM << *( 2 + cit ) << std::endl;
                STREAM << *( cit - 1 ) << std::endl;
                STREAM << cit - v.begin() << std::endl;
                STREAM << cit[3] << std::endl;
                STREAM << *( cit += 3 ) << std::endl;
                STREAM << *( cit -= 2 ) << std::endl;
                STREAM << cit->data() << std::endl;

                vector_type::reverse_iterator rit = v.rbegin();

                STREAM << *rit << std::endl;
                STREAM << *++rit << std::endl;
                STREAM << *rit++ << std::endl;
                STREAM << *rit << std::endl;
                STREAM << *--rit << std::endl;
                STREAM << *rit-- << std::endl;
                STREAM << *rit << std::endl;
                STREAM << *( rit++ + 3 ) << std::endl;
                STREAM << *( 2 + rit ) << std::endl;
                STREAM << *( rit - 1 ) << std::endl;
                STREAM << rit - v.rbegin() << std::endl;
                STREAM << rit[3] << std::endl;
                STREAM << *( rit += 3 ) << std::endl;
                STREAM << *( rit -= 2 ) << std::endl;
                STREAM << rit->data() << std::endl;

                vector_type::const_reverse_iterator crit = v.rbegin();

                STREAM << *crit << std::endl;
                STREAM << *++crit << std::endl;
                STREAM << *crit++ << std::endl;
                STREAM << *crit << std::endl;
                STREAM << *--crit << std::endl;
                STREAM << *crit-- << std::endl;
                STREAM << *crit << std::endl;
                STREAM << *( crit++ + 3 ) << std::endl;
                STREAM << *( 2 + crit ) << std::endl;
                STREAM << *( crit - 1 ) << std::endl;
                STREAM << crit - v.rbegin() << std::endl;
                STREAM << crit[3] << std::endl;
                STREAM << *( crit += 3 ) << std::endl;
                STREAM << *( crit -= 2 ) << std::endl;
                STREAM << crit->data() << std::endl;
            }
            {
                vector_type::iterator               it( v.begin() + 2 );
                vector_type::const_iterator         cit( it );
                vector_type::reverse_iterator       rit( v.begin() + 2 );
                vector_type::const_reverse_iterator crit( it );

                STREAM << ( it == it ) << std::endl;
                STREAM << ( it == cit ) << std::endl;
                STREAM << ( cit == it ) << std::endl;
                STREAM << ( it == it + 1 ) << std::endl;
                STREAM << ( it == cit + 1 ) << std::endl;
                STREAM << ( cit == it + 1 ) << std::endl;
                STREAM << ( rit == rit ) << std::endl;
                STREAM << ( rit == crit ) << std::endl;
                STREAM << ( crit == rit ) << std::endl;
                STREAM << ( rit == rit + 1 ) << std::endl;
                STREAM << ( rit == crit + 1 ) << std::endl;
                STREAM << ( crit == rit + 1 ) << std::endl;

                STREAM << ( it != it ) << std::endl;
                STREAM << ( it != cit ) << std::endl;
                STREAM << ( cit != it ) << std::endl;
                STREAM << ( it != it + 1 ) << std::endl;
                STREAM << ( it != cit + 1 ) << std::endl;
                STREAM << ( cit != it + 1 ) << std::endl;
                STREAM << ( rit != rit ) << std::endl;
                STREAM << ( rit != crit ) << std::endl;
                STREAM << ( crit != rit ) << std::endl;
                STREAM << ( rit != rit + 1 ) << std::endl;
                STREAM << ( rit != crit + 1 ) << std::endl;
                STREAM << ( crit != rit + 1 ) << std::endl;

                STREAM << ( it < it ) << std::endl;
                STREAM << ( it < cit ) << std::endl;
                STREAM << ( cit < it ) << std::endl;
                STREAM << ( it < it + 1 ) << std::endl;
                STREAM << ( it < cit + 1 ) << std::endl;
                STREAM << ( cit < it + 1 ) << std::endl;
                STREAM << ( it + 1 < it ) << std::endl;
                STREAM << ( it + 1 < cit ) << std::endl;
                STREAM << ( cit + 1 < it ) << std::endl;
                STREAM << ( rit < rit ) << std::endl;
                STREAM << ( rit < crit ) << std::endl;
                STREAM << ( crit < rit ) << std::endl;
                STREAM << ( rit < rit + 1 ) << std::endl;
                STREAM << ( rit < crit + 1 ) << std::endl;
                STREAM << ( crit < rit + 1 ) << std::endl;
                STREAM << ( rit + 1 < rit ) << std::endl;
                STREAM << ( rit + 1 < crit ) << std::endl;
                STREAM << ( crit + 1 < rit ) << std::endl;

                STREAM << ( it > it ) << std::endl;
                STREAM << ( it > cit ) << std::endl;
                STREAM << ( cit > it ) << std::endl;
                STREAM << ( it > it + 1 ) << std::endl;
                STREAM << ( it > cit + 1 ) << std::endl;
                STREAM << ( cit > it + 1 ) << std::endl;
                STREAM << ( it + 1 > it ) << std::endl;
                STREAM << ( it + 1 > cit ) << std::endl;
                STREAM << ( cit + 1 > it ) << std::endl;
                STREAM << ( rit > rit ) << std::endl;
                STREAM << ( rit > crit ) << std::endl;
                STREAM << ( crit > rit ) << std::endl;
                STREAM << ( rit > rit + 1 ) << std::endl;
                STREAM << ( rit > crit + 1 ) << std::endl;
                STREAM << ( crit > rit + 1 ) << std::endl;
                STREAM << ( rit + 1 > rit ) << std::endl;
                STREAM << ( rit + 1 > crit ) << std::endl;
                STREAM << ( crit + 1 > rit ) << std::endl;

                STREAM << ( it <= it ) << std::endl;
                STREAM << ( it <= cit ) << std::endl;
                STREAM << ( cit <= it ) << std::endl;
                STREAM << ( it <= it + 1 ) << std::endl;
                STREAM << ( it <= cit + 1 ) << std::endl;
                STREAM << ( cit <= it + 1 ) << std::endl;
                STREAM << ( it + 1 <= it ) << std::endl;
                STREAM << ( it + 1 <= cit ) << std::endl;
                STREAM << ( cit + 1 <= it ) << std::endl;
                STREAM << ( rit <= rit ) << std::endl;
                STREAM << ( rit <= crit ) << std::endl;
                STREAM << ( crit <= rit ) << std::endl;
                STREAM << ( rit <= rit + 1 ) << std::endl;
                STREAM << ( rit <= crit + 1 ) << std::endl;
                STREAM << ( crit <= rit + 1 ) << std::endl;
                STREAM << ( rit + 1 <= rit ) << std::endl;
                STREAM << ( rit + 1 <= crit ) << std::endl;
                STREAM << ( crit + 1 <= rit ) << std::endl;

                STREAM << ( it >= it ) << std::endl;
                STREAM << ( it >= cit ) << std::endl;
                STREAM << ( cit >= it ) << std::endl;
                STREAM << ( it >= it + 1 ) << std::endl;
                STREAM << ( it >= cit + 1 ) << std::endl;
                STREAM << ( cit >= it + 1 ) << std::endl;
                STREAM << ( it + 1 >= it ) << std::endl;
                STREAM << ( it + 1 >= cit ) << std::endl;
                STREAM << ( cit + 1 >= it ) << std::endl;
                STREAM << ( rit >= rit ) << std::endl;
                STREAM << ( rit >= crit ) << std::endl;
                STREAM << ( crit >= rit ) << std::endl;
                STREAM << ( rit >= rit + 1 ) << std::endl;
                STREAM << ( rit >= crit + 1 ) << std::endl;
                STREAM << ( crit >= rit + 1 ) << std::endl;
                STREAM << ( rit + 1 >= rit ) << std::endl;
                STREAM << ( rit + 1 >= crit ) << std::endl;
                STREAM << ( crit + 1 >= rit ) << std::endl;
            }
            {
                std::rotate( v.begin(),
                             v.end() - ( v.end() - v.begin() ) / 2,
                             v.end() );
                STREAM << v << std::endl;

                std::sort( v.rbegin(), v.rend() );
                STREAM << v << std::endl;

                std::fill( v.begin(), v.end(), f() );
                STREAM << v << std::endl;
            }
        }
        /* -------------------------------- Capacity -------------------------------- */

        /* ----------------------------- Element access ----------------------------- */

        /* -------------------------------- Modifiers ------------------------------- */

        /* -------------------------------------------------------------------------- */
    }
    /* ---------------------------------- Stack --------------------------------- */
    {

    }
    /* ----------------------------------- Map ---------------------------------- */
    {

    }
    /* ----------------------------------- Set ---------------------------------- */
    {}
    /* -------------------------------------------------------------------------- */
}
