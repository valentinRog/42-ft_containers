#pragma once

#include "iterator.hpp"
#include "utility.hpp"
#include <cstddef>
#include <functional>
#include <iostream>

namespace ft {

template < typename K,
           typename V,
           typename Comp      = std::less< K >,
           typename Allocator = std::allocator< ft::pair< const K, V > > >
class rb_tree {

public:
    /* ------------------------------ Member types ------------------------------ */

    typedef K                                        key_type;
    typedef V                                        mapped_type;
    typedef ft::pair< const key_type, mapped_type >  value_type;
    typedef Comp                                     key_compare;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef std::size_t                              size_type;

    struct value_compare {
        bool operator()( const value_type &a, const value_type &b ) const {
            return key_compare()( a.first, b.first );
        }
    };

private:
    /* ---------------------------------- Node ---------------------------------- */

    struct Node {

        value_type data;
        bool       red;
        Node      *p;
        Node      *left;
        Node      *right;

        Node( const value_type &val = value_type() )
            : data( val ),
              red( false ),
              p( 0 ),
              left( 0 ),
              right( 0 ) {}
        Node( Node             *left,
              Node             *right,
              Node             *p   = 0,
              bool              red = false,
              const value_type &val = value_type() )
            : data( val ),
              red( false ),
              p( 0 ),
              left( left ),
              right( right ) {}
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

    struct extended_key_compare {
        Node       *_end;
        key_compare _comp;
        extended_key_compare( Node              *end  = 0,
                              const key_compare &comp = key_compare() )
            : _end( end ),
              _comp( comp ) {}
        bool operator()( const key_type &a, const key_type &b ) const {
            if ( &a == &_end->data.first ) { return 0; }
            if ( &b == &_end->data.first ) { return 1; }
            return _comp( a, b );
        }
        extended_key_compare &operator=( const extended_key_compare &other ) {
            _end  = other._end;
            _comp = other._comp;
            return *this;
        }
    };

    typedef Node node_type;
    typedef
        typename Allocator::template rebind< Node >::other node_allocator_type;
    typedef typename node_allocator_type::pointer          node_pointer;

    /* -------------------------------- Iterator -------------------------------- */

    template < typename T > class Iterator {
    public:
        typedef T                              &reference;
        typedef const T                        &const_reference;
        typedef T                              *pointer;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::bidirectional_iterator_tag iterator_category;

    private:
        node_pointer _node;

    public:
        Iterator() : _node( &_nil ) {}
        Iterator( node_pointer node ) : _node( node ) {}
        Iterator( const Iterator &other ) : _node( other._node ) {}
        Iterator &operator=( const Iterator &other ) {
            _node = other._node;
            return *this;
        }

        const node_pointer &get_node() const { return _node; }

        Iterator &operator++() {
            if ( _node->right != &_nil ) {
                _node = _node->right;
                while ( _node->left != &_nil ) { _node = _node->left; }
            } else {
                node_pointer tmp = _node;
                while ( tmp->p && tmp == tmp->p->right ) { tmp = tmp->p; }
                if ( tmp->p != &_nil ) { _node = tmp->p; }
            }
            return *this;
        }

        Iterator operator++( int ) {
            Iterator tmp( *this );
            ++( *this );
            return tmp;
        }

        Iterator &operator--() {
            if ( _node->left != &_nil ) {
                _node = _node->left;
                while ( _node->right != &_nil ) { _node = _node->right; }
            } else {
                node_pointer tmp = _node;
                while ( tmp->p && tmp == tmp->p->left ) { tmp = tmp->p; }
                _node = tmp->p;
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
            return _node == other.get_node();
        }
        template < typename U >
        bool operator!=( const Iterator< U > &other ) const {
            return !( *this == other );
        }

        reference       operator*() { return _node->data; }
        const_reference operator*() const { return _node->data; }
        pointer         operator->() const { return &_node->data; }

        operator Iterator< const T >() const {
            return Iterator< const T >( _node );
        }
    };

    /* --------------------------------- Members -------------------------------- */

    static node_type     _nil;
    node_allocator_type  _allocator;
    node_pointer         _end;
    node_pointer         _root;
    extended_key_compare _key_compare;
    size_type            _size;

public:
    /* ------------------------------ Construction ------------------------------ */

    rb_tree( const key_compare    &comp  = key_compare(),
             const allocator_type &alloc = node_allocator_type() )
        : _allocator( alloc ),
          _end( _node_dup( node_type( &_nil, &_nil ) ) ),
          _root( _end ),
          _key_compare( extended_key_compare( _end, comp ) ),
          _size( 0 ) {}
    rb_tree( const rb_tree &other )
        : _allocator( other._allocator ),
          _end( _node_dup( node_type( &_nil, &_nil ) ) ),
          _root( _end ),
          _key_compare( extended_key_compare( _end, other.key_comp() ) ),
          _size( 0 ) {
        insert( other.cbegin(), other.cend() );
    }
    rb_tree &operator=( const rb_tree &other ) {
        clear();
        insert( other.cbegin(), other.cend() );
        return *this;
    }
    ~rb_tree() { clear(); }

    /* -------------------------------- Iterators ------------------------------- */

    typedef Iterator< value_type >                 iterator;
    typedef Iterator< const value_type >           const_iterator;
    typedef ft::reverse_iterator< iterator >       reverse_iterator;
    typedef ft::reverse_iterator< const_iterator > const_reverse_iterator;
    typedef typename iterator::difference_type     difference_type;

    iterator         begin() { return _root->min_child(); }
    const_iterator   begin() const { return _root->min_child(); }
    iterator         end() { return _end; }
    const_iterator   end() const { return const_cast< node_pointer >( _end ); }
    reverse_iterator rbegin() { return end(); }
    const_reverse_iterator rbegin() const { return end(); };
    reverse_iterator       rend() { return begin(); }
    const_reverse_iterator rend() const { return begin(); }
    const_iterator         cbegin() const { return begin(); }
    const_iterator         cend() const { return end(); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); };

    /* -------------------------------- Capacity -------------------------------- */

    size_type size() const { return _size; }

    /* -------------------------------- Modifiers ------------------------------- */

    iterator insert( const value_type &data ) { return _insert( data, _root ); }
    iterator insert( iterator hint, const value_type &data ) {
        iterator right = ++iterator( hint );
        if ( hint.get_node() == &_nil || hint == end()
             || _key_compare( data.first, hint->first )
             || ( right != end()
                  && _key_compare( right->first, data.first ) ) ) {
            return insert( data );
        }
        return _insert( data, hint.get_node() );
    }
    template < class InputIterator >
    void insert( InputIterator first, InputIterator last ) {
        while ( first != last ) {
            if ( find( first->first ) == end() ) { insert( *first ); }
            first++;
        }
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

    void swap( rb_tree &other ) {
        ft::swap( _end, other._end );
        ft::swap( _root, other._root );
        ft::swap( _key_compare, other._key_compare );
        ft::swap( _allocator, other._allocator );
        ft::swap( _size, other._size );
    }

    void clear() { erase( begin(), end() ); }

    /* -------------------------------- Observers ------------------------------- */

    key_compare key_comp() const { return key_compare(); }

    /* ------------------------------- Operations ------------------------------- */

    iterator find( const key_type &k ) {
        node_pointer node = _find_node( k );
        return node != &_nil ? node : end();
    }
    const_iterator find( const key_type &k ) const {
        node_pointer node = _find_node( k );
        return node != &_nil ? node : end();
    }

    iterator lower_bound( const key_type &k ) {
        node_pointer current( _root );
        while ( current != &_nil ) {
            if ( _key_compare( current->data.first, k ) ) {
                current = current->right;
            } else if ( _key_compare( k, current->data.first ) ) {
                if ( current->left != &_nil
                     && _key_compare( current->left->data.first, k ) ) {
                    return current;
                }
                current = current->left;
            } else {
                return current;
            }
        }
        return end();
    }
    iterator upper_bound( const key_type &k ) {
        node_pointer current( _root );
        while ( current != &_nil ) {
            if ( _key_compare( current->data.first, k ) ) {
                current = current->right;
            } else if ( _key_compare( k, current->data.first ) ) {
                if ( current->left != &_nil
                     && _key_compare( current->left->data.first, k ) ) {
                    return current;
                }
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return end();
    }
    const_iterator lower_bound( const key_type &k ) const {
        node_pointer current( _root );
        while ( current != &_nil ) {
            if ( _key_compare( current->data.first, k ) ) {
                current = current->right;
            } else if ( _key_compare( k, current->data.first ) ) {
                if ( current->left != &_nil
                     && _key_compare( current->left->data.first, k ) ) {
                    return current;
                }
                current = current->left;
            } else {
                return current;
            }
        }
        return end();
    }
    const_iterator upper_bound( const key_type &k ) const {
        node_pointer current( _root );
        while ( current != &_nil ) {
            if ( _key_compare( current->data.first, k ) ) {
                current = current->right;
            } else if ( _key_compare( k, current->data.first ) ) {
                if ( current->left != &_nil
                     && _key_compare( current->left->data.first, k ) ) {
                    return current;
                }
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return end();
    }

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() { return _allocator; }

    /* --------------------------------- Helper --------------------------------- */

private:
    node_pointer _node_dup( const node_type &other ) {
        node_pointer new_node = _allocator.allocate( 1 );
        _allocator.construct( new_node, other );
        return new_node;
    }

    node_pointer _find_node( const key_type &k ) const {
        node_pointer current = _root;
        while ( current != &_nil
                && ( _key_compare( k, current->data.first )
                     || _key_compare( current->data.first, k ) ) ) {
            if ( _key_compare( k, current->data.first ) ) {
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

    size_type _remove( node_pointer z ) {
        if ( z == &_nil ) { return 0; }
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
        return 1;
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

    node_pointer _insert( const value_type &data, node_pointer ancestor ) {
        node_pointer p( 0 );
        int          count = 0;
        while ( ancestor != &_nil ) {
            count++;
            p = ancestor;
            if ( _key_compare( data.first, ancestor->data.first ) ) {
                ancestor = ancestor->left;
            } else if ( _key_compare( ancestor->data.first, data.first ) ) {
                ancestor = ancestor->right;
            } else {
                ancestor->data.second = data.second;
                return ancestor;
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
        } else if ( _key_compare( new_node->data.first, p->data.first ) ) {
            p->left = new_node;
        } else {
            p->right = new_node;
        }
        _insert_fixup( new_node );
        _size++;
        return new_node;
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

template < class K, class V, class Comp, class Allocator >
typename ft::rb_tree< K, V, Comp, Allocator >::node_type
    ft::rb_tree< K, V, Comp, Allocator >::_nil
    = rb_tree< K, V, Comp, Allocator >::node_type();

/* ---------------------------------- Swap ---------------------------------- */

template < class Key, class T, class Compare, class Alloc >
void swap( rb_tree< Key, T, Compare, Alloc > &lhs,
           rb_tree< Key, T, Compare, Alloc > &rhs ) {
    lhs.swap( rhs );
}

/* -------------------------------------------------------------------------- */

}