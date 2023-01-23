#pragma once

#include "iterator.hpp"
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

/* ---------------------------------- Fill ---------------------------------- */

template < typename ForwardIterator, typename T, typename A >
void _uninitialized_fill_a( ForwardIterator first,
                            ForwardIterator last,
                            const T &       x,
                            A &             allocator ) {
    for ( ; first != last; first++ ) { allocator.construct( &( *first ), x ); }
}

template < typename ForwardIterator, typename T, typename U >
void _uninitialized_fill_a( ForwardIterator first,
                            ForwardIterator last,
                            const T &       x,
                            std::allocator< U > & ) {
    return std::uninitialized_fill( first, last, x );
}

/* --------------------------------- Destroy -------------------------------- */

template < typename T > inline void _destroy( T *p ) { p->~T(); }

template < bool > struct _Destroy_aux {
    template < typename ForwardIterator >
    static void _destroy( ForwardIterator first, ForwardIterator last ) {
        for ( ; first != last; first++ ) ft::_destroy( &( *first ) );
    }
};

template <> struct _Destroy_aux< true > {
    template < typename ForwardIterator >
    static void _destroy( ForwardIterator, ForwardIterator ) {}
};

template < typename ForwardIterator >
inline void _destroy( ForwardIterator first, ForwardIterator last ) {
#ifdef __GNUG__
    _Destroy_aux< __has_trivial_destructor(
        typename ft::iterator_traits< ForwardIterator >::value_type ) >::
        _destroy( first, last );
#else
    _Destroy_aux< false >::_destroy( first, last );
#endif
}

template < typename ForwardIterator, typename Alloc >
void _destroy( ForwardIterator first, ForwardIterator last, Alloc &allocator ) {
    for ( ; first != last; first++ ) { allocator.destroy( &( *first ) ); }
}

template < typename ForwardIterator, typename T >
inline void
_destroy( ForwardIterator first, ForwardIterator last, std::allocator< T > & ) {
    _destroy( first, last );
}

/* -------------------------------------------------------------------------- */

}