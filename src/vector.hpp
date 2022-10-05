#pragma once

#include <iostream>
#include <iterator>
#include <memory>

namespace ft {

template <typename T, class Allocator = std::allocator<T>> class vector {

    /* ---------------------------- Iterator classes ---------------------------- */

    class Iterator {
    public:
        typedef const T                         value_type;
        typedef value_type &                    reference;
        typedef const value_type &              const_reference;
        typedef value_type *                    pointer;
        typedef typename std::ptrdiff_t         difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        Iterator( pointer p );
    };

    /* -------------------------------------------------------------------------- */

    class ConstIterator {
    public:
        typedef const T                         value_type;
        typedef value_type &                    reference;
        typedef const value_type &              const_reference;
        typedef value_type *                    pointer;
        typedef typename std::ptrdiff_t         difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        ConstIterator( pointer p );
    };

    /* -------------------------------------------------------------------------- */

    class ReverseIterator {
    public:
        typedef const T                         value_type;
        typedef value_type &                    reference;
        typedef const value_type &              const_reference;
        typedef value_type *                    pointer;
        typedef typename std::ptrdiff_t         difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        void yo();
    };

    /* -------------------------------------------------------------------------- */

    class ConstReverseIterator {
    public:
        typedef const T                         value_type;
        typedef value_type &                    reference;
        typedef const value_type &              const_reference;
        typedef value_type *                    pointer;
        typedef typename std::ptrdiff_t         difference_type;
        typedef std::random_access_iterator_tag iterator_category;

    private:
        pointer _p;

    public:
        void yo();
    };

    /* -------------------------------------------------------------------------- */

public:
    typedef T                                        value_type;
    typedef Allocator                                allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef Iterator                                 iterator;
    typedef ConstIterator                            const_iterator;
    typedef ReverseIterator                          reverse_iterator;
    typedef ConstReverseIterator                     const_reverse_iterator;
    typedef std::size_t                              size_type;

    /* ------------------------------- Attributes ------------------------------- */

protected:
    allocator_type _allocator;
    pointer        _data;
    size_type      _capacity;
    size_type      _size;

    /* ------------------------------ Construction ------------------------------ */

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
    virtual ~vector();
    vector &operator=( const vector &x );

    /* -------------------------------- Capacity -------------------------------- */

    size_type size() const;
    size_type max_size() const;
    void      resize( size_type n, value_type val = value_type() );
    size_type capacity() const;
    bool      empty() const;
    void      reserve( size_type n );
    void      shrink_to_fit();

    /* -------------------------------- Iterators ------------------------------- */

    iterator               begin();
    const_iterator         begin() const;
    iterator               end();
    const_iterator         end() const;
    reverse_iterator       rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator       rend();
    const_reverse_iterator rend() const;
    const_iterator         cbegin() const noexcept;
    const_iterator         cend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    /* ----------------------------- Element access ----------------------------- */

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

    /* -------------------------------- Modifiers ------------------------------- */

    template <class InputIterator>
    void     assign( InputIterator first, InputIterator last );
    void     assign( size_type n, const value_type &val );
    void     push_back( const value_type &val );
    void     pop_back();
    iterator insert( iterator position, const value_type &val );
    void     insert( iterator position, size_type n, const value_type &val );
    template <class InputIterator>
    void insert( iterator position, InputIterator first, InputIterator last );
    iterator erase( iterator position );
    iterator erase( iterator first, iterator last );
    void     swap( vector &x );
    void     clear();

    /* -------------------------------- Allocator ------------------------------- */

    allocator_type get_allocator() const;

    /* -------------------------------------------------------------------------- */
};
}        // namespace ft
