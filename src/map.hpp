#pragma once

#include "rb_tree.hpp"

namespace ft {
template < typename Key,
           typename T,
           typename Compare = std::less< Key >,
           typename Alloc   = std::allocator< std::pair< const Key, T > > >
class map {

public:
    typedef Key                                      key_type;
    typedef T                                        mapped_type;
    typedef std::pair< const key_type, mapped_type > value_type;
    typedef Compare                                  key_compare;

    struct value_compare {
        bool operator()( const value_type &a, const value_type &b ) const {
            return key_compare()( a.first, b.first );
        }
    };

    typedef Alloc                                    allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;

private:
    typedef typename ft::rb_tree< std::pair< key_type, mapped_type >,
                                  value_compare,
                                  allocator_type >
        tree_type;

public:
    typedef typename tree_type::iterator               iterator;
    typedef typename tree_type::const_iterator         const_iterator;
    typedef typename tree_type::reverse_iterator       reverse_iterator;
    typedef typename tree_type::const_reverse_iterator const_reverse_iterator;
    typedef typename tree_type::difference_type        difference_type;
    typedef typename tree_type::size_type              size_type;

private:
    tree_type _tree;

public:
    /* ------------------------------ Construction ------------------------------ */

    explicit map( const key_compare &   comp  = key_compare(),
                  const allocator_type &alloc = allocator_type() )
        : _tree( tree_type() ) {}
    template < class InputIterator >
    map( InputIterator         first,
         InputIterator         last,
         const key_compare &   comp  = key_compare(),
         const allocator_type &alloc = allocator_type() );
    map( const map &x );
    ~map() {}
    map &operator=( const map &x );

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
    bool      empty() const { return !_tree.size(); }
    size_type size() const { return _tree.size(); }
    size_type max_size() const;

    /* ----------------------------- Element access ----------------------------- */
    mapped_type &operator[]( const key_type &k ) {
        value_type x = value_type( k, mapped_type() );
        _tree.insert( x );
        return _tree.find( x )->second;
    }
    mapped_type &at( key_type &k ) {
    }
    const mapped_type &at( const key_type &k ) const {}

    /* -------------------------------- Modifiers ------------------------------- */

    std::pair< iterator, bool > insert( const value_type &val ) {
        if (_tree.find(val) == _tree.end()) {
            _tree.insert(val);
            return std::make_pair(_tree.find(val), true);
        }
    }
    iterator insert( iterator position, const value_type &val );
    template < class InputIterator >
    void insert( InputIterator first, InputIterator last );

    /* -------------------------------------------------------------------------- */
};
}