#pragma once

namespace ft {

/* ---------------------------- Reverse iterator ---------------------------- */

template < typename T > class reverse_iterator {
public:
    typedef typename T::value_type        value_type;
    typedef typename T::reference         reference;
    typedef typename T::pointer           pointer;
    typedef typename T::difference_type   difference_type;
    typedef typename T::iterator_category iterator_category;

private:
    T _it;

public:
    reverse_iterator() {}
    reverse_iterator( pointer p ) : _it( T( p ) ) {}
    reverse_iterator( const T &other ) : _it( other ) {}
    template < typename U >
    reverse_iterator( const reverse_iterator< U > &other )
        : _it( other.base() ) {}

    template < typename U >
    reverse_iterator &operator=( const reverse_iterator< U > &other ) {
        _it = other.base();
        return *this;
    }

    T base() const { return _it; }

    reverse_iterator operator+( difference_type n ) const { return _it - n; }
    friend reverse_iterator operator+( difference_type         n,
                                       const reverse_iterator &other ) {
        return other + n;
    };
    reverse_iterator operator-( difference_type n ) const { return _it + n; }
    difference_type  operator-( const reverse_iterator &other ) const {
        return other.base() - _it;
    }

    reverse_iterator operator++() { return --_it; }
    reverse_iterator operator--() { return ++_it; }
    reverse_iterator operator++( int ) { return _it--; }
    reverse_iterator operator--( int ) { return _it++; }

    reverse_iterator &operator+=( difference_type n ) {
        _it -= n;
        return *this;
    };
    reverse_iterator &operator-=( difference_type n ) {
        _it += n;
        return *this;
    };

    template < typename U >
    bool operator==( const reverse_iterator< U > &other ) const {
        return ( _it == other.base() );
    };
    template < typename U >
    bool operator!=( const reverse_iterator< U > &other ) const {
        return ( _it != other.base() );
    };
    template < typename U >
    bool operator>( const reverse_iterator< U > &other ) const {
        return ( _it < other.base() );
    };
    template < typename U >
    bool operator<( const reverse_iterator< U > &other ) const {
        return ( _it > other.base() );
    };
    template < typename U >
    bool operator>=( const reverse_iterator< U > &other ) const {
        return ( _it <= other.base() );
    };
    template < typename U >
    bool operator<=( const reverse_iterator< U > &other ) const {
        return ( _it >= other.base() );
    };

    reference       operator*() { return *( --T( _it ) ); }
    const reference operator*() const { return *( --T( _it ) ); }
    reference       operator[]( difference_type i ) { return *operator+( i ); }
    const reference operator[]( difference_type i ) const {
        return *operator+( i );
    }
    pointer       operator->() { return ( --T( _it ) ).operator->(); };
    const pointer operator->() const { return ( --T( _it ) ).operator->(); };
};

/* -------------------------------------------------------------------------- */

}