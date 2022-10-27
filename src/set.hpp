#pragma once

#include "algorithm.hpp"
#include "rb_tree.hpp"
#include "utility.hpp"

namespace ft {

template < typename T,
           typename Compare = std::less< T >,
           typename Alloc   = std::allocator< T > >
class set {

    /* ------------------------------ Member types ------------------------------ */

    typedef rb_tree< T, T, Compare, Alloc >      tree_type;
    typedef typename tree_type::iterator         tree_iterator;
    typedef typename tree_type::const_iterator   tree_const_iterator;

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

    class Iterator {
    private:
        tree_const_iterator _it;

    public:
        Iterator() : _it( tree_const_iterator() ) {}
        Iterator( const Iterator &other ) : _it( other._it ) {}
        Iterator( const tree_const_iterator &other ) : _it( other ) {}
        Iterator( const tree_iterator &other ) : _it( other ) {}
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
        const_pointer   operator->() const { return _it.operator->(); }

        bool operator==( const Iterator &other ) const {
            return _it == other._it;
        }
        bool operator!=( const Iterator &other ) const {
            return _it != other._it;
        }

        operator tree_const_iterator() const { return _it; }
        //operator Iterator< tree_const_iterator >() const {
        //return Iterator< tree_const_iterator >( _it );
        //}
    };

    typedef Iterator                               iterator;
    typedef Iterator                               const_iterator;
    typedef ft::reverse_iterator< iterator >       reverse_iterator;
    typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;

private:
    /* --------------------------------- Members -------------------------------- */

    tree_type _tree;

public:
    /* ------------------------------ Construction ------------------------------ */

    explicit set( const key_compare    &comp  = key_compare(),
                  const allocator_type &alloc = allocator_type() )
        : _tree( comp, alloc ) {}

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
        return _tree.insert( position,
                             typename tree_type::value_type( val, val ) );
    }

    template < class InputIterator >
    void insert( InputIterator first, InputIterator last ) {
        for ( ; first != last; first++ ) {
            _tree.insert( typename tree_type::value_type( *first, *first ) );
        }
    }

    void      erase( iterator position ) { _tree.erase( *position ); }
    size_type erase( const value_type &val ) { return _tree.erase( val ); }
    void      erase( iterator first, iterator last ) {
        for ( ; first != last; first++ ) { erase( first ); }
    }

    void swap( set &other ) { ft::swap( _tree, other._tree ); }

    void clear() { _tree.clear(); }

    /* -------------------------------- Observers ------------------------------- */

    key_compare   key_comp() const { return key_compare(); }
    value_compare value_comp() const { return value_compare(); }

    /* ------------------------------- Operations ------------------------------- */

    iterator  find( const value_type &val ) const { return _tree.find( val ); }
    size_type count( const value_type &val ) const {
        return _tree.find( val ) != end();
    }
    iterator lower_bound( const value_type &val ) const {
        return _tree.lower_bound( val );
    }
    iterator upper_bound( const value_type &val ) const {
        return _tree.upper_bound( val );
    }
    pair< iterator, iterator > equal_range( const value_type &val ) const {
        return ft::pair< iterator, iterator >( lower_bound( val ),
                                               upper_bound( val ) );
    }

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() const { return _tree.get_allocator(); }

    /* -------------------------- Relational operators -------------------------- */

    bool operator==( const set &other ) const { return _tree == other._tree; }
    bool operator!=( const set &other ) const { return _tree != other._tree; }
    bool operator<( const set &other ) const { return _tree < other._tree; }
    bool operator<=( const set &other ) const { return _tree <= other._tree; }
    bool operator>( const set &other ) const { return _tree > other._tree; }
    bool operator>=( const set &other ) const { return _tree >= other._tree; }

    /* -------------------------------------------------------------------------- */
};

/* ---------------------------------- Swap ---------------------------------- */

template < typename T, class Compare, class Alloc >
void swap( set< T, Compare, Alloc > &lhs, set< T, Compare, Alloc > &rhs ) {
    lhs.swap( rhs );
}

/* -------------------------------------------------------------------------- */

}