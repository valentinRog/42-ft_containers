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

/* -------------------------------------------------------------------------- */
}