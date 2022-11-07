#ifndef NS
#define NS std
#endif

#ifdef NS

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
#include <stack>
#include <typeinfo>
#include <vector>

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
    F() : _n( 0 ) {}
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
        _n_construction++;
        ::new ( p ) value_type( val );
    }
    void destroy( pointer p ) {
        _n_construction--;
        p->~value_type();
    }

    size_type max_size() const throw() {
        return std::numeric_limits< size_type >::max() / sizeof( value_type );
    }
    pointer       address( reference x ) const { return &x; }
    const_pointer address( const_reference x ) const { return &x; }

    static size_type get_n_allocation() { return _n_allocation; }
    static size_type get_n_construction() { return _n_construction; }

private:
    static size_type _n_allocation;
    static size_type _n_construction;
};

template < typename T >
typename Vallocator< T >::size_type Vallocator< T >::_n_allocation;
template < typename T >
typename Vallocator< T >::size_type Vallocator< T >::_n_construction;

template < typename T, typename U >
bool operator==( Vallocator< T > const &, Vallocator< U > const & ) {
    return true;
}
template < typename T, typename U >
bool operator!=( Vallocator< T > const &x, Vallocator< U > const &y ) {
    return !( x == y );
}

/* ------------------------------ Mutant stack ------------------------------ */

template < typename T, typename C > struct S : public NS::stack< T, C > {
    typedef typename NS::stack< T, C >::container_type::const_iterator
        const_iterator;
    typedef typename NS::stack< T, C >::container_type::const_reverse_iterator
        const_reverse_iterator;

    S< T, C >() {}
    S< T, C >( const S< T, C > &other ) : NS::stack< T, C >( other ) {}
    S< T, C >( const NS::stack< T, C > &other ) : NS::stack< T, C >( other ) {}
    S< T, C > &operator=( const S< T, C > &other ) {
        NS::stack< T, C >::operator=( other );
    }

    const_iterator         begin() const { return this->c.begin(); }
    const_reverse_iterator rbegin() const { return this->c.rbegin(); }
    const_iterator         end() const { return this->c.end(); }
    const_reverse_iterator rend() const { return this->c.rend(); }
};

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

template < typename T, typename C >
std::ostream &operator<<( std::ostream &os, const NS::stack< T, C > &s ) {
    S< T, C > ms( s );
    os << "[";
    for ( typename S< T, C >::const_iterator cit = ms.begin(); cit != ms.end();
          cit++ ) {
        if ( cit != ms.begin() ) { os << ", "; }
        os << *cit;
    }
    os << "]";
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

template < typename T, typename C, typename A >
std::ostream &operator<<( std::ostream &os, const NS::set< T, C, A > s ) {
    os << "{";
    for ( typename NS::set< T, C, A >::const_iterator it = s.begin();
          it != s.end();
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

typedef A< std::string >                        key_type;
typedef A< int >                                mapped_type;
typedef NS::pair< const key_type, mapped_type > value_type;

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
            const mapped_type arr[] = { f(), f(), f(), f(), f() };

            vector_type v1;
            vector_type v2( 7, f() );
            vector_type v3( arr, arr + sizeof( arr ) / sizeof( mapped_type ) );
            vector_type v4( v3 );
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

                *it   = f();
                it[1] = f();

                STREAM << v << std::endl;

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

                *rit   = f();
                rit[1] = f();

                STREAM << v << std::endl;

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

            v[1]        = f();
            v.at( 2 )   = f();
            v.data()[3] = f();

            STREAM << v << std::endl;

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
            const mapped_type arr[] = { f(), f(), f(), f(), f() };
            vector_type       v;

            v.assign( 5, f() );

            STREAM << v << std::endl;

            v.assign( arr, arr + 3 );

            STREAM << v << std::endl;

            v.push_back( f() );

            STREAM << v << std::endl;

            v.push_back( f() );
            v.push_back( f() );

            STREAM << v << std::endl;

            for ( vector_type::size_type i( 0 ); v.size(); i++ ) {
                v.pop_back();

                STREAM << v << std::endl;
            }

            v = vector_type( 3 );
            v.insert( v.begin(), 10, f() );

            STREAM << v << std::endl;

            v.insert( v.begin(), f() );
            v.insert( v.begin() + 3, f() );
            v.insert( v.end(), f() );

            STREAM << v << std::endl;

            v.insert( v.begin(), 2, f() );
            v.insert( v.begin() + 3, 2, f() );
            v.insert( v.end(), 2, f() );

            STREAM << v << std::endl;

            v.insert( v.begin(),
                      arr,
                      arr + sizeof( arr ) / sizeof( mapped_type ) );
            v.insert( v.begin() + 2,
                      arr,
                      arr + sizeof( arr ) / sizeof( mapped_type ) );
            v.insert( v.end(),
                      arr,
                      arr + sizeof( arr ) / sizeof( mapped_type ) );

            STREAM << v << std::endl;

            v.erase( v.begin() );
            v.erase( v.begin() + 5 );
            v.erase( v.end() );

            STREAM << v << std::endl;

            v.erase( v.begin(), v.begin() + 4 );
            v.erase( v.begin() + 3, v.begin() + 4 );
            v.erase( v.end() - 2, v.end() );

            STREAM << v << std::endl;

            vector_type v2( 3 );
            std::generate( v2.begin(), v2.end(), f );

            v.swap( v2 );

            STREAM << v << std::endl;
            STREAM << v2 << std::endl;

            vector_type( v ).swap( v );
            vector_type( v2 ).swap( v2 );

            STREAM << v << std::endl;
            STREAM << v2 << std::endl;

            v.clear();
            v2.clear();

            STREAM << v << std::endl;
            STREAM << v2 << std::endl;
        }
        /* -------------------------- Relational operators -------------------------- */
        {
            const mapped_type arr[] = { f(), f(), f(), f(), f() };
            const_vector_type v( arr,
                                 arr + sizeof( arr ) / sizeof( mapped_type ) );
            vector_type       v1( v );
            vector_type       v2( v );
            vector_type       v3( v );
            vector_type       v4( v );

            v1.back().data()--;
            v2.back().data()++;
            v3.pop_back();
            v4.push_back( f() );

            STREAM << ( v == v1 ) << std::endl;
            STREAM << ( v == v2 ) << std::endl;
            STREAM << ( v == v3 ) << std::endl;
            STREAM << ( v == v4 ) << std::endl;

            STREAM << ( v != v1 ) << std::endl;
            STREAM << ( v != v2 ) << std::endl;
            STREAM << ( v != v3 ) << std::endl;
            STREAM << ( v != v4 ) << std::endl;

            STREAM << ( v < v1 ) << std::endl;
            STREAM << ( v < v2 ) << std::endl;
            STREAM << ( v < v3 ) << std::endl;
            STREAM << ( v < v4 ) << std::endl;

            STREAM << ( v > v1 ) << std::endl;
            STREAM << ( v > v2 ) << std::endl;
            STREAM << ( v > v3 ) << std::endl;
            STREAM << ( v > v4 ) << std::endl;

            STREAM << ( v <= v1 ) << std::endl;
            STREAM << ( v <= v2 ) << std::endl;
            STREAM << ( v <= v3 ) << std::endl;
            STREAM << ( v <= v4 ) << std::endl;

            STREAM << ( v >= v1 ) << std::endl;
            STREAM << ( v >= v2 ) << std::endl;
            STREAM << ( v >= v3 ) << std::endl;
            STREAM << ( v >= v4 ) << std::endl;
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
        /* -------------------------------- Allocator ------------------------------- */
        {
            STREAM << vector_type().get_allocator().get_n_allocation()
                   << std::endl;
            STREAM << vector_type().get_allocator().get_n_construction()
                   << std::endl;
        }
        /* -------------------------------------------------------------------------- */
    }

    /* ---------------------------------- Stack --------------------------------- */
    {
        typedef NS::stack<
            mapped_type,
            NS::vector< mapped_type, Vallocator< mapped_type > > >
            stack_type;

        stack_type s;

        STREAM << s << std::endl;
        STREAM << s.empty() << std::endl;

        s.push( f() );

        STREAM << s.top() << std::endl;
        STREAM << s << std::endl;

        s.pop();

        STREAM << s << std::endl;

        s.push( f() );
        s.push( f() );
        s.push( f() );
        s.top() = f();

        STREAM << s << std::endl;

        s.pop();
        s.pop();

        STREAM << s << std::endl;

        stack_type s2;
        s2.push( f() );

        STREAM << ( s == s ) << std::endl;
        STREAM << ( s == s2 ) << std::endl;
        STREAM << ( s2 == s ) << std::endl;
        STREAM << ( s != s ) << std::endl;
        STREAM << ( s != s2 ) << std::endl;
        STREAM << ( s2 != s ) << std::endl;
        STREAM << ( s < s ) << std::endl;
        STREAM << ( s < s2 ) << std::endl;
        STREAM << ( s2 < s ) << std::endl;
        STREAM << ( s > s ) << std::endl;
        STREAM << ( s > s2 ) << std::endl;
        STREAM << ( s2 > s ) << std::endl;
        STREAM << ( s <= s ) << std::endl;
        STREAM << ( s <= s2 ) << std::endl;
        STREAM << ( s2 <= s ) << std::endl;
        STREAM << ( s >= s ) << std::endl;
        STREAM << ( s >= s2 ) << std::endl;
        STREAM << ( s2 >= s ) << std::endl;
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
            const value_type arr[] = { value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ) };

            std::cout << arr[0] << std::endl;
            std::cout << arr[1] << std::endl;
            std::cout << arr[2] << std::endl;
            std::cout << arr[3] << std::endl;

            map_type m1;
            map_type m2( arr, arr + sizeof( arr ) / sizeof( value_type ) );
            map_type m3( m2 );
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

                ( *it ).second         = f();
                ft::next( it )->second = f();

                STREAM << m << std::endl;

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

                ( *rit ).second         = f();
                ft::next( rit )->second = f();

                STREAM << m << std::endl;

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
            map_type m;

            STREAM << m.empty() << std::endl;

            m[g()] = f();

            STREAM << m.empty() << std::endl;

            m.clear();

            STREAM << m.empty() << std::endl;
        }
        /* ----------------------------- Element access ----------------------------- */
        {
            const value_type arr[] = { value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ) };
            map_type m( arr, arr + sizeof( arr ) / sizeof( value_type ) );

            for ( map_type::size_type i( 0 ); i < m.size(); i++ ) {
                STREAM << m[arr[i].first] << std::endl;
                STREAM << m.at( arr[i].first ) << std::endl;
            }

            m[arr[1].first]      = f();
            m.at( arr[2].first ) = f();
            m[g()]               = f();

            STREAM << m << std::endl;

            try {
                m.at( g() );
            } catch ( const std::out_of_range &e ) {
                STREAM << "caught \"out_of_range\" exception" << std::endl;
            }

            const_map_type cm( m );

            STREAM << cm.at( arr[3].first ) << std::endl;
        }
        /* -------------------------------- Modifiers ------------------------------- */
        {
            const value_type arr[] = { value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ) };
            map_type m( arr, arr + sizeof( arr ) / sizeof( value_type ) );
            NS::pair< map_type::iterator, bool > res;

            res = m.insert( value_type( g(), f() ) );

            STREAM << res.second << std::endl;
            STREAM << *res.first << std::endl;

            res = m.insert( arr[2] );

            STREAM << res.second << std::endl;
            STREAM << *res.first << std::endl;
            STREAM << m << std::endl;
            STREAM << *m.insert( m.begin(), value_type( g(), f() ) )
                   << std::endl;
            STREAM << *m.insert( m.end(), value_type( g(), f() ) ) << std::endl;
            STREAM << *m.insert( m.begin(), arr[1] ) << std::endl;
            STREAM << *m.insert( m.end(), arr[1] ) << std::endl;
            STREAM << m << std::endl;

            m.erase( m.begin() );
            m.erase( ft::next( m.begin() ) );

            STREAM << m << std::endl;
            STREAM << m.erase( arr[4].first ) << std::endl;
            STREAM << m.erase( arr[4].first ) << std::endl;
            STREAM << m << std::endl;

            m.erase( ft::next( ft::next( m.begin() ) ) );

            STREAM << m << std::endl;

            m.clear();

            STREAM << m << std::endl;

            m.clear();
            m.insert( arr, arr + sizeof( arr ) / sizeof( value_type ) );

            STREAM << m << std::endl;

            map_type m2;
            m2.insert( value_type( g(), f() ) );
            m2.insert( value_type( g(), f() ) );

            m.swap( m2 );

            STREAM << m << std::endl;
            STREAM << m2 << std::endl;

            m.swap( m );
            m2.swap( m2 );

            STREAM << m << std::endl;
            STREAM << m2 << std::endl;
        }
        /* -------------------------------- Observers ------------------------------- */
        {
            key_type x = g();
            key_type y = g();

            STREAM << const_map_type().key_comp()( x, y ) << std::endl;
            STREAM << const_map_type().key_comp()( y, x ) << std::endl;

            value_type a( g(), f() );
            value_type b( g(), a.second );

            STREAM << const_map_type().value_comp()( a, b ) << std::endl;
            STREAM << const_map_type().value_comp()( b, a ) << std::endl;
        }
        /* ------------------------------- Operations ------------------------------- */
        {
            const value_type arr[] = { value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ) };

            map_type m( arr, arr + sizeof( arr ) / sizeof( value_type ) );

            STREAM << ( m.find( g() ) == m.end() ) << std::endl;
            STREAM << *m.find( arr[2].first ) << std::endl;

            STREAM << m.count( g() ) << std::endl;
            STREAM << m.count( arr[2].first ) << std::endl;

            STREAM << *m.lower_bound( arr->first ) << std::endl;
            STREAM << *m.lower_bound( arr[1].first ) << std::endl;
            STREAM << ( m.lower_bound( g() ) == m.end() ) << std::endl;
            STREAM
                << ( m.lower_bound(
                         arr[sizeof( arr ) / sizeof( value_type ) - 1].first )
                     == m.end() )
                << std::endl;

            STREAM << *m.upper_bound( arr->first ) << std::endl;
            STREAM << *m.upper_bound( arr[1].first ) << std::endl;
            STREAM << ( m.upper_bound( g() ) == m.end() ) << std::endl;
            STREAM
                << ( m.upper_bound(
                         arr[sizeof( arr ) / sizeof( value_type ) - 1].first )
                     == m.end() )
                << std::endl;

            NS::pair< map_type::iterator, map_type::iterator > res;
            res = m.equal_range( arr[2].first );

            STREAM << *res.first << std::endl;
            STREAM << std::distance( res.first, res.second ) << std::endl;

            res = m.equal_range( g() );

            STREAM << std::distance( res.first, res.second ) << std::endl;

            const_map_type cm( m );

            STREAM << *cm.find( arr->first ) << std::endl;
            STREAM << cm.count( arr->first ) << std::endl;
            STREAM << *cm.lower_bound( arr->first ) << std::endl;
            STREAM << *cm.upper_bound( arr->first ) << std::endl;
            STREAM << *cm.equal_range( arr[2].first ).first << std::endl;
        }
        /* -------------------------- Relational operators -------------------------- */
        {
            value_type       x     = value_type( g(), f() );
            value_type       y     = value_type( g(), f() );
            value_type       z     = value_type( g(), f() );
            const value_type arr[] = { value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       value_type( g(), f() ),
                                       y };
            const_map_type m( arr, arr + sizeof( arr ) / sizeof( value_type ) );
            map_type       m1( m );
            map_type       m2( m );
            map_type       m3( m );
            map_type       m4( m );

            m1.erase( y.first );
            m1.insert( x );
            m1.erase( y.first );
            m2.insert( z );
            m3.erase( y.first );
            m4[g()] = f();

            STREAM << ( m == m1 ) << std::endl;
            STREAM << ( m == m2 ) << std::endl;
            STREAM << ( m == m3 ) << std::endl;
            STREAM << ( m == m4 ) << std::endl;

            STREAM << ( m != m1 ) << std::endl;
            STREAM << ( m != m2 ) << std::endl;
            STREAM << ( m != m3 ) << std::endl;
            STREAM << ( m != m4 ) << std::endl;

            STREAM << ( m < m1 ) << std::endl;
            STREAM << ( m < m2 ) << std::endl;
            STREAM << ( m < m3 ) << std::endl;
            STREAM << ( m < m4 ) << std::endl;

            STREAM << ( m > m1 ) << std::endl;
            STREAM << ( m > m2 ) << std::endl;
            STREAM << ( m > m3 ) << std::endl;
            STREAM << ( m > m4 ) << std::endl;

            STREAM << ( m <= m1 ) << std::endl;
            STREAM << ( m <= m2 ) << std::endl;
            STREAM << ( m <= m3 ) << std::endl;
            STREAM << ( m <= m4 ) << std::endl;

            STREAM << ( m >= m1 ) << std::endl;
            STREAM << ( m >= m2 ) << std::endl;
            STREAM << ( m >= m3 ) << std::endl;
            STREAM << ( m >= m4 ) << std::endl;
        }
        /* -------------------------------- Allocator ------------------------------- */
        {
            STREAM << map_type().get_allocator().get_n_allocation()
                   << std::endl;
            STREAM << map_type().get_allocator().get_n_construction()
                   << std::endl;
        }
        /* -------------------------------------------------------------------------- */
    }
    /* ----------------------------------- Set ---------------------------------- */
    {
        typedef NS::set< mapped_type,
                         std::less< mapped_type >,
                         Vallocator< mapped_type > >
                               set_type;
        typedef const set_type const_set_type;

        /* ------------------------------ Construction ------------------------------ */
        {
            const mapped_type arr[] = { f(), f(), f(), f(), f() };

            set_type s1;
            set_type s2( arr, arr + sizeof( arr ) / sizeof( mapped_type ) );
            set_type s3( s2 );
            const_set_type cs( s2 );

            STREAM << s1 << std::endl;
            STREAM << s2 << std::endl;
            STREAM << s3 << std::endl;
            STREAM << cs << std::endl;

            s3 = s2;

            STREAM << s3 << std::endl;
        }
        /* -------------------------------- Iterators ------------------------------- */
        {
            const mapped_type arr[] = { f(), f(), f(), f() };
            set_type s( arr, arr + sizeof( arr ) / sizeof( mapped_type ) );

            {
                STREAM << ( typeid( set_type::iterator )
                            == typeid( set_type::const_iterator ) )
                       << std::endl;
                STREAM << ( typeid( set_type::reverse_iterator )
                            == typeid( set_type::const_reverse_iterator ) )
                       << std::endl;
            }
            {
                set_type::iterator it1;
                set_type::iterator it2 = s.begin();
                set_type::iterator it3( it2 );

                it1 = it2;

                STREAM << *it1 << std::endl;
                STREAM << *it2 << std::endl;
                STREAM << *it3 << std::endl;

                set_type::reverse_iterator rit1;
                set_type::reverse_iterator rit2 = s.rbegin();
                set_type::reverse_iterator rit3( rit2 );
                set_type::reverse_iterator rit4( s.end() );

                rit1 = rit2;

                STREAM << *rit1 << std::endl;
                STREAM << *rit2 << std::endl;
                STREAM << *rit3 << std::endl;
                STREAM << *rit4 << std::endl;
            }
            {
                set_type::iterator it = s.begin();

                STREAM << *it << std::endl;
                STREAM << *++it << std::endl;
                STREAM << *it++ << std::endl;
                STREAM << *it << std::endl;
                STREAM << *--it << std::endl;
                STREAM << *it-- << std::endl;
                STREAM << *it << std::endl;
                STREAM << it->data() << std::endl;

                STREAM << s << std::endl;

                set_type::reverse_iterator rit = s.rbegin();

                STREAM << *rit << std::endl;
                STREAM << *++rit << std::endl;
                STREAM << *rit++ << std::endl;
                STREAM << *rit << std::endl;
                STREAM << *--rit << std::endl;
                STREAM << *rit-- << std::endl;
                STREAM << *rit << std::endl;
                STREAM << rit->data() << std::endl;

                STREAM << s << std::endl;

                const_set_type cm( s );

                STREAM << *cm.begin() << std::endl;
                STREAM << *--cm.end() << std::endl;
                STREAM << *cm.rbegin() << std::endl;
                STREAM << *--cm.rend() << std::endl;
            }
            {
                set_type::iterator         it( ft::next( s.begin() ) );
                set_type::reverse_iterator rit( ft::next( s.rbegin() ) );

                STREAM << ( it == it ) << std::endl;
                STREAM << ( it == ft::next( it ) ) << std::endl;
                STREAM << ( rit == rit ) << std::endl;
                STREAM << ( rit == ft::next( rit ) ) << std::endl;

                STREAM << ( it != it ) << std::endl;
                STREAM << ( it != ft::next( it ) ) << std::endl;
                STREAM << ( rit != rit ) << std::endl;
                STREAM << ( rit != ft::next( rit ) ) << std::endl;
            }
        }
        /* -------------------------------- Capacity -------------------------------- */
        {
            set_type s;

            STREAM << s.empty() << std::endl;

            s.insert( f() );

            STREAM << s.empty() << std::endl;

            s.clear();

            STREAM << s.empty() << std::endl;
        }
        /* -------------------------------- Modifiers ------------------------------- */
        {
            const mapped_type arr[] = { f(), f(), f(), f(), f() };
            set_type s( arr, arr + sizeof( arr ) / sizeof( mapped_type ) );
            NS::pair< set_type::iterator, bool > res;

            res = s.insert( f() );

            STREAM << res.second << std::endl;
            STREAM << *res.first << std::endl;

            res = s.insert( arr[2] );

            STREAM << res.second << std::endl;
            STREAM << *res.first << std::endl;
            STREAM << s << std::endl;
            STREAM << *s.insert( s.begin(), f() ) << std::endl;
            STREAM << *s.insert( s.end(), f() ) << std::endl;
            STREAM << *s.insert( s.begin(), arr[1] ) << std::endl;
            STREAM << *s.insert( s.end(), arr[1] ) << std::endl;
            STREAM << s << std::endl;

            s.erase( s.begin() );
            s.erase( ft::next( s.begin() ) );

            STREAM << s << std::endl;
            STREAM << s.erase( arr[4] ) << std::endl;
            STREAM << s.erase( arr[4] ) << std::endl;
            STREAM << s << std::endl;

            s.erase( ft::next( ft::next( s.begin() ) ) );

            STREAM << s << std::endl;

            s.clear();

            STREAM << s << std::endl;

            s.clear();
            s.insert( arr, arr + sizeof( arr ) / sizeof( mapped_type ) );

            STREAM << s << std::endl;

            set_type s2;
            s2.insert( f() );
            s2.insert( f() );

            s.swap( s2 );

            STREAM << s << std::endl;
            STREAM << s2 << std::endl;

            s.swap( s );
            s2.swap( s2 );

            STREAM << s << std::endl;
            STREAM << s2 << std::endl;
        }
        /* -------------------------------- Observers ------------------------------- */
        {
            mapped_type x = f();
            mapped_type y = f();

            STREAM << const_set_type().key_comp()( x, y ) << std::endl;
            STREAM << const_set_type().key_comp()( y, x ) << std::endl;
            STREAM << const_set_type().value_comp()( x, y ) << std::endl;
            STREAM << const_set_type().value_comp()( y, x ) << std::endl;
        }
        /* ------------------------------- Operations ------------------------------- */
        {
            const mapped_type arr[] = { f(), f(), f(), f() };

            set_type s( arr, arr + sizeof( arr ) / sizeof( mapped_type ) );

            STREAM << ( s.find( f() ) == s.end() ) << std::endl;
            STREAM << *s.find( arr[2] ) << std::endl;

            STREAM << s.count( f() ) << std::endl;
            STREAM << s.count( arr[2] ) << std::endl;

            STREAM << *s.lower_bound( *arr ) << std::endl;
            STREAM << *s.lower_bound( arr[1] ) << std::endl;
            STREAM << ( s.lower_bound( f() ) == s.end() ) << std::endl;
            STREAM << ( s.lower_bound(
                            arr[sizeof( arr ) / sizeof( mapped_type ) - 1] )
                        == s.end() )
                   << std::endl;

            STREAM << *s.upper_bound( *arr ) << std::endl;
            STREAM << *s.upper_bound( arr[1] ) << std::endl;
            STREAM << ( s.upper_bound( f() ) == s.end() ) << std::endl;
            STREAM << ( s.upper_bound(
                            arr[sizeof( arr ) / sizeof( mapped_type ) - 1] )
                        == s.end() )
                   << std::endl;

            NS::pair< set_type::iterator, set_type::iterator > res;
            res = s.equal_range( arr[2] );

            STREAM << *res.first << std::endl;
            STREAM << std::distance( res.first, res.second ) << std::endl;

            res = s.equal_range( f() );

            STREAM << std::distance( res.first, res.second ) << std::endl;

            const_set_type cs( s );

            STREAM << *cs.find( *arr ) << std::endl;
            STREAM << cs.count( *arr ) << std::endl;
            STREAM << *cs.lower_bound( *arr ) << std::endl;
            STREAM << *cs.upper_bound( *arr ) << std::endl;
            STREAM << *cs.equal_range( arr[2] ).first << std::endl;
        }
        /* -------------------------- Relational operators -------------------------- */
        {
            mapped_type       x     = f();
            mapped_type       y     = f();
            mapped_type       z     = f();
            const mapped_type arr[] = { f(), f(), f(), f(), y };
            const_set_type    s( arr,
                              arr + sizeof( arr ) / sizeof( mapped_type ) );
            set_type          s1( s );
            set_type          s2( s );
            set_type          s3( s );
            set_type          s4( s );

            s1.erase( y );
            s1.insert( x );
            s1.erase( y );
            s2.insert( z );
            s3.erase( y );
            s4.insert( f() );

            STREAM << ( s == s1 ) << std::endl;
            STREAM << ( s == s2 ) << std::endl;
            STREAM << ( s == s3 ) << std::endl;
            STREAM << ( s == s4 ) << std::endl;

            STREAM << ( s != s1 ) << std::endl;
            STREAM << ( s != s2 ) << std::endl;
            STREAM << ( s != s3 ) << std::endl;
            STREAM << ( s != s4 ) << std::endl;

            STREAM << ( s < s1 ) << std::endl;
            STREAM << ( s < s2 ) << std::endl;
            STREAM << ( s < s3 ) << std::endl;
            STREAM << ( s < s4 ) << std::endl;

            STREAM << ( s > s1 ) << std::endl;
            STREAM << ( s > s2 ) << std::endl;
            STREAM << ( s > s3 ) << std::endl;
            STREAM << ( s > s4 ) << std::endl;

            STREAM << ( s <= s1 ) << std::endl;
            STREAM << ( s <= s2 ) << std::endl;
            STREAM << ( s <= s3 ) << std::endl;
            STREAM << ( s <= s4 ) << std::endl;

            STREAM << ( s >= s1 ) << std::endl;
            STREAM << ( s >= s2 ) << std::endl;
            STREAM << ( s >= s3 ) << std::endl;
            STREAM << ( s >= s4 ) << std::endl;
        }
        /* -------------------------------- Allocator ------------------------------- */
        {
            STREAM << set_type().get_allocator().get_n_allocation()
                   << std::endl;
            STREAM << set_type().get_allocator().get_n_construction()
                   << std::endl;
        }
        /* -------------------------------------------------------------------------- */
    }

    /* -------------------------------------------------------------------------- */
}

#endif