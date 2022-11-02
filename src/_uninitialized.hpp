#pragma once

#include <memory>

namespace ft {

/* ---------------------------------- Copy ---------------------------------- */

template < typename InputIterator, typename ForwardIterator, typename Alloc >
ForwardIterator _uninitialized_copy_a( InputIterator   first,
                                       InputIterator   last,
                                       ForwardIterator dst,
                                       Alloc &         allocator ) {
    for ( ; first != last; first++, dst++ ) {
        allocator.construct( &( *dst ), *first );
    }
    return dst;
}

template < typename InputIterator, typename ForwardIterator, typename T >
inline ForwardIterator _uninitialized_copy_a( InputIterator   first,
                                              InputIterator   last,
                                              ForwardIterator dst,
                                              std::allocator< T > & ) {
    return std::uninitialized_copy( first, last, dst );
}

/* --------------------------------- Destroy -------------------------------- */

template < typename T > inline void _destroy( T *p ) { p->~T(); }

template < bool > struct _Destroy_aux {
    template < typename ForwardIterator >
    static void __destroy( ForwardIterator first, ForwardIterator last ) {
        for ( ; first != last; first++ ) _destroy( &( *first ) );
    }
};

template <> struct _Destroy_aux< true > {
    template < typename ForwardIterator >
    static void __destroy( ForwardIterator, ForwardIterator ) {}
};

template < typename ForwardIterator >
inline void _Destroy( ForwardIterator first, ForwardIterator last ) {
    typedef typename std::iterator_traits< ForwardIterator >::value_type
        _Value_type;

    std::_Destroy_aux< __has_trivial_destructor( _Value_type ) >::__destroy(
        first,
        last );
}

template < typename ForwardIterator, typename Alloc >
void _destroy( ForwardIterator first, ForwardIterator last, Alloc &allocator ) {
    for ( ; first != last; first++ ) { allocator.destroy( &( *first ) ); }
}

template < typename ForwardIterator, typename T >
inline void
_destroy( ForwardIterator first, ForwardIterator last, std::allocator< T > & ) {
    _Destroy( first, last );
}

/* -------------------------------------------------------------------------- */
}