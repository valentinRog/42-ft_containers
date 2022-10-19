#pragma once

#include "iterator.hpp"
#include <functional>
#include <iostream>
#include <utility>

namespace ft {

template < typename T,
           typename Compare   = std::less< T >,
           typename Allocator = std::allocator< T > >
class rb_tree {

    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;

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
    };

    typedef Node node_type;
    typedef
        typename Allocator::template rebind< Node >::other node_allocator_type;
    typedef typename node_allocator_type::pointer          node_pointer;

public:
    template < typename U > class Iterator {
    public:
        typedef U &                             reference;
        typedef U *                             pointer;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        node_pointer _node;
        bool         _overflow;

    public:
        Iterator() : _node( &_nil ), _overflow( true ) {}
        Iterator( node_pointer node ) : _node( node ) {
            _overflow = _node == &_nil;
        }
        Iterator( const Iterator &other )
            : _node( other._node ),
              _overflow( other._overflow ) {}

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

        template < typename V >
        bool operator==( const Iterator< V > &other ) const {
            return _overflow == other._overflow && _node == other._node;
        }

        reference operator*() { return _node->data; }

        operator Iterator< const U >() const {
            return Iterator< const U >( _node );
        }
    };

public:
    typedef Iterator< value_type >                 iterator;
    typedef Iterator< const value_type >           const_iterator;
    typedef ft::reverse_iterator< iterator >       reverse_iterator;
    typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;

private:
    node_pointer        _root;
    static node_type    _nil;
    Compare             _cmp;
    node_allocator_type _allocator;

public:
    rb_tree() : _cmp( Compare() ), _allocator( node_allocator_type() ) {
        _root = &_nil;
    }
    ~rb_tree() {
        while ( _root != &_nil ) { remove( _root->data ); }
    }

    iterator       begin() { return minimum( _root ); }
    const_iterator begin() const { return minimum( _root ); }
    iterator       end() { return ++iterator( maximum( _root ) ); }
    const_iterator end() const { return ++const_iterator( maximum( _root ) ); }
    reverse_iterator       rbegin() { return end(); }
    const_reverse_iterator rbegin() const { return end(); };
    reverse_iterator       rend() { return begin(); }
    const_reverse_iterator rend() const { return begin(); }
    const_iterator         cbegin() const { return end(); }
    const_iterator         cend() const { return begin(); }
    const_reverse_iterator crbegin() const { return end(); }
    const_reverse_iterator crend() const { return begin(); };

    void print( std::ostream &os, node_pointer root = 0, int space = 0 ) const {
        static const int count = 10;
        if ( root == &_nil ) {
            return;
        } else if ( !root ) {
            root = _root;
        }
        space += count;
        print( os, root->right, space );
        std::cout << std::endl;
        for ( int i = count; i < space; i++ ) { os << " "; };
        if ( root->red ) { os << "\033[1;31m"; }
        os << root->data << "\033[0m"
           << "\n";
        print( os, root->left, space );
    }

    friend std::ostream &operator<<( std::ostream &os, const rb_tree &tree ) {
        tree.print( os );
        return os;
    }

    void rotate_left( node_pointer x ) {
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

    void rotate_right( node_pointer x ) {
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

    void fix_insert( node_pointer z ) {
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
                        rotate_right( z );
                    }
                    z->p->red    = false;
                    z->p->p->red = true;
                    rotate_left( z->p->p );
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
                        rotate_left( z );
                    }
                    z->p->red    = false;
                    z->p->p->red = true;
                    rotate_right( z->p->p );
                }
            }
        }
        _root->red = false;
    }

    void insert( value_type data ) {
        node_pointer current = _root;
        node_pointer p( 0 );
        while ( current != &_nil ) {
            p = current;
            if ( _cmp( data, current->data ) < 0 ) {
                current = current->left;
            } else if ( _cmp( data, current->data ) > 0 ) {
                current = current->right;
            } else {
                current->data = data;
                return;
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
        fix_insert( new_node );
    }

    node_pointer search_node( const value_type &val ) {
        node_pointer current = _root;
        while ( current != &_nil && _cmp( current->data, val ) ) {
            if ( _cmp( val, current->data ) < 0 ) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }

    void transplant( node_pointer u, node_pointer v ) {
        if ( u == _root ) {
            _root = v;
        } else if ( u == u->p->left ) {
            u->p->left = v;
        } else {
            u->p->right = v;
        }
        v->p = u->p;
    }

    node_pointer minimum( node_pointer x ) {
        while ( x != &_nil && x->left != &_nil ) { x = x->left; }
        return x;
    }
    node_pointer maximum( node_pointer x ) {
        while ( x != &_nil && x->right != &_nil ) { x = x->right; }
        return x;
    }

    void remove_fixup( node_pointer x ) {
        while ( x != _root and x->red == false ) {
            if ( x == x->p->left ) {
                node_pointer w = x->p->right;
                if ( w->red ) {
                    w->red    = false;
                    x->p->red = true;
                    rotate_left( x->p );
                    w = x->p->right;
                }
                if ( w->left->red == false and w->right->red == false ) {
                    w->red = true;
                    x      = x->p;
                } else {
                    if ( w->right->red == false ) {
                        w->left->red = false;
                        w->red       = true;
                        rotate_right( w );
                        w = x->p->right;
                    }
                    w->red        = x->p->red;
                    x->p->red     = false;
                    w->right->red = false;
                    rotate_left( x->p );
                    x = _root;
                }
            } else {
                node_pointer w = x->p->left;
                if ( w->red ) {
                    w->red    = false;
                    x->p->red = true;
                    rotate_right( x->p );
                    w = x->p->left;
                }
                if ( w->right->red == false and w->left->red == false ) {
                    w->red = true;
                    x      = x->p;
                } else {
                    if ( w->left->red == false ) {
                        w->right->red = false;
                        w->red        = true;
                        rotate_left( w );
                        w = x->p->left;
                    }
                    w->red       = x->p->red;
                    x->p->red    = false;
                    w->left->red = false;
                    rotate_right( x->p );
                    x = _root;
                }
            }
        }
        x->red = false;
    }

    void remove( const value_type &val ) {
        node_pointer z = search_node( val );
        if ( z == &_nil ) { return; }
        node_pointer y = z;
        node_pointer x;
        bool         y_orig_color = y->red;
        if ( z->left == &_nil ) {
            x = z->right;
            transplant( z, x );
        } else if ( z->right == &_nil ) {
            x = z->left;
            transplant( z, x );
        } else {
            y            = minimum( z->right );
            y_orig_color = y->red;
            x            = y->right;
            if ( y->p == z ) {
                x->p = y;
            } else {
                transplant( y, y->right );
                y->right    = z->right;
                y->right->p = y;
            }
            transplant( z, y );
            y->left    = z->left;
            y->left->p = y;
            y->red     = z->red;
        }
        if ( !y_orig_color ) { remove_fixup( x ); }
        _allocator.destroy( z );
        _allocator.deallocate( z, 1 );
    };
};

template < class T, class Compare, class Allocator >
typename rb_tree< T, Compare, Allocator >::node_type
    rb_tree< T, Compare, Allocator >::_nil
    = rb_tree< T, Compare, Allocator >::node_type();

}