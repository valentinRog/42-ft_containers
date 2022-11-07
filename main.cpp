#include "src/map.hpp"
#include "src/set.hpp"
#include "src/stack.hpp"
#include "src/vector.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#ifndef NS
#define NS std
#endif

#define STREAM std::cout << "#" << __LINE__ << "  "

/* ---------------------------------- Data ---------------------------------- */

template < typename T > class A {
    T *_data;

public:
    typedef T value_type;

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

template < typename T > struct F {
    typename T::value_type operator()() {
        _n++;
        std::stringstream ss;
        ss << _n;
        typename T::value_type res;
        ss >> res;
        return res;
    }

private:
    int _n;
};

/* ------------------------------- Algorithms ------------------------------- */

namespace ft {

template < typename T > T next( T it ) { return ++it; }

}
/* ---------------------------- Custom allocator ---------------------------- */

template < typename T > struct Vallocator;

template <> struct Vallocator< void > {
    typedef void              value_type;
    typedef value_type *      pointer;
    typedef const value_type *const_pointer;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    template < class U > struct rebind { typedef Vallocator< U > other; };
};

template < typename T > struct Vallocator {
    typedef T                 value_type;
    typedef value_type &      reference;
    typedef const value_type &const_reference;
    typedef value_type *      pointer;
    typedef const value_type *const_pointer;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    template < typename U > struct rebind { typedef Vallocator< U > other; };

    Vallocator() throw() {}
    template < typename U > Vallocator( Vallocator< U > const & ) throw() {}

    pointer allocate( const size_type &n,
                      Vallocator< void >::const_pointer = 0 ) {
        if ( n ) { _n_allocation++; }
        return static_cast< pointer >(
            ::operator new( n * sizeof( value_type ) ) );
    }
    void deallocate( pointer p, const size_type &n ) {
        if ( n ) { _n_allocation--; }
        ::operator delete( p );
    }
    void construct( pointer p, value_type const &val ) {
        ::new ( p ) value_type( val );
    }
    void destroy( pointer p ) { p->~value_type(); }

    size_type max_size() const throw() {
        return std::numeric_limits< size_type >::max() / sizeof( value_type );
    }
    pointer       address( reference x ) const { return &x; }
    const_pointer address( const_reference x ) const { return &x; }

    static size_type get_n_allocation() { return _n_allocation; }

private:
    static size_type _n_allocation;
};

template < typename T >
typename Vallocator< T >::size_type Vallocator< T >::_n_allocation;

template < typename T, typename U >
bool operator==( Vallocator< T > const &, Vallocator< U > const & ) {
    return true;
}
template < typename T, typename U >
bool operator!=( Vallocator< T > const &x, Vallocator< U > const &y ) {
    return !( x == y );
}

/* ------------------------ ostream operator overload ----------------------- */

template < typename T, typename A >
std::ostream &operator<<( std::ostream &os, const NS::vector< T, A > &v ) {
    os << "{size: " << v.size() << ", capacity: " << v.capacity() << ", data: "
       << "[";
    for ( typename NS::vector< T, A >::const_iterator it = v.begin();
          it != v.end();
          it++ ) {
        if ( it != v.begin() ) { os << ", "; }
        os << *it;
    }
    os << "]}";
    return os;
}

template < typename T1, typename T2 >
std::ostream &operator<<( std::ostream &os, const NS::pair< T1, T2 > &p ) {
    os << p.first << ": " << p.second;
    return os;
}

template < typename K, typename V, typename C, typename A >
std::ostream &operator<<( std::ostream &os, const NS::map< K, V, C, A > &m ) {
    os << "{";
    for ( typename NS::map< K, V, C, A >::const_iterator it = m.begin();
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

typedef A< std::string > key_type;
typedef A< int >         mapped_type;

/* -------------------------------------------------------------------------- */

int main() {

    F< mapped_type > f;
    F< key_type >    g;

    /* --------------------------------- Vector --------------------------------- */
    {
        typedef NS::vector< mapped_type, Vallocator< mapped_type > >
                                  vector_type;
        typedef const vector_type const_vector_type;

        /* ------------------------------ Construction ------------------------------ */
        {
            const int arr[] = { f(), f(), f(), f(), f() };

            vector_type       v1;
            vector_type       v2( 7, f() );
            vector_type       v3( arr, arr + sizeof( arr ) / sizeof( int ) );
            vector_type       v4( v3 );
            const_vector_type cv( v2 );

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;
            STREAM << v3 << std::endl;
            STREAM << v4 << std::endl;
            STREAM << cv << std::endl;

            v4 = v2;
            v2 = vector_type( 3 );

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
                vector_type::reverse_iterator rit4( v.end() );

                rit1 = rit2;

                STREAM << *rit1 << std::endl;
                STREAM << *rit2 << std::endl;
                STREAM << *rit3 << std::endl;
                STREAM << *rit4 << std::endl;

                vector_type::const_reverse_iterator crit1;
                vector_type::const_reverse_iterator crit2 = v.rbegin();
                vector_type::const_reverse_iterator crit3( crit2 );
                vector_type::const_reverse_iterator crit4( rit1 );
                vector_type::const_reverse_iterator crit5;
                vector_type::reverse_iterator       crit6( v.end() );

                crit1 = crit2;
                crit5 = rit1;

                STREAM << *crit1 << std::endl;
                STREAM << *crit2 << std::endl;
                STREAM << *crit3 << std::endl;
                STREAM << *crit4 << std::endl;
                STREAM << *crit5 << std::endl;
                STREAM << *crit6 << std::endl;
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

                const_vector_type cv( v );

                STREAM << *cv.begin() << std::endl;
                STREAM << *--cv.end() << std::endl;
                STREAM << *cv.rbegin() << std::endl;
                STREAM << *--cv.rend() << std::endl;
            }
            {
                vector_type::iterator               it( v.begin() + 2 );
                vector_type::const_iterator         cit( it );
                vector_type::reverse_iterator       rit( v.rbegin() + 2 );
                vector_type::const_reverse_iterator crit( rit );

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
                STREAM << std::distance( v.begin(), v.end() ) << std::endl;

                const_vector_type cv( v );

                STREAM << std::count( cv.begin(), cv.end(), v[0] ) << std::endl;
            }
        }
        /* -------------------------------- Capacity -------------------------------- */
        {
            vector_type v;

            STREAM << v.empty() << std::endl;

            v.reserve( 10 );

            STREAM << v.empty() << std::endl;

            std::generate( v.begin(), v.end(), f );

            STREAM << v.empty() << std::endl;

            v.resize( 11 );

            STREAM << v << std::endl;

            v.resize( 6 );

            STREAM << v << std::endl;

            v.reserve( 40 );

            STREAM << v << std::endl;

            v.resize( 40 );

            STREAM << v << std::endl;

            v.reserve( 20 );

            STREAM << v << std::endl;

            try {
                v.reserve( v.max_size() + 1 );
            } catch ( const std::length_error & ) {
                STREAM << "caught \"length_error\" exception" << std::endl;
            }
        }
        /* ----------------------------- Element access ----------------------------- */
        {
            vector_type v( 10 );
            std::generate( v.begin(), v.end(), f );

            for ( vector_type::size_type i( 0 ); i < v.size(); i++ ) {
                STREAM << v[i] << std::endl;
                STREAM << v.at( i ) << std::endl;
                STREAM << v.data()[i] << std::endl;
            }

            try {
                v.at( v.size() );
            } catch ( const std::out_of_range &e ) {
                STREAM << "caught \"out_of_range\" exception" << std::endl;
            }
            try {
                v.at( -1 );
            } catch ( const std::out_of_range &e ) {
                STREAM << "caught \"out_of_range\" exception" << std::endl;
            }

            STREAM << v.front() << std::endl;
            STREAM << v.back() << std::endl;

            const_vector_type cv( v );

            STREAM << cv[3] << std::endl;
            STREAM << cv.at( 3 ) << std::endl;
            STREAM << cv.front() << std::endl;
            STREAM << cv.back() << std::endl;
        }
        /* -------------------------------- Modifiers ------------------------------- */
        {
            vector_type v1;
            vector_type v2;

            v1.assign( 5, f() );
            v2.assign( v1.begin(), v1.end() - 1 );

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;

            v1.assign( 4, f() );
            v2.assign( v1.rbegin(), v1.rend() - 2 );

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;

            v1.push_back( f() );

            STREAM << v1 << std::endl;

            v1.push_back( f() );
            v1.push_back( f() );

            STREAM << v1 << std::endl;

            for ( vector_type::size_type i( 0 ); v1.size(); i++ ) {
                v1.pop_back();

                STREAM << v1 << std::endl;
            }

            v2 = vector_type( 3 );
            std::generate( v2.begin(), v2.end(), f );
            v1.insert( v1.begin(), 10, f() );

            STREAM << v1 << std::endl;

            v1.insert( v1.begin(), f() );
            v1.insert( v1.begin() + 3, f() );
            v1.insert( v1.end(), f() );

            STREAM << v1 << std::endl;

            v1.insert( v1.begin(), 2, f() );
            v1.insert( v1.begin() + 3, 2, f() );
            v1.insert( v1.end(), 2, f() );

            STREAM << v1 << std::endl;

            v1.insert( v1.begin(), v2.begin(), v2.end() );
            v1.insert( v1.begin() + 2, v2.begin(), v2.end() );
            v1.insert( v1.end(), v2.begin(), v2.end() );

            STREAM << v1 << std::endl;

            v1.erase( v1.begin() );
            v1.erase( v1.begin() + 5 );
            v1.erase( v1.end() );

            STREAM << v1 << std::endl;

            v1.erase( v1.begin(), v1.begin() + 4 );
            v1.erase( v1.begin() + 3, v1.begin() + 4 );
            v1.erase( v1.end() - 2, v1.end() );

            STREAM << v1 << std::endl;

            v1.swap( v2 );

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;

            vector_type( v1 ).swap( v1 );
            vector_type( v2 ).swap( v2 );

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;

            v1.clear();
            v2.clear();

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;
        }
        /* -------------------------------- Allocator ------------------------------- */
        {
            vector_type               v1;
            vector_type               v2( ( vector_type::allocator_type() ) );
            NS::vector< mapped_type > v3;

            STREAM << ( v1.get_allocator() == Vallocator< mapped_type >() )
                   << std::endl;
            STREAM << ( v1.get_allocator() == v2.get_allocator() ) << std::endl;
            STREAM << ( v3.get_allocator() == std::allocator< mapped_type >() )
                   << std::endl;
        }
        /* -------------------------- Relational operators -------------------------- */
        {
            vector_type v( 10 );
            std::generate( v.begin(), v.end(), f );
            const_vector_type v1( v );
            vector_type       v2( v1 );
            vector_type       v3( v1 );
            vector_type       v4( v1 );
            vector_type       v5( v1 );

            v2.back().data()--;
            v3.back().data()++;
            v4.pop_back();
            v5.push_back( f() );

            STREAM << ( v == v1 ) << std::endl;
            STREAM << ( v == v2 ) << std::endl;
            STREAM << ( v == v3 ) << std::endl;
            STREAM << ( v == v4 ) << std::endl;
            STREAM << ( v == v5 ) << std::endl;

            STREAM << ( v != v1 ) << std::endl;
            STREAM << ( v != v2 ) << std::endl;
            STREAM << ( v != v3 ) << std::endl;
            STREAM << ( v != v4 ) << std::endl;
            STREAM << ( v != v5 ) << std::endl;

            STREAM << ( v < v1 ) << std::endl;
            STREAM << ( v < v2 ) << std::endl;
            STREAM << ( v < v3 ) << std::endl;
            STREAM << ( v < v4 ) << std::endl;
            STREAM << ( v < v5 ) << std::endl;

            STREAM << ( v > v1 ) << std::endl;
            STREAM << ( v > v2 ) << std::endl;
            STREAM << ( v > v3 ) << std::endl;
            STREAM << ( v > v4 ) << std::endl;
            STREAM << ( v > v5 ) << std::endl;

            STREAM << ( v <= v1 ) << std::endl;
            STREAM << ( v <= v2 ) << std::endl;
            STREAM << ( v <= v3 ) << std::endl;
            STREAM << ( v <= v4 ) << std::endl;
            STREAM << ( v <= v5 ) << std::endl;

            STREAM << ( v >= v1 ) << std::endl;
            STREAM << ( v >= v2 ) << std::endl;
            STREAM << ( v >= v3 ) << std::endl;
            STREAM << ( v >= v4 ) << std::endl;
            STREAM << ( v >= v5 ) << std::endl;
        }
        /* ---------------------------------- Swap ---------------------------------- */
        {
            vector_type v1( 10 );
            std::generate( v1.begin(), v1.end(), f );
            vector_type v2( 4, f() );

            NS::swap( v1, v2 );

            STREAM << v1 << std::endl;
            STREAM << v2 << std::endl;
        }
        /* -------------------------------------------------------------------------- */
    }

    /* ---------------------------------- Stack --------------------------------- */
    {

    }
    /* ----------------------------------- Map ---------------------------------- */
    {
        typedef NS::map< key_type,
                         mapped_type,
                         std::less< key_type >,
                         Vallocator< NS::pair< const key_type, mapped_type > > >
                               map_type;
        typedef const map_type const_map_type;

        /* ------------------------------ Construction ------------------------------ */
        {
            const map_type::value_type arr[]
                = { map_type::value_type( g(), f() ),
                    map_type::value_type( g(), f() ),
                    map_type::value_type( g(), f() ),
                    map_type::value_type( g(), f() ) };

            map_type       m1;
            map_type m2( arr,
                         arr + sizeof( arr ) / sizeof( map_type::value_type ) );
            map_type       m3( m2 );
            const_map_type cm( m3 );

            STREAM << m1 << std::endl;
            STREAM << m2 << std::endl;
            STREAM << m3 << std::endl;
            STREAM << cm << std::endl;

            m3 = m1;
            m1 = m2;

            STREAM << m3 << std::endl;
            STREAM << m1 << std::endl;
        }
        /* -------------------------------- Iterators ------------------------------- */
        {
            const map_type::value_type arr[]
                = { map_type::value_type( g(), f() ),
                    map_type::value_type( g(), f() ),
                    map_type::value_type( g(), f() ),
                    map_type::value_type( g(), f() ) };
            map_type m( arr,
                        arr + sizeof( arr ) / sizeof( map_type::value_type ) );

            {
                map_type::iterator it1;
                map_type::iterator it2 = m.begin();
                map_type::iterator it3( it2 );

                it1 = it2;

                STREAM << *it1 << std::endl;
                STREAM << *it2 << std::endl;
                STREAM << *it3 << std::endl;

                map_type::const_iterator cit1;
                map_type::const_iterator cit2 = m.begin();
                map_type::const_iterator cit3( cit2 );
                map_type::const_iterator cit4( it1 );
                map_type::const_iterator cit5;

                cit1 = cit2;
                cit5 = it1;

                STREAM << *cit1 << std::endl;
                STREAM << *cit2 << std::endl;
                STREAM << *cit3 << std::endl;
                STREAM << *cit4 << std::endl;
                STREAM << *cit5 << std::endl;

                map_type::reverse_iterator rit1;
                map_type::reverse_iterator rit2 = m.rbegin();
                map_type::reverse_iterator rit3( rit2 );
                map_type::reverse_iterator rit4( m.end() );

                rit1 = rit2;

                STREAM << *rit1 << std::endl;
                STREAM << *rit2 << std::endl;
                STREAM << *rit3 << std::endl;
                STREAM << *rit4 << std::endl;

                map_type::const_reverse_iterator crit1;
                map_type::const_reverse_iterator crit2 = m.rbegin();
                map_type::const_reverse_iterator crit3( crit2 );
                map_type::const_reverse_iterator crit4( rit1 );
                map_type::const_reverse_iterator crit5;
                map_type::reverse_iterator       crit6( m.end() );

                crit1 = crit2;
                crit5 = rit1;

                STREAM << *crit1 << std::endl;
                STREAM << *crit2 << std::endl;
                STREAM << *crit3 << std::endl;
                STREAM << *crit4 << std::endl;
                STREAM << *crit5 << std::endl;
                STREAM << *crit6 << std::endl;
            }
            {
                map_type::iterator it = m.begin();

                STREAM << *it << std::endl;
                STREAM << *++it << std::endl;
                STREAM << *it++ << std::endl;
                STREAM << *it << std::endl;
                STREAM << *--it << std::endl;
                STREAM << *it-- << std::endl;
                STREAM << *it << std::endl;
                STREAM << it->first << std::endl;

                map_type::const_iterator cit = m.begin();

                STREAM << *cit << std::endl;
                STREAM << *++cit << std::endl;
                STREAM << *cit++ << std::endl;
                STREAM << *cit << std::endl;
                STREAM << *--cit << std::endl;
                STREAM << *cit-- << std::endl;
                STREAM << *cit << std::endl;
                STREAM << cit->first << std::endl;

                map_type::reverse_iterator rit = m.rbegin();

                STREAM << *rit << std::endl;
                STREAM << *++rit << std::endl;
                STREAM << *rit++ << std::endl;
                STREAM << *rit << std::endl;
                STREAM << *--rit << std::endl;
                STREAM << *rit-- << std::endl;
                STREAM << *rit << std::endl;
                STREAM << rit->first << std::endl;

                map_type::const_reverse_iterator crit = m.rbegin();

                STREAM << *crit << std::endl;
                STREAM << *++crit << std::endl;
                STREAM << *crit++ << std::endl;
                STREAM << *crit << std::endl;
                STREAM << *--crit << std::endl;
                STREAM << *crit-- << std::endl;
                STREAM << *crit << std::endl;
                STREAM << crit->first << std::endl;

                const_map_type cm( m );

                STREAM << *cm.begin() << std::endl;
                STREAM << *--cm.end() << std::endl;
                STREAM << *cm.rbegin() << std::endl;
                STREAM << *--cm.rend() << std::endl;
            }
            {
                map_type::iterator               it( ft::next( m.begin() ) );
                map_type::const_iterator         cit( it );
                map_type::reverse_iterator       rit( ft::next( m.rbegin() ) );
                map_type::const_reverse_iterator crit( rit );

                STREAM << ( it == it ) << std::endl;
                STREAM << ( it == cit ) << std::endl;
                STREAM << ( cit == it ) << std::endl;
                STREAM << ( it == ft::next( it ) ) << std::endl;
                STREAM << ( it == ft::next( cit ) ) << std::endl;
                STREAM << ( cit == ft::next( it ) ) << std::endl;
                STREAM << ( rit == rit ) << std::endl;
                STREAM << ( rit == crit ) << std::endl;
                STREAM << ( crit == rit ) << std::endl;
                STREAM << ( rit == ft::next( rit ) ) << std::endl;
                STREAM << ( rit == ft::next( crit ) ) << std::endl;
                STREAM << ( crit == ft::next( rit ) ) << std::endl;

                STREAM << ( it != it ) << std::endl;
                STREAM << ( it != cit ) << std::endl;
                STREAM << ( cit != it ) << std::endl;
                STREAM << ( it != ft::next( it ) ) << std::endl;
                STREAM << ( it != ft::next( cit ) ) << std::endl;
                STREAM << ( cit != ft::next( it ) ) << std::endl;
                STREAM << ( rit != rit ) << std::endl;
                STREAM << ( rit != crit ) << std::endl;
                STREAM << ( crit != rit ) << std::endl;
                STREAM << ( rit != ft::next( rit ) ) << std::endl;
                STREAM << ( rit != ft::next( crit ) ) << std::endl;
                STREAM << ( crit != ft::next( rit ) ) << std::endl;
            }
        }
        /* -------------------------------- Capacity -------------------------------- */
        {

        }
        /* ----------------------------- Element access ----------------------------- */
        {

        }
        /* -------------------------------- Modifiers ------------------------------- */
        {

        }
        /* -------------------------------- Observers ------------------------------- */
        {

        }
        /* ------------------------------- Operations ------------------------------- */
        {

        }
        /* -------------------------------- Allocator ------------------------------- */
        {}
        /* -------------------------------------------------------------------------- */
    }
    /* ----------------------------------- Set ---------------------------------- */
    {
        /* ------------------------------ Construction ------------------------------ */
        {

        }
        /* -------------------------------- Iterators ------------------------------- */
        {

        }
        /* -------------------------------- Capacity -------------------------------- */
        {

        }
        /* -------------------------------- Modifiers ------------------------------- */
        {

        }
        /* -------------------------------- Observers ------------------------------- */
        {

        }
        /* ------------------------------- Operations ------------------------------- */
        {

        }
        /* -------------------------------- Allocator ------------------------------- */
        {}
        /* -------------------------------------------------------------------------- */
    }

    /* -------------------------------------------------------------------------- */
}
