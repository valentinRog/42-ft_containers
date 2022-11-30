#pragma once

#include "_Rb_tree.hpp"
#include "algorithm.hpp"
#include "utility.hpp"

namespace ft {

template < typename T,
           typename Compare = std::less< T >,
           typename Alloc   = std::allocator< T > >
class set {

    /* ------------------------------ Member types ------------------------------ */

    typedef _Rb_tree<
        T,
        T,
        Compare,
        typename Alloc::template rebind< ft::pair< const T, T > >::other >
                                               tree_type;
    typedef typename tree_type::iterator       tree_iterator;
    typedef typename tree_type::const_iterator tree_const_iterator;

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
    public:
        typedef set::value_type                               value_type;
        typedef set::const_reference                          reference;
        typedef set::const_pointer                            pointer;
        typedef typename tree_const_iterator::difference_type difference_type;
        typedef
            typename tree_const_iterator::iterator_category iterator_category;

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

        reference operator*() const { return _it->second; }
        pointer   operator->() const { return &_it->second; }

        bool operator==( const Iterator &other ) const {
            return _it == other._it;
        }
        bool operator==( const tree_const_iterator &other ) const {
            return _it == other;
        }
        bool operator==( const tree_iterator &other ) const {
            return _it == other;
        }
        bool operator!=( const Iterator &other ) const {
            return _it != other._it;
        }
        bool operator!=( const tree_const_iterator &other ) const {
            return _it != other;
        }
        bool operator!=( const tree_iterator &other ) const {
            return _it != other;
        }

        operator tree_const_iterator() const { return _it; }
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

    explicit set( const key_compare &   comp  = key_compare(),
                  const allocator_type &alloc = allocator_type() )
        : _tree( comp, alloc ) {}

    template < class InputIterator >
    set( InputIterator         first,
         InputIterator         last,
         const key_compare &   comp  = key_compare(),
         const allocator_type &alloc = allocator_type() )
        : _tree( comp, alloc ) {
        insert( first, last );
    }

    set( const set &other ) : _tree( other._tree ) {}

    ~set() {}

    set &operator=( const set &other ) {
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

    /* -------------------------------- Capacity -------------------------------- */

    bool      empty() const { return !size(); }
    size_type size() const { return _tree.size(); }
    size_type max_size() const { return _tree.max_size(); }

    /* -------------------------------- Modifiers ------------------------------- */

    ft::pair< iterator, bool > insert( const value_type &val ) {
        return _tree.insert( ft::make_pair( val, val ) );
    }
    iterator insert( iterator position, const value_type &val ) {
        return _tree
            .insert( tree_const_iterator( position ).get_node(),
                     ft::make_pair( val, val ) )
            .first;
    }
    template < class InputIterator >
    void insert( InputIterator first, InputIterator last ) {
        for ( ; first != last; first++ ) { insert( *first ); }
    }

    void      erase( iterator position ) { _tree.erase( *position ); }
    size_type erase( const value_type &val ) { return _tree.erase( val ); }
    void      erase( iterator first, iterator last ) {
        while ( first != last ) {
            iterator next( first );
            next++;
            erase( first );
            first = next;
        }
    }

    void swap( set &other ) { ft::swap( _tree, other._tree ); }

    void clear() { _tree.clear(); }

    /* -------------------------------- Observers ------------------------------- */

    key_compare   key_comp() const { return key_compare(); }
    value_compare value_comp() const { return value_compare(); }

    /* ------------------------------- Operations ------------------------------- */

    iterator  find( const value_type &val ) const { return _tree.find( val ); }
    size_type count( const value_type &val ) const {
        return _tree.find( val ) != _tree.end();
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
};

/* ---------------------------------- Swap ---------------------------------- */

template < typename T, typename Compare, typename Alloc >
void swap( set< T, Compare, Alloc > &lhs, set< T, Compare, Alloc > &rhs ) {
    lhs.swap( rhs );
}

/* -------------------------------------------------------------------------- */

}