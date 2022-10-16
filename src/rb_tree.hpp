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
        value_type val;
        bool       is_red;
        Node *     parent;
        Node *     left;
        Node *     right;

        Node( const_reference val = value_type() ) : val( val ), is_red( false ), parent( 0 ), left( 0 ), right( 0 ) {}
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
        if ( root->is_red ) { os << "\033[1;31m"; }
        os << root->val << "\033[0m"
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
        if ( y->left != _nil ) { y->left->parent = x; }
        y->parent = x->parent;
        if ( !x->parent ) {
            _root = y;
        } else if ( x == x->parent->left ) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left   = x;
        x->parent = y;
    }

    void rotate_right( Node *x ) {
        Node *y  = x->left;
        x->left = y->right;
        if ( y->right != _nil ) { y->right->parent = x; }
        y->parent = x->parent;
        if ( !x->parent ) {
            _root = y;
        } else if ( x == x->parent->right ) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right   = x;
        x->parent = y;
    }

    void fix_insert( Node *z ) {
        while ( z != _root && z->parent->is_red ) {
            if ( z->parent == z->parent->parent->right ) {
                if ( z->parent->parent->left->is_red ) {
                    z->parent->parent->left->is_red = false;
                    z->parent->is_red               = false;
                    z->parent->parent->is_red       = true;
                    z                               = z->parent->parent;
                } else {
                    if ( z == z->parent->left ) {
                        z = z->parent;
                        rotate_right( z );
                    }
                    z->parent->is_red         = false;
                    z->parent->parent->is_red = true;
                    rotate_left( z->parent->parent );
                }
            } else {
                if ( z->parent->parent->right->is_red ) {
                    z->parent->parent->right->is_red = false;
                    z->parent->is_red                = false;
                    z->parent->parent->is_red        = true;
                    z                                = z->parent->parent;
                } else {
                    if ( z == z->parent->right ) {
                        z = z->parent;
                        rotate_left( z );
                    }
                    z->parent->is_red         = false;
                    z->parent->parent->is_red = true;
                    rotate_right( z->parent->parent );
                }
            }
        }
        _root->is_red = false;
    }

    void insert( value_type val ) {
        Node *new_node   = new Node( val );
        new_node->is_red = true;
        new_node->left   = _nil;
        new_node->right  = _nil;
        Node *current    = _root;
        Node *parent( 0 );
        while ( current != _nil ) {
            parent = current;
            if ( new_node->val < current->val ) {
                current = current->left;
            } else if ( new_node->val > current->val ) {
                current = current->right;
            } else {
                return;
            }
        }
        new_node->parent = parent;
        if ( !parent ) {
            _root = new_node;
        } else if ( new_node->val < parent->val ) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        fix_insert( new_node );
    }
};
}