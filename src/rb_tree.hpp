#pragma once

#include "iterator.hpp"
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

namespace ft {

template < typename K,
           typename V,
           typename Less      = std::less< K >,
           typename Allocator = std::allocator< std::pair< const K, V > > >
class rb_tree {

public:
    /* ------------------------------ Member types ------------------------------ */

    typedef K                                        key_type;
    typedef V                                        mapped_type;
    typedef std::pair< const key_type, mapped_type > value_type;
    typedef Less                                     key_compare;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef std::size_t                              size_type;

private:
    /* ---------------------------------- Node ---------------------------------- */

    struct Node {
        value_type data;
        bool       red;
        Node *     p;
        Node *     left;
        Node *     right;

        Node( const value_type &val = value_type() )
            : data( val ),
              red( false ),
              p( 0 ),
              left( 0 ),
              right( 0 ) {}
        Node( const Node &other )
            : data( other.data ),
              red( other.red ),
              p( other.p ),
              left( other.left ),
              right( other.right ) {}

        Node *min_child() {
            Node *tmp( this );
            while ( tmp->left && tmp->left != &_nil ) { tmp = tmp->left; }
            return tmp;
        }
        Node *max_child() {
            Node *tmp( this );
            while ( tmp->right && tmp->right != &_nil ) { tmp = tmp->right; }
            return tmp;
        }
    };

    typedef Node node_type;
    typedef
        typename Allocator::template rebind< Node >::other node_allocator_type;
    typedef typename node_allocator_type::pointer          node_pointer;

    /* -------------------------------- Iterator -------------------------------- */

    template < typename T > class Iterator {
    public:
        typedef T &                             reference;
        typedef T *                             pointer;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        node_pointer _node;
        bool         _overflow;

    public:
        Iterator() : _node( &_nil ), _overflow( true ) {}
        Iterator( node_pointer node, bool overflow = false )
            : _node( node ),
              _overflow( overflow ) {
            _overflow |= _node == &_nil;
        }
        Iterator( const Iterator &other )
            : _node( other._node ),
              _overflow( other._overflow ) {}
        Iterator &operator=( const Iterator &other ) {
            _node     = other._node;
            _overflow = other._overflow;
            return *this;
        }

        const node_pointer &get_node() const { return _node; }
        bool                overflow() const { return _overflow; }

        Iterator &operator++() {
            if ( _overflow ) {
                if ( _node != &_nil ) { _overflow = false; }
            } else {
                if ( _node->right != &_nil ) {
                    _node = _node->right;
                    while ( _node->left != &_nil ) { _node = _node->left; }
                } else {
                    node_pointer tmp = _node;
                    while ( tmp->p && tmp == tmp->p->right ) { tmp = tmp->p; }
                    if ( !tmp->p ) {
                        _overflow = true;
                    } else {
                        _node = tmp->p;
                    }
                }
            }
            return *this;
        }

        Iterator operator++( int ) {
            Iterator tmp( *this );
            ++( *this );
            return tmp;
        }

        Iterator &operator--() {
            if ( _overflow ) {
                if ( _node != &_nil ) { _overflow = false; }
            } else {
                if ( _node->left != &_nil ) {
                    _node = _node->left;
                    while ( _node->right != &_nil ) { _node = _node->right; }
                } else {
                    node_pointer tmp = _node;
                    while ( tmp->p && tmp == tmp->p->left ) { tmp = tmp->p; }
                    if ( !tmp->p ) {
                        _overflow = true;
                    } else {
                        _node = tmp->p;
                    }
                }
            }
            return *this;
        }

        Iterator operator--( int ) {
            Iterator tmp( *this );
            --( *this );
            return tmp;
        }

        template < typename U >
        bool operator==( const Iterator< U > &other ) const {
            return _node == other.get_node()
                   && overflow() == other.overflow();
        }
        template < typename U >
        bool operator!=( const Iterator< U > &other ) const {
            return !( *this == other );
        }

        reference operator*() { return _node->data; }
        pointer   operator->() const { return &_node->data; }

        operator Iterator< const T >() const {
            return Iterator< const T >( _node, _overflow );
        }
    };

    /* --------------------------------- Members -------------------------------- */

    node_pointer        _root;
    static node_type    _nil;
    size_type           _size;
    Less                _less;
    node_allocator_type _allocator;

public:
    /* ------------------------------ Construction ------------------------------ */

    rb_tree( const key_compare &   less  = key_compare(),
             const allocator_type &alloc = allocator_type() )
        : _size( 0 ),
          _less( less ),
          _allocator( alloc ) {
        _root = &_nil;
    }
    ~rb_tree() {
        while ( _root != &_nil ) { remove( _root->data.first ); }
    }

    /* -------------------------------- Iterators ------------------------------- */

    typedef Iterator< value_type >                 iterator;
    typedef Iterator< const value_type >           const_iterator;
    typedef ft::reverse_iterator< iterator >       reverse_iterator;
    typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;
    typedef typename iterator::difference_type     difference_type;

    iterator       begin() { return _root->min_child(); }
    const_iterator begin() const { return _root->min_child(); }
    iterator       end() { return ++iterator( _root->max_child() ); }
    const_iterator end() const {
        return ++const_iterator( _root->max_child() );
    }
    reverse_iterator       rbegin() { return end(); }
    const_reverse_iterator rbegin() const { return end(); };
    reverse_iterator       rend() { return begin(); }
    const_reverse_iterator rend() const { return begin(); }
    const_iterator         cbegin() const { return begin(); }
    const_iterator         cend() const { return end(); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); };

    iterator find( const key_type &k ) {
        node_pointer node = _find_node( k );
        return node != &_nil ? node : end();
    }
    const_iterator find( const key_type &k ) const {
        node_pointer node = _find_node( k );
        return node != &_nil ? node : end();
    }

    /* -------------------------------- Capacity -------------------------------- */

    size_type size() const { return _size; }

    /* -------------------------------- Modifiers ------------------------------- */

    iterator insert( const value_type &data ) {
        node_pointer current = _root;
        node_pointer p( 0 );
        while ( current != &_nil ) {
            p = current;
            if ( _less( data.first, current->data.first ) ) {
                current = current->left;
            } else if ( _less( current->data.first, data.first ) ) {
                current = current->right;
            } else {
                current->data.second = data.second;
                return current;
            }
        }
        node_pointer new_node = _allocator.allocate( 1 );
        _allocator.construct( new_node, data );
        new_node->red   = true;
        new_node->left  = &_nil;
        new_node->right = &_nil;
        new_node->p     = p;
        if ( !p ) {
            _root = new_node;
        } else if ( new_node->data < p->data ) {
            p->left = new_node;
        } else {
            p->right = new_node;
        }
        _insert_fixup( new_node );
        _size++;
        return new_node;
    }

    iterator insert( iterator position, const value_type &data ) {
        std::cout << is_bounded( position, data.first ) << std::endl;
        return insert( data );
    }

    void remove( const key_type &k ) {
        node_pointer z = _find_node( k );
        if ( z == &_nil ) { return; }
        node_pointer y = z;
        node_pointer x;
        bool         y_orig_color = y->red;
        if ( z->left == &_nil ) {
            x = z->right;
            _transplant( z, x );
        } else if ( z->right == &_nil ) {
            x = z->left;
            _transplant( z, x );
        } else {
            y            = z->right->min_child();
            y_orig_color = y->red;
            x            = y->right;
            if ( y->p == z ) {
                x->p = y;
            } else {
                _transplant( y, y->right );
                y->right    = z->right;
                y->right->p = y;
            }
            _transplant( z, y );
            y->left    = z->left;
            y->left->p = y;
            y->red     = z->red;
        }
        if ( !y_orig_color ) { _remove_fixup( x ); }
        _allocator.destroy( z );
        _allocator.deallocate( z, 1 );
        _size--;
    }

    /* ------------------------------- Operations ------------------------------- */

    bool is_bounded( iterator position, const key_type &k ) {
        iterator right = ++iterator( position );
        return ( position == begin() || _less( position->first, k ) )
               && ( position == end() || _less( k, right->first ) );
    }

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() { return _allocator; }

    /* --------------------------------- Helper --------------------------------- */

private:
    node_pointer _find_node( const key_type &k ) {
        node_pointer current = _root;
        while ( current != &_nil
                && ( _less( k, current->data.first )
                     || _less( current->data.first, k ) ) ) {
            if ( _less( k, current->data.first ) ) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
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

    void _remove_fixup( node_pointer x ) {
        while ( x != _root and x->red == false ) {
            if ( x == x->p->left ) {
                node_pointer w = x->p->right;
                if ( w->red ) {
                    w->red    = false;
                    x->p->red = true;
                    _rotate_left( x->p );
                    w = x->p->right;
                }
                if ( w->left->red == false and w->right->red == false ) {
                    w->red = true;
                    x      = x->p;
                } else {
                    if ( w->right->red == false ) {
                        w->left->red = false;
                        w->red       = true;
                        _rotate_right( w );
                        w = x->p->right;
                    }
                    w->red        = x->p->red;
                    x->p->red     = false;
                    w->right->red = false;
                    _rotate_left( x->p );
                    x = _root;
                }
            } else {
                node_pointer w = x->p->left;
                if ( w->red ) {
                    w->red    = false;
                    x->p->red = true;
                    _rotate_right( x->p );
                    w = x->p->left;
                }
                if ( w->right->red == false and w->left->red == false ) {
                    w->red = true;
                    x      = x->p;
                } else {
                    if ( w->left->red == false ) {
                        w->right->red = false;
                        w->red        = true;
                        _rotate_left( w );
                        w = x->p->left;
                    }
                    w->red       = x->p->red;
                    x->p->red    = false;
                    w->left->red = false;
                    _rotate_right( x->p );
                    x = _root;
                }
            }
        }
        x->red = false;
    }
    void _rotate_left( node_pointer x ) {
        node_pointer y = x->right;
        x->right       = y->left;
        if ( y->left != &_nil ) { y->left->p = x; }
        y->p = x->p;
        if ( !x->p ) {
            _root = y;
        } else if ( x == x->p->left ) {
            x->p->left = y;
        } else {
            x->p->right = y;
        }
        y->left = x;
        x->p    = y;
    }

    void _rotate_right( node_pointer x ) {
        node_pointer y = x->left;
        x->left        = y->right;
        if ( y->right != &_nil ) { y->right->p = x; }
        y->p = x->p;
        if ( !x->p ) {
            _root = y;
        } else if ( x == x->p->right ) {
            x->p->right = y;
        } else {
            x->p->left = y;
        }
        y->right = x;
        x->p     = y;
    }

    void _insert_fixup( node_pointer z ) {
        while ( z != _root && z->p->red ) {
            if ( z->p == z->p->p->right ) {
                if ( z->p->p->left->red ) {
                    z->p->p->left->red = false;
                    z->p->red          = false;
                    z->p->p->red       = true;
                    z                  = z->p->p;
                } else {
                    if ( z == z->p->left ) {
                        z = z->p;
                        _rotate_right( z );
                    }
                    z->p->red    = false;
                    z->p->p->red = true;
                    _rotate_left( z->p->p );
                }
            } else {
                if ( z->p->p->right->red ) {
                    z->p->p->right->red = false;
                    z->p->red           = false;
                    z->p->p->red        = true;
                    z                   = z->p->p;
                } else {
                    if ( z == z->p->right ) {
                        z = z->p;
                        _rotate_left( z );
                    }
                    z->p->red    = false;
                    z->p->p->red = true;
                    _rotate_right( z->p->p );
                }
            }
        }
        _root->red = false;
    }
};

/* --------------------------- nil initialization --------------------------- */

template < class K, class V, class Less, class Allocator >
typename ft::rb_tree< K, V, Less, Allocator >::node_type
    ft::rb_tree< K, V, Less, Allocator >::_nil
    = rb_tree< K, V, Less, Allocator >::node_type();

/* -------------------------------------------------------------------------- */

}