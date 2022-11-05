#pragma once

#include "vector.hpp"

namespace ft {

template < typename T, typename Container = vector< T > > class stack {

public:
    typedef Container                                container_type;
    typedef typename container_type::value_type      value_type;
    typedef typename container_type::size_type       size_type;

private:
    typedef typename container_type::reference       reference;
    typedef typename container_type::const_reference const_reference;

protected:
    container_type c;

public:
    explicit stack( const container_type &c = container_type() ) : c( c ) {}

    bool            empty() const { return c.empty(); }
    size_type       size() const { return c.size(); }
    reference       top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void            push( const value_type &val ) { c.push_back( val ); }
    void            pop() { c.pop_back(); }

    bool operator==( const stack &other ) const { return c == other.c; }
    bool operator!=( const stack &other ) const { return c != other.c; }
    bool operator<( const stack &other ) const { return c < other.c; }
    bool operator<=( const stack &other ) const { return c <= other.c; }
    bool operator>( const stack &other ) const { return c > other.c; }
    bool operator>=( const stack &other ) const { return c >= other.c; }
};
}