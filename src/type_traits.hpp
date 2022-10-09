#pragma once

namespace ft {

/* -------------------------------- enable_if ------------------------------- */

template < bool B, class T = void > struct enable_if {};

template < class T > struct enable_if< true, T > { typedef T type; };

/* ---------------------------- integral_constant --------------------------- */

template < class T, T v > struct integral_constant {
    static const T                    value = v;
    typedef T                         value_type;
    typedef integral_constant< T, v > type;
};

typedef integral_constant< bool, true >  true_type;
typedef integral_constant< bool, false > false_type;

/* ------------------------------- is_integral ------------------------------ */

template < typename > struct __is_integral_helper : public false_type {};

template <> struct __is_integral_helper< bool > : public true_type {};

template <> struct __is_integral_helper< char > : public true_type {};

template <> struct __is_integral_helper< signed char > : public true_type {};

template <> struct __is_integral_helper< unsigned char > : public true_type {};

template <> struct __is_integral_helper< short > : public true_type {};

template <> struct __is_integral_helper< unsigned short > : public true_type {};

template <> struct __is_integral_helper< int > : public true_type {};

template <> struct __is_integral_helper< unsigned int > : public true_type {};

template <> struct __is_integral_helper< long > : public true_type {};

template <> struct __is_integral_helper< unsigned long > : public true_type {};

template < class T > struct is_integral : public __is_integral_helper< T > {};
}