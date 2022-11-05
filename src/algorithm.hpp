#pragma once

namespace ft {

/* ---------------------------------- Swap ---------------------------------- */

template < typename T > void swap( T &a, T &b ) {
    T tmp( a );
    a = b;
    b = tmp;
}

/* ---------------------------------- Equal --------------------------------- */

template < typename T1, typename T2 >
bool equal( T1 first1, T1 last1, T2 first2 ) {
    for ( ; first1 != last1; first1++, first2++ ) {
        if ( !( *first1 == *first2 ) ) { return false; }
    }
    return true;
}

/* ------------------------- Lexicographical compare ------------------------ */

template < typename T1, typename T2 >
bool lexicographical_compare( T1 first1, T1 last1, T2 first2, T2 last2 ) {
    for ( ; first1 != last1; first1++, first2++ ) {
        if ( first2 == last2 || *first2 < *first1 ) {
            return false;
        } else if ( *first1 < *first2 ) {
            return true;
        }
    }
    return first2 != last2;
}

/* -------------------------------------------------------------------------- */

}
