#pragma once

#include "algorithm.hpp"
#include "iterator.hpp"
#include "stack.hpp"
#include "utility.hpp"
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>

namespace ft {

template < typename K,
           typename V,
           typename Comp      = std::less< K >,
           typename Allocator = std::allocator< ft::pair< const K, V > > >
class _Rb_tree {

    /* ------------------------------ Member types ------------------------------ */

public:
    typedef K                                        key_type;
    typedef V                                        mapped_type;
    typedef ft::pair< const key_type, mapped_type >  value_type;
    typedef Comp                                     key_compare;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef std::ptrdiff_t                           difference_type;
    typedef std::size_t                              size_type;

    /* ---------------------------------- Node ---------------------------------- */

private:
    struct _Node {
    private:
        enum {
            RED = 1 << 0,
            NIL = 1 << 1,
        };

        u_int8_t _bits;

    public:
        typedef typename Allocator::template rebind< _Node >::other
                                                      node_allocator_type;
        typedef typename node_allocator_type::pointer node_pointer;

        pointer      data;
        node_pointer p;
        node_pointer left;
        node_pointer right;

    public:
        _Node() : _bits( 0 ), data( 0 ), p( 0 ), left( 0 ), right( 0 ) {}
        _Node( node_pointer left, node_pointer right )
            : _bits( 0 ),
              data( 0 ),
              p( 0 ),
              left( left ),
              right( right ) {}

        bool is_red() const { return _bits & RED; }
        bool is_nil() const { return _bits & NIL; }
        void set_red( bool is_red ) { _bits = ( _bits & NIL ) | is_red * RED; }

        static _Node create_nil() {
            _Node node;
            node._bits = NIL;
            return node;
        }

        node_pointer clone( node_allocator_type &allocator ) {
            node_pointer node = allocator.allocate( 1 );
            allocator.construct( node, *this );
            return node;
        }

        static void destroy( node_pointer         node,
                             node_allocator_type &allocator,
                             allocator_type      &data_allocator ) {
            if ( node->data ) {
                data_allocator.destroy( node->data );
                data_allocator.deallocate( node->data, 1 );
            }
            allocator.destroy( node );
            allocator.deallocate( node, 1 );
        }

        node_pointer min_child() {
            node_pointer node( this );
            while ( node->left && !node->left->is_nil() ) { node = node->left; }
            return node;
        }
        node_pointer max_child() {
            node_pointer node( this );
            while ( node->right && !node->right->is_nil() ) {
                node = node->right;
            }
            return node;
        }
    };

    typedef _Node                               node_type;
    typedef typename _Node::node_allocator_type node_allocator_type;
    typedef typename _Node::node_pointer        node_pointer;

    /* --------------------------------- Compare -------------------------------- */

    class extended_key_compare {
        node_pointer _end;
        node_pointer _rend;
        key_compare  _comp;

    public:
        extended_key_compare( node_pointer       end,
                              node_pointer       rend,
                              const key_compare &comp = key_compare() )
            : _end( end ),
              _rend( rend ),
              _comp( comp ) {}
        bool operator()( const key_type &a, const key_type &b ) const {
            return _comp( a, b );
        }
        bool operator()( const key_type &a, const node_pointer &b ) const {
            if ( b == _end ) { return true; }
            if ( b == _rend ) { return false; }
            return _comp( a, b->data->first );
        }
        bool operator()( const node_pointer &a, const key_type &b ) const {
            if ( a == _end ) { return false; }
            if ( a == _rend ) { return true; }
            return _comp( a->data->first, b );
        }
        const key_compare &key_comp() const { return _comp; }
    };

    /* -------------------------------- Iterator -------------------------------- */

    template < typename T > class Iterator {
    public:
        typedef T                               value_type;
        typedef T                              &reference;
        typedef T                              *pointer;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        node_pointer _node;

    public:
        Iterator() : _node( 0 ) {}
        Iterator( node_pointer node ) : _node( node ) {}

        const node_pointer &get_node() const { return _node; }

        Iterator &operator++() {
            if ( !_node->right->is_nil() ) {
                _node = _node->right;
                while ( !_node->left->is_nil() ) { _node = _node->left; }
            } else {
                node_pointer tmp = _node;
                while ( tmp->p && tmp == tmp->p->right ) { tmp = tmp->p; }
                if ( !tmp->p->is_nil() ) { _node = tmp->p; }
            }
            return *this;
        }

        Iterator operator++( int ) {
            Iterator tmp( *this );
            ++*this;
            return tmp;
        }

        Iterator &operator--() {
            if ( !_node->left->is_nil() ) {
                _node = _node->left;
                while ( !_node->right->is_nil() ) { _node = _node->right; }
            } else {
                node_pointer tmp = _node;
                while ( tmp->p && tmp == tmp->p->left ) { tmp = tmp->p; }
                _node = tmp->p;
            }
            return *this;
        }

        Iterator operator--( int ) {
            Iterator tmp( *this );
            --*this;
            return tmp;
        }

        template < typename U >
        bool operator==( const Iterator< U > &other ) const {
            return _node == other.get_node();
        }
        template < typename U >
        bool operator!=( const Iterator< U > &other ) const {
            return !( *this == other );
        }

        reference operator*() const { return *_node->data; }
        pointer   operator->() const { return _node->data; }

        operator Iterator< const T >() const {
            return Iterator< const T >( _node );
        }
    };

    /* --------------------------------- Members -------------------------------- */

    node_allocator_type  _node_allocator;
    allocator_type       _data_allocator;
    node_pointer         _nil;
    node_pointer         _end;
    node_pointer         _rend;
    node_pointer         _root;
    extended_key_compare _key_compare;
    size_type            _size;

    /* ------------------------------ Construction ------------------------------ */

public:
    _Rb_tree( const key_compare    &comp  = key_compare(),
              const allocator_type &alloc = allocator_type() )
        : _node_allocator( alloc ),
          _data_allocator( alloc ),
          _nil( node_type::create_nil().clone( _node_allocator ) ),
          _end( node_type( _nil, _nil ).clone( _node_allocator ) ),
          _rend( node_type( _nil, _nil ).clone( _node_allocator ) ),
          _root( _end ),
          _key_compare( extended_key_compare( _end, _rend, comp ) ),
          _size( 0 ) {
        _root->left = _rend;
        _rend->set_red( true );
        _rend->p = _root;
    }
    _Rb_tree( const _Rb_tree &other )
        : _node_allocator( other._node_allocator ),
          _data_allocator( other._data_allocator ),
          _nil( node_type::create_nil().clone( _node_allocator ) ),
          _end( node_type( _nil, _nil ).clone( _node_allocator ) ),
          _rend( node_type( _nil, _nil ).clone( _node_allocator ) ),
          _root( _end ),
          _key_compare( extended_key_compare( _end,
                                              _rend,
                                              other._key_compare.key_comp() ) ),
          _size( 0 ) {
        _root->left = _rend;
        _rend->set_red( true );
        _rend->p = _root;
        insert( other.begin(), other.end() );
    }
    _Rb_tree &operator=( const _Rb_tree &other ) {
        clear();
        insert( other.begin(), other.end() );
        return *this;
    }
    ~_Rb_tree() {
        clear();
        _Node::destroy( _nil, _node_allocator, _data_allocator );
        _Node::destroy( _end, _node_allocator, _data_allocator );
        _Node::destroy( _rend, _node_allocator, _data_allocator );
    }

    /* -------------------------------- Iterators ------------------------------- */

    typedef Iterator< value_type >                 iterator;
    typedef Iterator< const value_type >           const_iterator;
    typedef ft::reverse_iterator< iterator >       reverse_iterator;
    typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;

    iterator               begin() { return ++iterator( _rend ); }
    const_iterator         begin() const { return ++iterator( _rend ); }
    iterator               end() { return _end; }
    const_iterator         end() const { return _end; }
    reverse_iterator       rbegin() { return end(); }
    const_reverse_iterator rbegin() const { return end(); };
    reverse_iterator       rend() { return begin(); }
    const_reverse_iterator rend() const { return begin(); }

    /* -------------------------------- Capacity -------------------------------- */

    size_type size() const { return _size; }
    size_type max_size() const { return _node_allocator.max_size(); }

    /* -------------------------------- Modifiers ------------------------------- */

    ft::pair< iterator, bool > insert( const value_type &data ) {
        return _insert( data, _root );
    }
    ft::pair< iterator, bool > insert( iterator hint, const value_type &data ) {
        if ( _is_upper_bound( data.first, hint.get_node() ) ) {
            node_pointer node( hint.get_node() );
            return _insert( data, node );
        }
        return insert( data );
    }
    template < class InputIterator >
    void insert( InputIterator first, InputIterator last ) {
        for ( ; first != last; first++ ) { insert( *first ); }
    }

    size_type erase( const key_type &k ) { return _remove( _find_node( k ) ); }
    size_type erase( iterator it ) { return _remove( it.get_node() ); }
    void      erase( iterator first, iterator last ) {
        while ( first != last ) {
            iterator tmp = ++( iterator( first ) );
            erase( first );
            first = tmp;
        }
    }

    void swap( _Rb_tree &other ) {
        ft::swap( _nil, other._nil );
        ft::swap( _end, other._end );
        ft::swap( _rend, other._rend );
        ft::swap( _root, other._root );
        ft::swap( _key_compare, other._key_compare );
        ft::swap( _node_allocator, other._node_allocator );
        ft::swap( _size, other._size );
    }

    void clear() { erase( begin(), end() ); }

    /* ------------------------------- Operations ------------------------------- */

    iterator       find( const key_type &k ) { return _find_node( k ); }
    const_iterator find( const key_type &k ) const { return _find_node( k ); }

    iterator lower_bound( const key_type &k ) { return _lower_bound( k ); }
    const_iterator lower_bound( const key_type &k ) const {
        return _lower_bound( k );
    }
    iterator upper_bound( const key_type &k ) { return _upper_bound( k ); }
    const_iterator upper_bound( const key_type &k ) const {
        return _upper_bound( k );
    }

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() const { return _node_allocator; }

    /* -------------------------- Relational operators -------------------------- */

    bool operator==( const _Rb_tree &other ) const {
        return size() == other.size()
               && ft::equal( begin(), end(), other.begin() );
    }
    bool operator<( const _Rb_tree &other ) const {
        return ft::lexicographical_compare( begin(),
                                            end(),
                                            other.begin(),
                                            other.end() );
    }
    bool operator!=( const _Rb_tree &other ) const {
        return !( *this == other );
    }
    bool operator>( const _Rb_tree &other ) const { return other < *this; }
    bool operator<=( const _Rb_tree &other ) const {
        return !( *this > other );
    }
    bool operator>=( const _Rb_tree &other ) const {
        return !( *this < other );
    }

    /* --------------------------------- Helper --------------------------------- */

private:
    node_pointer _find_node( const key_type &k ) const {
        node_pointer current( _root );
        while ( !current->is_nil() ) {
            if ( _key_compare( k, current ) ) {
                current = current->left;
            } else if ( _key_compare( current, k ) ) {
                current = current->right;
            } else {
                break;
            }
        }
        return current == _nil ? _end : current;
    }

    node_pointer _lower_bound( const key_type &k ) const {
        node_pointer current( _root );
        while ( !_is_lower_bound( current, k ) ) {
            if ( _key_compare( k, current ) ) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }
    node_pointer _upper_bound( const key_type &k ) const {
        node_pointer current( _root );
        while ( !_is_upper_bound( current, k ) ) {
            if ( _key_compare( k, current ) ) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }

    bool _is_lower_bound( node_pointer current, const key_type &k ) const {
        node_pointer prev = ( --const_iterator( current ) ).get_node();
        return ( current == begin().get_node() || _key_compare( prev, k ) )
               && !_key_compare( current, k );
    }
    bool _is_upper_bound( node_pointer current, const key_type &k ) const {
        node_pointer prev = ( --const_iterator( current ) ).get_node();
        return ( current == begin().get_node() || !_key_compare( k, prev ) )
               && _key_compare( k, current );
    }

    bool _is_upper_bound( const key_type &k, node_pointer current ) const {
        if ( current == end().get_node() || _key_compare( k, current ) ) {
            return false;
        }
        return _key_compare( k, ( ++const_iterator( current ) ).get_node() );
    }

    void _transplant( node_pointer u, node_pointer v ) {
        if ( u == _root ) {
            _root = v;
        } else if ( u == u->p->left ) {
            u->p->left = v;
        } else {
            u->p->right = v;
        }
        v->p = u->p;
    }

    size_type _remove( node_pointer z ) {
        if ( z == _end ) { return 0; }
        node_pointer y( z );
        node_pointer x;
        bool         y_red_0( y->is_red() );
        if ( z->left == _nil ) {
            x = z->right;
            _transplant( z, x );
        } else if ( z->right == _nil ) {
            x = z->left;
            _transplant( z, x );
        } else {
            y       = z->right->min_child();
            y_red_0 = y->is_red();
            x       = y->right;
            if ( y->p == z ) {
                x->p = y;
            } else {
                _transplant( y, x );
                y->right    = z->right;
                y->right->p = y;
            }
            _transplant( z, y );
            y->left    = z->left;
            y->left->p = y;
            y->set_red( z->is_red() );
        }
        if ( !y_red_0 ) {
            while ( x != _root && !x->is_red() ) {
                if ( x == x->p->left ) {
                    node_pointer w( x->p->right );
                    if ( w->is_red() ) {
                        w->set_red( false );
                        x->p->set_red( true );
                        _rotate_left( x->p );
                        w = x->p->right;
                    }
                    if ( !w->left->is_red() && !w->right->is_red() ) {
                        w->set_red( true );
                        x = x->p;
                    } else {
                        if ( !w->right->is_red() ) {
                            w->left->set_red( false );
                            w->set_red( true );
                            _rotate_right( w );
                            w = x->p->right;
                        }
                        w->set_red( x->p->is_red() );
                        x->p->set_red( false );
                        w->right->set_red( false );
                        _rotate_left( x->p );
                        x = _root;
                    }
                } else {
                    node_pointer w( x->p->left );
                    if ( w->is_red() ) {
                        w->set_red( false );
                        x->p->set_red( true );
                        _rotate_right( x->p );
                        w = x->p->left;
                    }
                    if ( !w->right->is_red() && !w->left->is_red() ) {
                        w->set_red( true );
                        x = x->p;
                    } else {
                        if ( !w->left->is_red() ) {
                            w->right->set_red( false );
                            w->set_red( true );
                            _rotate_left( w );
                            w = x->p->left;
                        }
                        w->set_red( x->p->is_red() );
                        x->p->set_red( false );
                        w->left->set_red( false );
                        _rotate_right( x->p );
                        x = _root;
                    }
                }
            }
            x->set_red( false );
        }
        _Node::destroy( z, _node_allocator, _data_allocator );
        _size--;
        return 1;
    }

    void _rotate_left( node_pointer x ) {
        node_pointer y( x->right );
        x->right = y->left;
        if ( !y->left->is_nil() ) { y->left->p = x; }
        _transplant( x, y );
        y->left = x;
        x->p    = y;
    }

    void _rotate_right( node_pointer x ) {
        node_pointer y( x->left );
        x->left = y->right;
        if ( !y->right->is_nil() ) { y->right->p = x; }
        _transplant( x, y );
        y->right = x;
        x->p     = y;
    }

    ft::pair< node_pointer, bool > _insert( const value_type &data,
                                            node_pointer      ancestor ) {
        node_pointer p( 0 );
        while ( !ancestor->is_nil() ) {
            p = ancestor;
            if ( _key_compare( data.first, ancestor ) ) {
                ancestor = ancestor->left;
            } else if ( _key_compare( ancestor, data.first ) ) {
                ancestor = ancestor->right;
            } else {
                return ft::make_pair( ancestor, false );
            }
        }
        node_pointer node( _Node( _nil, _nil ).clone( _node_allocator ) );
        node->data = _data_allocator.allocate( 1 );
        _data_allocator.construct( node->data, data );
        node->set_red( true );
        node->p = p;
        if ( !p ) {
            _root = node;
        } else if ( _key_compare( data.first, p ) ) {
            p->left = node;
        } else {
            p->right = node;
        }
        node_pointer z( node );
        while ( z != _root && z->p->is_red() ) {
            if ( z->p == z->p->p->right ) {
                if ( z->p->p->left->is_red() ) {
                    z->p->p->left->set_red( false );
                    z->p->set_red( false );
                    z->p->p->set_red( true );
                    z = z->p->p;
                } else {
                    if ( z == z->p->left ) {
                        z = z->p;
                        _rotate_right( z );
                    }
                    z->p->set_red( false );
                    z->p->p->set_red( true );
                    _rotate_left( z->p->p );
                }
            } else {
                if ( z->p->p->right->is_red() ) {
                    z->p->p->right->set_red( false );
                    z->p->set_red( false );
                    z->p->p->set_red( true );
                    z = z->p->p;
                } else {
                    if ( z == z->p->right ) {
                        z = z->p;
                        _rotate_left( z );
                    }
                    z->p->set_red( false );
                    z->p->p->set_red( true );
                    _rotate_right( z->p->p );
                }
            }
        }
        _root->set_red( false );
        _size++;
        return ft::make_pair( node, true );
    }
};

/* ---------------------------------- Swap ---------------------------------- */

template < class Key, class T, class Compare, class Alloc >
void swap( _Rb_tree< Key, T, Compare, Alloc > &lhs,
           _Rb_tree< Key, T, Compare, Alloc > &rhs ) {
    lhs.swap( rhs );
}

/* -------------------------------------------------------------------------- */

}
