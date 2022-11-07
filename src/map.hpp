#pragma once

#include "_Rb_tree.hpp"
#include "algorithm.hpp"
#include "iterator.hpp"

namespace ft {
template < typename K,
           typename V,
           typename Comp      = std::less< K >,
           typename Allocator = std::allocator< ft::pair< const K, V > > >
class map {

    /* ------------------------------ Member types ------------------------------ */

    typedef typename ft::_Rb_tree< K, V, Comp, Allocator > tree_type;
    typedef typename tree_type::iterator                   tree_iterator;
    typedef typename tree_type::const_iterator             tree_const_iterator;

public:
    typedef typename tree_type::key_type        key_type;
    typedef typename tree_type::mapped_type     mapped_type;
    typedef typename tree_type::value_type      value_type;
    typedef typename tree_type::key_compare     key_compare;
    typedef typename tree_type::allocator_type  allocator_type;
    typedef typename tree_type::reference       reference;
    typedef typename tree_type::const_reference const_reference;
    typedef typename tree_type::pointer         pointer;
    typedef typename tree_type::const_pointer   const_pointer;
    typedef typename tree_type::difference_type difference_type;
    typedef typename tree_type::size_type       size_type;

    struct value_compare {
        bool operator()( const value_type &a, const value_type &b ) const {
            return key_compare()( a.first, b.first );
        }
    };

    /* -------------------------------- Iterator -------------------------------- */

    template < typename T > class Iterator {
    public:
        typedef typename T::value_type        value_type;
        typedef typename T::reference         reference;
        typedef typename T::const_reference   const_reference;
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

        reference       operator*() { return *_it; }
        const_reference operator*() const { return *_it; }
        pointer         operator->() const { return _it.operator->(); }

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

    typedef Iterator< tree_iterator >              iterator;
    typedef Iterator< tree_const_iterator >        const_iterator;
    typedef ft::reverse_iterator< iterator >       reverse_iterator;
    typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;

private:
    /* --------------------------------- Members -------------------------------- */

    tree_type _tree;

public:
    /* ------------------------------ Construction ------------------------------ */

    explicit map( const key_compare &   comp  = key_compare(),
                  const allocator_type &alloc = allocator_type() )
        : _tree( tree_type( comp, alloc ) ) {}

    template < class InputIterator >
    map( InputIterator         first,
         InputIterator         last,
         const key_compare &   comp  = key_compare(),
         const allocator_type &alloc = allocator_type() )
        : _tree( tree_type( comp, alloc ) ) {
        insert( first, last );
    }
    map( const map &other ) : _tree( other._tree ) {}
    ~map() {}
    map &operator=( const map &other ) {
        _tree = other._tree;
        return *this;
    }

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
    size_type max_size() const { return _tree.max_size(); }

    /* ----------------------------- Element access ----------------------------- */

    mapped_type &operator[]( const key_type &k ) {
        return _tree.insert( value_type( k, mapped_type() ) ).first->second;
    }
    mapped_type &at( const key_type &k ) {
        iterator it = _tree.find( k );
        if ( it == end() ) { throw std::out_of_range( "nooooooon" ); }
        return it->second;
    }
    const mapped_type &at( const key_type &k ) const {
        const_iterator it = _tree.find( k );
        if ( it == end() ) { throw std::out_of_range( "nooooooon" ); }
        return it->second;
    }

    /* -------------------------------- Modifiers ------------------------------- */

    ft::pair< iterator, bool > insert( const value_type &val ) {
        return _tree.insert( val );
    }
    iterator insert( iterator position, const value_type &val ) {
        return _tree.insert( position, val ).first;
    }
    template < class InputIterator >
    void insert( InputIterator first, InputIterator last ) {
        _tree.insert( first, last );
    }

    void      erase( iterator position ) { _tree.erase( position->first ); }
    size_type erase( const key_type &k ) { return _tree.erase( k ); }
    void erase( iterator first, iterator last ) { _tree.erase( first, last ); }

    void swap( map &other ) { ft::swap( _tree, other._tree ); }

    void clear() { _tree.clear(); }

    /* -------------------------------- Observers ------------------------------- */

    key_compare   key_comp() const { return key_compare(); }
    value_compare value_comp() const { return value_compare(); }

    /* ------------------------------- Operations ------------------------------- */

    iterator       find( const key_type &k ) { return _tree.find( k ); }
    const_iterator find( const key_type &k ) const { return _tree.find( k ); }
    size_type      count( const key_type &k ) const {
        return _tree.find( k ) != _tree.end();
    }
    iterator lower_bound( const key_type &k ) { return _tree.lower_bound( k ); }
    const_iterator lower_bound( const key_type &k ) const {
        return _tree.lower_bound( k );
    }
    iterator upper_bound( const key_type &k ) { return _tree.upper_bound( k ); }
    const_iterator upper_bound( const key_type &k ) const {
        return _tree.upper_bound( k );
    }
    ft::pair< const_iterator, const_iterator >
    equal_range( const key_type &k ) const {
        return ft::pair< const_iterator, const_iterator >( lower_bound( k ),
                                                           upper_bound( k ) );
    }
    ft::pair< iterator, iterator > equal_range( const key_type &k ) {
        return ft::pair< iterator, iterator >( lower_bound( k ),
                                               upper_bound( k ) );
    }

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() const { return _tree.get_allocator(); }

    /* -------------------------- Relational operators -------------------------- */

    bool operator==( const map &other ) const { return _tree == other._tree; }
    bool operator!=( const map &other ) const { return _tree != other._tree; }
    bool operator<( const map &other ) const { return _tree < other._tree; }
    bool operator<=( const map &other ) const { return _tree <= other._tree; }
    bool operator>( const map &other ) const { return _tree > other._tree; }
    bool operator>=( const map &other ) const { return _tree >= other._tree; }

    /* -------------------------------------------------------------------------- */
};

/* ---------------------------------- Swap ---------------------------------- */

template < class Key, class T, class Compare, class Alloc >
void swap( map< Key, T, Compare, Alloc > &lhs,
           map< Key, T, Compare, Alloc > &rhs ) {
    lhs.swap( rhs );
}

/* -------------------------------------------------------------------------- */

}