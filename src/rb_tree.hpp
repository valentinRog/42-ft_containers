#pragma once

#include "iterator.hpp"
#include <functional>
#include <iostream>
#include <utility>

namespace ft {

template < typename T, typename Compare = std::less< T > > class rb_tree {

public:
    typedef T                 value_type;
    typedef value_type *      pointer;
    typedef const pointer     const_pointer;
    typedef value_type &      reference;
    typedef const value_type &const_reference;

    struct Node {
        value_type data;
        bool       red;
        Node *     p;
        Node *     left;
        Node *     right;

        Node( const_reference data = value_type() ) : data( data ), red( false ), p( 0 ), left( 0 ), right( 0 ) {}
    };

public:
    Node *  _root;
    Node *  _nil;
    Compare _cmp;

public:
    rb_tree() : _nil( new Node() ), _cmp( Compare() ) { _root = _nil; }

    void print( std::ostream &os, Node *root, int space = 0 ) {
        static const int count = 10;
        if ( root == _nil ) { return; }
        space += count;
        print( os, root->right, space );
        std::cout << std::endl;
        for ( int i = count; i < space; i++ ) { os << " "; };
        if ( root->red ) { os << "\033[1;31m"; }
        os << root->data << "\033[0m"
           << "\n";
        print( os, root->left, space );
    }

    friend std::ostream &operator<<( std::ostream &os, rb_tree tree ) {
        tree.print( os, tree._root );
        return os;
    }

    void rotate_left( Node *x ) {
        Node *y  = x->right;
        x->right = y->left;
        if ( y->left != _nil ) { y->left->p = x; }
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

    void rotate_right( Node *x ) {
        Node *y = x->left;
        x->left = y->right;
        if ( y->right != _nil ) { y->right->p = x; }
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

    void fix_insert( Node *z ) {
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
        Node *new_node  = new Node( data );
        new_node->red   = true;
        new_node->left  = _nil;
        new_node->right = _nil;
        Node *current   = _root;
        Node *p( 0 );
        while ( current != _nil ) {
            p = current;
            if ( new_node->data < current->data ) {
                current = current->left;
            } else if ( new_node->data > current->data ) {
                current = current->right;
            } else {
                return;
            }
        }
        new_node->p = p;
        if ( !p ) {
            _root = new_node;
        } else if ( new_node->data < p->data ) {
            p->left = new_node;
        } else {
            p->right = new_node;
        }
        fix_insert( new_node );
    }

    Node *search_node( const value_type &val ) {
        Node *current = _root;
        while ( current != _nil && current->data != val ) {
            if ( val < current->data ) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }

    void transplant( Node *u, Node *v ) {
        if ( u == _root ) {
            _root = v;
        } else if ( u == u->p->left ) {
            u->p->left = v;
        } else {
            u->p->right = v;
        }
        v->p = u->p;
    }

    Node *minimum( Node *x ) {
        while ( x->left != _nil ) { x = x->left; }
        return x;
    }

    void remove_fixup( Node *x ) {
        while ( x != _root and x->red == false ) {
            if ( x == x->p->left ) {
                Node *w = x->p->right;
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
                Node *w = x->p->left;
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
        Node *z = search_node( val );
        if ( z == _nil ) { return; }
        Node *y = z;
        Node *x;
        bool  y_orig_color = y->red;
        if ( z->left == _nil ) {
            x = z->right;
            transplant( z, x );
        } else if ( z->right == _nil ) {
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
    };
};
}