#pragma once

#include <memory>

namespace ft {

/* ---------------------------------- Copy ---------------------------------- */

// Extensions: versions of uninitialized_copy, uninitialized_fill,
//  and uninitialized_fill_n that take an allocator parameter.
//  We dispatch back to the standard versions when we're given the
//  default allocator.  For nondefault allocators we do not use
//  any of the POD optimizations.

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