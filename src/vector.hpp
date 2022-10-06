#pragma once

#include <iostream>
#include <iterator>
#include <memory>

namespace ft {

template <typename T, class Allocator = std::allocator<T> > class vector {

    /* -------------------------------- Iterator -------------------------------- */

public:
    template <typename value_type> class Iterator {
    public:
        typedef value_type &                    reference;
        typedef value_type *                    pointer;
        typedef typename std::ptrdiff_t         difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        Iterator( pointer p ) : _p( p ) {}
        Iterator( const Iterator &other ) : _p( other._p ) {}

        Iterator operator+( difference_type n ) const { return Iterator( _p + n ); }
        Iterator operator-( difference_type n ) const { return Iterator( _p - n ); }
        Iterator operator+( const Iterator &other ) const { return Iterator( _p + other._p ); }
        Iterator operator-( const Iterator &other ) const { return Iterator( _p - other._p ); }

        Iterator operator++() {
            _p++;
            return *this;
        }
        Iterator operator--() {
            _p--;
            return *this;
        }
        Iterator operator++( int ) {
            Iterator copy( *this );
            _p++;
            return copy;
        }
        Iterator operator--( int ) {
            Iterator copy( *this );
            _p--;
            return copy;
        }

        void operator+=( difference_type n ) { _p += n; };
        void operator-=( difference_type n ) { _p -= n; };

        bool operator==( const Iterator &other ) const { return ( _p == other._p ); };
        bool operator!=( const Iterator &other ) const { return ( _p != other._p ); };
        bool operator>( const Iterator &other ) const { return ( _p > other._p ); };
        bool operator<( const Iterator &other ) const { return ( _p < other._p ); };
        bool operator>=( const Iterator &other ) const { return ( _p >= other._p ); };
        bool operator<=( const Iterator &other ) const { return ( _p <= other._p ); };

        reference operator*() { return *_p; }
        reference operator[]( difference_type i ) { return _p[i]; }
        pointer   operator->() const { return ( _p ); };
    };

    /* ---------------------------- Reverse iterator ---------------------------- */

    template <typename value_type> class ReverseIterator {
    public:
        typedef value_type &                    reference;
        typedef value_type *                    pointer;
        typedef typename std::ptrdiff_t         difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        ReverseIterator( pointer p ) : _p( p ) {}
        ReverseIterator( const ReverseIterator &other ) : _p( other._p ) {}

        ReverseIterator operator+( difference_type n ) const { return ReverseIterator( _p - n ); }
        ReverseIterator operator-( difference_type n ) const { return ReverseIterator( _p + n ); }
        ReverseIterator operator+( const ReverseIterator &other ) const { return ReverseIterator( _p - other._p ); }
        ReverseIterator operator-( const ReverseIterator &other ) const { return ReverseIterator( _p + other._p ); }

        ReverseIterator operator++() {
            _p--;
            return *this;
        }
        ReverseIterator operator--() {
            _p++;
            return *this;
        }
        ReverseIterator operator++( int ) {
            ReverseIterator copy( *this );
            _p--;
            return copy;
        }
        ReverseIterator operator--( int ) {
            ReverseIterator copy( *this );
            _p++;
            return copy;
        }

        void operator+=( difference_type n ) { _p -= n; };
        void operator-=( difference_type n ) { _p += n; };

        bool operator==( const ReverseIterator &other ) const { return ( _p == other._p ); };
        bool operator!=( const ReverseIterator &other ) const { return ( _p != other._p ); };
        bool operator>( const ReverseIterator &other ) const { return ( _p > other._p ); };
        bool operator<( const ReverseIterator &other ) const { return ( _p < other._p ); };
        bool operator>=( const ReverseIterator &other ) const { return ( _p >= other._p ); };
        bool operator<=( const ReverseIterator &other ) const { return ( _p <= other._p ); };

        reference operator*() { return *_p; }
        reference operator[]( difference_type i ) { return _p[-i]; }
        pointer   operator->() const { return ( _p ); };
    };

    /* ------------------------------ Member types ------------------------------ */

public:
    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef Iterator<value_type>                     iterator;
    typedef Iterator<const value_type>               const_iterator;
    typedef ReverseIterator<value_type>              reverse_iterator;
    typedef ReverseIterator<const value_type>        const_reverse_iterator;
    typedef std::size_t                              size_type;

    /* ------------------------------- Attributes ------------------------------- */

protected:
    allocator_type _allocator;
    pointer        _data;
    size_type      _capacity;
    size_type      _size;

    /* ------------------------------ Construction ------------------------------ */

public:
    explicit vector( const allocator_type &alloc = allocator_type() )
        : _allocator( alloc ),
          _data( 0 ),
          _capacity( 0 ),
          _size( 0 ) {
        _data    = new T[10];
        _data[1] = 8;
    }

    explicit vector( size_type             n,
                     const value_type &    val   = value_type(),
                     const allocator_type &alloc = allocator_type() );
    template <class InputIterator>
    vector( InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type() );
    vector( const vector &x );
    virtual ~vector() {}

    vector &operator=( const vector &other );

    /* -------------------------------- Capacity -------------------------------- */

    size_type size() const { return _size; }
    size_type max_size() const;
    void      resize( size_type n, value_type val = value_type() );
    size_type capacity() const { return _capacity; }
    bool      empty() const { return !_size; }
    void      reserve( size_type n );
    void      shrink_to_fit();

    /* -------------------------------- Iterators ------------------------------- */

    iterator               begin() { return _data; }
    const_iterator         begin() const { return _data; }
    iterator               end();
    const_iterator         end() const;
    reverse_iterator       rbegin();
    const_reverse_iterator rbegin() const {return _data;};
    reverse_iterator       rend();
    const_reverse_iterator rend() const;
    const_iterator         cbegin() const ;
    const_iterator         cend() const ;
    const_reverse_iterator crbegin() const ;
    const_reverse_iterator crend() const ;

    /* ----------------------------- Element access ----------------------------- */

    reference         operator[]( size_type i );
    const_reference   operator[]( size_type i ) const;
    reference         at( size_type i );
    const_reference   at( size_type i ) const;
    reference         front();
    const_reference   front() const;
    reference         back();
    const_reference   back() const;
    value_type *      data() ;
    const value_type *data() const ;

    /* -------------------------------- Modifiers ------------------------------- */

    template <class InputIterator> void assign( InputIterator first, InputIterator last );
    void                                assign( size_type n, const value_type &val );
    void                                push_back( const value_type &val );
    void                                pop_back();
    iterator                            insert( iterator position, const value_type &val );
    void                                insert( iterator position, size_type n, const value_type &val );
    template <class InputIterator> void insert( iterator position, InputIterator first, InputIterator last );
    iterator                            erase( iterator position );
    iterator                            erase( iterator first, iterator last );
    void                                swap( vector &x );
    void                                clear();

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() const { return _allocator; }

    /* -------------------------------------------------------------------------- */
};
}
