#pragma once

#include <iostream>
#include <memory>

namespace ft {

template <typename T, class Allocator = std::allocator<T>> class vector {

public:
    template <typename T2> class Iterator {
    public:
        void yo();
    };

    template <typename T2> class ReverseIterator {
    public:
        void yo();
    };

public:
    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef Iterator<T>                              iterator;
    typedef Iterator<const T>                        const_iterator;
    typedef ReverseIterator<T>                       reverse_iterator;
    typedef ReverseIterator<const T>                 const_reverse_iterator;
    typedef std::size_t                              size_type;

protected:
    allocator_type _allocator;
    pointer        _data;
    size_type      _capacity;
    size_type      _size;

public:
    explicit vector( const allocator_type &alloc = allocator_type() );
    explicit vector( size_type             n,
                     const value_type &    val   = value_type(),
                     const allocator_type &alloc = allocator_type() );
    template <class InputIterator>
    vector( InputIterator         first,
            InputIterator         last,
            const allocator_type &alloc = allocator_type() );
    vector( const vector &x );

    ~vector();

    vector &operator=( const vector &x );

    size_type size() const;
    size_type max_size() const;
    void      resize( size_type n, value_type val = value_type() );
    size_type capacity() const;
    bool      empty() const;
    void      reserve( size_type n );
    void      shrink_to_fit();

    reference         operator[]( size_type n );
    const_reference   operator[]( size_type n ) const;
    reference         at( size_type n );
    const_reference   at( size_type n ) const;
    reference         front();
    const_reference   front() const;
    reference         back();
    const_reference   back() const;
    value_type *      data() noexcept;
    const value_type *data() const noexcept;

    template <class InputIterator>
    void     assign( InputIterator first, InputIterator last );
    void     assign( size_type n, const value_type &val );
    void     push_back( const value_type &val );
    void     pop_back();
    Iterator insert( Iterator position, const value_type &val );
    void     insert( Iterator position, size_type n, const value_type &val );
    template <class InputIterator>
    void insert( Iterator position, InputIterator first, InputIterator last
    ); Iterator erase( Iterator position ); Iterator erase( Iterator first,
    Iterator last ); void     swap( vector &x ); void     clear();

    allocator_type get_allocator() const;
};

}        // namespace ft

#include "vector.tpp"