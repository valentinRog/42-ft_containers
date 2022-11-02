#pragma once

#include "_uninitialized.hpp"
#include "algorithm.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <typeinfo>

namespace ft {

template < typename T, class Allocator = std::allocator< T > > class vector {

    /* -------------------------------- Iterator -------------------------------- */

    template < typename U > class Iterator {
    public:
        typedef U                               value_type;
        typedef U &                             reference;
        typedef U *                             pointer;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        Iterator() : _p( 0 ) {}
        Iterator( pointer p ) : _p( p ) {}
        Iterator( const Iterator &other ) : _p( other.operator->() ) {}

        Iterator &operator=( const Iterator &other ) {
            _p = other._p;
            return *this;
        }

        Iterator        operator+( difference_type n ) const { return _p + n; }
        friend Iterator operator+( difference_type lhs, const Iterator &rhs ) {
            return rhs + lhs;
        }
        Iterator        operator-( difference_type n ) const { return _p - n; }
        difference_type operator-( const Iterator &other ) const {
            return _p - other._p;
        }

        Iterator &operator++() {
            _p++;
            return *this;
        }
        Iterator &operator--() {
            _p--;
            return *this;
        }
        Iterator operator++( int ) {
            Iterator copy( *this );
            _p++;
            return copy;
        }
        Iterator operator--( int ) {
            Iterator copy( *this );
            _p--;
            return copy;
        }

        Iterator operator+=( difference_type n ) {
            _p += n;
            return *this;
        }
        Iterator operator-=( difference_type n ) {
            _p -= n;
            return *this;
        }

        template < typename V >
        bool operator==( const Iterator< V > &other ) const {
            return ( _p == other.operator->() );
        };
        template < typename V >
        bool operator!=( const Iterator< V > &other ) const {
            return ( _p != other.operator->() );
        };
        template < typename V >
        bool operator>( const Iterator< V > &other ) const {
            return ( _p > other.operator->() );
        };
        template < typename V >
        bool operator<( const Iterator< V > &other ) const {
            return ( _p < other.operator->() );
        };
        template < typename V >
        bool operator>=( const Iterator< V > &other ) const {
            return ( _p >= other.operator->() );
        };
        template < typename V >
        bool operator<=( const Iterator< V > &other ) const {
            return ( _p <= other.operator->() );
        };

        reference operator*() { return *_p; }
        reference operator[]( difference_type i ) { return _p[i]; }
        pointer   operator->() const { return ( _p ); };

        operator Iterator< const U >() const {
            return ( Iterator< const U >( _p ) );
        }
    };

    /* ------------------------------ Member types ------------------------------ */

public:
    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef Iterator< value_type >                   iterator;
    typedef Iterator< const value_type >             const_iterator;
    typedef ft::reverse_iterator< iterator >         reverse_iterator;
    typedef ft::reverse_iterator< const_iterator >   const_reverse_iterator;
    typedef std::size_t                              size_type;

    /* ------------------------------- Attributes ------------------------------- */

protected:
    allocator_type _allocator;
    pointer        _data;
    size_type      _capacity;
    size_type      _size;

    /* ------------------------------ Construction ------------------------------ */

public:
    explicit vector( const allocator_type &alloc = allocator_type() )
        : _allocator( alloc ),
          _data( 0 ),
          _capacity( 0 ),
          _size( 0 ) {}

    explicit vector( size_type             n,
                     const value_type &    val   = value_type(),
                     const allocator_type &alloc = allocator_type() )
        : _allocator( alloc ),
          _data( 0 ),
          _capacity( 0 ),
          _size( 0 ) {
        assign( n, val );
    }

    template < class U >
    vector( U first, U last, const allocator_type &alloc = allocator_type() )
        : _allocator( alloc ),
          _data( 0 ),
          _capacity( 0 ),
          _size( 0 ) {
        assign( first, last );
    }

    vector( const vector &other )
        : _allocator( other._allocator ),
          _data( 0 ),
          _capacity( 0 ),
          _size( 0 ) {
        *this = other;
    }

    virtual ~vector() {
        clear();
        _allocator.deallocate( _data, _capacity );
    }

    vector &operator=( const vector &other ) {
        assign( other.begin(), other.end() );
        return *this;
    }

    /* -------------------------------- Capacity -------------------------------- */

    size_type size() const { return _size; }

    size_type max_size() const { return _allocator.max_size(); }

    void resize( size_type n, value_type val = value_type() ) {
        if ( n > _size ) {
            insert( end(), n - _size, val );
        } else {
            erase( begin() + n, end() );
        }
    }

    size_type capacity() const { return _capacity; }

    bool empty() const { return !_size; }

    void reserve( size_type n ) {
        if ( n > _capacity ) {
            size_type len( 1 );
            while ( len < n ) { len <<= 1; }
            pointer tmp = _allocator.allocate( len );
            ft::_uninitialized_copy_a( _data, _data + _size, tmp, _allocator );
            ft::_destroy( _data, _data + _size, _allocator );
            _allocator.deallocate( _data, _capacity );
            _capacity = len;
            _data     = tmp;
        }
    }

    /* -------------------------------- Iterators ------------------------------- */

    iterator               begin() { return _data; }
    const_iterator         begin() const { return _data; }
    iterator               end() { return _data + _size; }
    const_iterator         end() const { return _data + _size; }
    reverse_iterator       rbegin() { return _data + _size; }
    const_reverse_iterator rbegin() const { return _data + _size; };
    reverse_iterator       rend() { return _data; }
    const_reverse_iterator rend() const { return _data; }
    const_iterator         cbegin() const { return _data; }
    const_iterator         cend() const { return _data + _size; }
    const_reverse_iterator crbegin() const { return _data + _size; }
    const_reverse_iterator crend() const { return _data; };

    /* ----------------------------- Element access ----------------------------- */

    reference operator[]( size_type i ) {
        return i >= _size ? *end() : _data[i];
    }
    const_reference operator[]( size_type i ) const {
        return i >= _size ? *end() : _data[i];
    }

    reference at( size_type i ) {
        if ( i >= _size ) { throw std::out_of_range( "Trop grand frere" ); }
        return operator[]( i );
    }

    const_reference at( size_type i ) const {
        if ( i >= _size ) { throw std::out_of_range( "Trop grand frere" ); }
        return operator[]( i );
    }

    reference         front() { return *begin(); }
    const_reference   front() const { return *begin(); }
    reference         back() { return *rbegin(); }
    const_reference   back() const { return *rbegin(); }
    value_type *      data() { return _data; }
    const value_type *data() const { return _data; }

    /* -------------------------------- Modifiers ------------------------------- */

    template < class U >
    void assign( U first,
                 U last,
                 typename ft::enable_if< !ft::is_integral< U >::value, U >::type
                     * = 0 ) {
        clear();
        insert( begin(), first, last );
    }

    void assign( size_type n, const value_type &val ) {
        clear();
        insert( begin(), n, val );
    }

    void push_back( const value_type &val ) {
        reserve( _size + 1 );
        _allocator.construct( _data + _size, val );
        _size++;
    }

    void pop_back() {
        if ( _size ) {
            _allocator.destroy( _data + _size - 1 );
            _size--;
        }
    }

    iterator insert( iterator position, const value_type &val ) {
        typename iterator::difference_type i = position - begin();
        insert( position, 1, val );
        return begin() + i;
    }

    void insert( iterator position, size_type n, const value_type &val ) {
        typename iterator::difference_type i = position - begin();
        reserve( _size + n );
        size_type x = std::min( n, _size - i );
        ft::_uninitialized_copy_a( _data + _size - x,
                                   _data + _size,
                                   _data + _size - x + n,
                                   _allocator );
        std::copy_backward( _data + i,
                            _data + _size - x,
                            _data + _size - x + n );
        std::uninitialized_fill( _data + _size, _data + _size - x + n, val );
        std::fill( _data + i, _data + i + x, val );
        _size += n;
    }

    template < typename U >
    void insert( iterator position,
                 U        first,
                 U        last,
                 typename ft::enable_if< !ft::is_integral< U >::value, U >::type
                     * = 0 ) {
        size_type i = position - begin();
        size_type n = std::distance( first, last );
        reserve( _size + n );
        size_type x = std::min( n, _size - i );
        ft::_uninitialized_copy_a( _data + _size - x,
                                   _data + _size,
                                   _data + _size - x + n,
                                   _allocator );
        std::copy_backward( _data + i,
                            _data + _size - x,
                            _data + _size - x + n );
        U tmp( last );
        std::advance( tmp, x - n );
        std::copy( first, tmp, _data + i );
        ft::_uninitialized_copy_a( tmp, last, _data + _size, _allocator );
        _size += n;
    }

    iterator erase( iterator position ) {
        return erase( position, position + 1 );
    }

    iterator erase( iterator first, iterator last ) {
        std::copy( last, end(), first );
        ft::_destroy( last, end(), _allocator );
        _size -= last - first;
        return first;
    }

    void swap( vector &other ) {
        ft::swap( _data, other._data );
        ft::swap( _capacity, other._capacity );
        ft::swap( _size, other._size );
    }

    void clear() {
        ft::_destroy( begin(), end(), _allocator );
        _size = 0;
    }

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() const { return _allocator; }

    /* -------------------------- Relational operators -------------------------- */

    bool operator==( const vector &other ) const {
        return size() == other.size() && equal( begin(), end(), other.begin() );
    }
    bool operator!=( const vector &other ) const { return !( *this == other ); }
    bool operator<( const vector &other ) const {
        return lexicographical_compare( begin(),
                                        end(),
                                        other.begin(),
                                        other.end() );
    }
    bool operator<=( const vector &other ) const {
        return *this == other || *this < other;
    }
    bool operator>( const vector &other ) const { return !( *this <= other ); }
    bool operator>=( const vector &other ) const { return !( *this < other ); }

    /* -------------------------------------------------------------------------- */
};

/* ---------------------------------- Swap ---------------------------------- */

template < typename T > void swap( vector< T > &x, vector< T > &y ) {
    x.swap( y );
}

/* -------------------------------------------------------------------------- */

}
