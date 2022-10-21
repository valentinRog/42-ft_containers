#pragma once

#include "rb_tree.hpp"

namespace ft {
template < typename K,
           typename V,
           typename Less      = std::less< K >,
           typename Allocator = std::allocator< std::pair< const K, V > > >
class map {

public:
    /* ------------------------------ Member types ------------------------------ */

    typedef typename ft::rb_tree< K, V, Less, Allocator > tree_type;
    typedef typename tree_type::key_type                  key_type;
    typedef typename tree_type::mapped_type               mapped_type;
    typedef typename tree_type::value_type                value_type;
    typedef typename tree_type::key_compare               key_compare;
    typedef typename tree_type::allocator_type            allocator_type;
    typedef typename tree_type::reference                 reference;
    typedef typename tree_type::const_reference           const_reference;
    typedef typename tree_type::pointer                   pointer;
    typedef typename tree_type::const_pointer             const_pointer;
    typedef typename tree_type::iterator                  tree_iterator;
    typedef typename tree_type::const_iterator            tree_const_iterator;
    typedef typename tree_type::reverse_iterator          tree_reverse_iterator;
    typedef
        typename tree_type::const_reverse_iterator tree_const_reverse_iterator;
    typedef typename tree_type::difference_type    difference_type;
    typedef typename tree_type::size_type          size_type;

    struct value_compare {
        bool operator()( const value_type &a, const value_type &b ) const {
            return key_compare( a.first, b.first );
        }
    };

    /* -------------------------------- Iterator -------------------------------- */

    template < typename T > class Iterator {
    public:
        typedef typename T::reference         reference;
        typedef typename T::pointer           pointer;
        typedef typename T::difference_type   difference_type;
        typedef typename T::iterator_category iterator_category;

    private:
        T _it;

    public:
        Iterator() : _it( T() ) {}
        Iterator( const Iterator &other ) : _it( other._it ) {}
        Iterator( const T &other ) : _it( other ) {}
        Iterator &operator=( const Iterator &other ) {
            _it = other._it;
            return *this;
        }

        Iterator &operator++() { return ++_it; }
        Iterator  operator++( int ) { return _it++; }
        Iterator &operator--() { return --_it; }
        Iterator  operator--( int ) { return _it--; }

        reference operator*() { return *_it; }
        pointer   operator->() const { return _it.operator->(); }

        template < typename U >
        bool operator==( const Iterator< U > &other ) const {
            return T( *this ) == U( other );
        }
        bool operator==( const T &other ) const { return T( *this ) == other; }
        template < typename U >
        bool operator!=( const Iterator< U > &other ) const {
            return !( *this == other );
        }
        bool operator!=( const T &other ) const { return !( *this == other ); }

        operator T() const { return T( _it ); }
        operator Iterator< tree_const_iterator >() const {
            return Iterator< tree_const_iterator >( _it );
        }
    };

    typedef Iterator< tree_iterator >               iterator;
    typedef Iterator< tree_const_iterator >         const_iterator;
    typedef Iterator< tree_reverse_iterator >       reverse_iterator;
    typedef Iterator< tree_const_reverse_iterator > const_reverse_iterator;

private:
    /* --------------------------------- Members -------------------------------- */

    tree_type _tree;

public:
    /* ------------------------------ Construction ------------------------------ */

    explicit map( const key_compare &   comp  = key_compare(),
                  const allocator_type &alloc = allocator_type() )
        : _tree( tree_type( comp, alloc ) ) {}

    /* -------------------------------- Iterators ------------------------------- */

    iterator               begin() { return _tree.begin(); }
    const_iterator         begin() const { return _tree.begin(); }
    iterator               end() { return _tree.end(); }
    const_iterator         end() const { return _tree.end(); }
    reverse_iterator       rbegin() { return _tree.rbegin(); }
    const_reverse_iterator rbegin() const { return _tree.rbegin(); }
    reverse_iterator       rend() { return _tree.rend(); }
    const_reverse_iterator rend() const { return _tree.rend(); }
    const_iterator         cbegin() const { return _tree.cbegin(); }
    const_iterator         cend() const { return _tree.cend(); }
    const_reverse_iterator crbegin() const { return _tree.crbegin(); }
    const_reverse_iterator crend() const { return _tree.crend(); }

    /* -------------------------------- Capacity -------------------------------- */

    bool      empty() const { return !size(); }
    size_type size() const { return _tree.size(); }
    size_type max_size() const;

    /* ----------------------------- Element access ----------------------------- */

    mapped_type &operator[]( const key_type &k );

    mapped_type &find( const key_type &k ) { return _tree.find( k )->second; }

    /* -------------------------------- Modifiers ------------------------------- */

    std::pair< iterator, bool > insert( const value_type &val ) {
        iterator it = _tree.find( val.first );
        if ( it != _tree.end() ) { return std::make_pair( it, false ); }
        _tree.insert( val );
        it = _tree.find( val.first );
        return std::make_pair( it, true );
    }
    iterator insert( iterator position, const value_type &val ) {
        return _tree.insert( position, val );
    }
    template < class InputIterator >
    void insert( InputIterator first, InputIterator last );
};
}