#pragma once

#include "rb_tree.hpp"

namespace ft {

template < typename T,
           typename Compare = std::less< T >,
           typename Alloc   = std::allocator< T > >
class set {

    /* ------------------------------ Member types ------------------------------ */

    typedef rb_tree< T, T, Compare, Alloc >      tree_type;
    typedef typename tree_type::iterator         tree_iterator;
    typedef typename tree_type::const_iterator   tree_const_iterator;
    typedef typename tree_type::reverse_iterator tree_reverse_iterator;
    typedef
        typename tree_type::const_reverse_iterator tree_const_reverse_iterator;

public:
    typedef typename tree_type::key_type             key_type;
    typedef typename tree_type::key_type             value_type;
    typedef typename tree_type::key_compare          key_compare;
    typedef typename tree_type::key_compare          value_compare;
    typedef Alloc                                    allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef typename tree_type::size_type            size_type;

    /* -------------------------------- Iterator -------------------------------- */

    template < typename U > class Iterator {
    private:
        U _it;

    public:
        Iterator() : _it( U() ) {}
        Iterator( const Iterator &other ) : _it( other._it ) {}
        Iterator( const U &other ) : _it( other ) {}
        Iterator &operator=( const Iterator &other ) {
            _it = other._it;
            return *this;
        }

        Iterator &operator++() {
            _it++;
            return *this;
        }
        Iterator  operator++( int ) { return _it++; }
        Iterator &operator--() {
            _it--;
            return *this;
        }
        Iterator operator--( int ) { return _it--; }

        const_reference operator*() const { return _it->first; }
        const_pointer   operator->() const { return _it.operator->().first; }

        template < typename V >
        bool operator==( const Iterator< V > &other ) const {
            return U( *this ) == U( other );
        }
        bool operator==( const U &other ) const { return U( *this ) == other; }
        template < typename V >
        bool operator!=( const Iterator< V > &other ) const {
            return !( *this == other );
        }
        bool operator!=( const U &other ) const { return !( *this == other ); }

        operator U() const { return U( _it ); }
        operator Iterator< tree_const_iterator >() const {
            return Iterator< tree_const_iterator >( _it );
        }
    };

    typedef Iterator< tree_iterator >              iterator;
    typedef Iterator< tree_const_iterator >        const_iterator;
    typedef ft::reverse_iterator< iterator >       reverse_iterator;
    typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;

private:
    /* --------------------------------- Members -------------------------------- */

    tree_type _tree;

public:
    /* ------------------------------ Construction ------------------------------ */

    explicit set( const key_compare    &comp  = key_compare(),
                  const allocator_type &alloc = allocator_type() )
        : _tree( comp ) {}

    template < class InputIterator >
    set( InputIterator         first,
         InputIterator         last,
         const key_compare    &comp  = key_compare(),
         const allocator_type &alloc = allocator_type() );

    set( const set &x );

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
    size_type max_size() const { return 5; }

    /* -------------------------------- Modifiers ------------------------------- */

    ft::pair< iterator, bool > insert( const value_type &val ) {
        iterator it = _tree.find( val );
        if ( it != _tree.end() ) {
            return ft::pair< iterator, bool >( it, false );
        }
        _tree.insert( typename tree_type::value_type( val, val ) );
        return ft::pair< iterator, bool >( _tree.find( val ), true );
    }

    iterator insert( iterator position, const value_type &val ) {
        typename tree_type::value_type data =
            typename tree_type::value_type( val, val );
        return _tree.insert( data );
    }
};

}