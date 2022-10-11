#pragma once

#include "vector.hpp"

namespace ft {

template < typename T, typename Container = vector< T > > class stack {

public:
    typedef Container                                container_type;
    typedef typename container_type::value_type      value_type;
    typedef typename container_type::size_type       size_type;
    typedef typename container_type::reference       reference;
    typedef typename container_type::const_reference const_reference;

private:
    container_type _c;

public:
    explicit stack( const container_type &c = container_type() ) : _c( c ) {}

    bool            empty() const { return _c.empty(); }
    size_type       size() const { return _c.size(); }
    reference       top() { return _c.back(); }
    const_reference top() const { return _c.back(); }
    void            push( const value_type &val ) { _c.push_back( val ); }
    void            pop() { _c.pop_back(); }

    template < typename U >
    friend bool operator==( const stack< U, Container > &lhs, const stack< U, Container > &rhs ) {
        return lhs._c == rhs._c;
    }
    template < typename U >
    friend bool operator!=( const stack< U, Container > &lhs, const stack< U, Container > &rhs ) {
        return lhs._c != rhs._c;
    }
    template < typename U >
    friend bool operator<( const stack< U, Container > &lhs, const stack< U, Container > &rhs ) {
        return lhs._c < rhs._c;
    }
    template < typename U >
    friend bool operator<=( const stack< U, Container > &lhs, const stack< U, Container > &rhs ) {
        return lhs._c <= rhs._c;
    }
    template < typename U >
    friend bool operator>( const stack< U, Container > &lhs, const stack< U, Container > &rhs ) {
        return lhs._c > rhs._c;
    }
    template < typename U >
    friend bool operator>=( const stack< U, Container > &lhs, const stack< U, Container > &rhs ) {
        return lhs._c >= rhs._c;
    }
};

}