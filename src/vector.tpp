#include "vector.hpp"

#include <iostream>

/* -------------------------------------------------------------------------- */

template <class T, class Allocator>
template <class T2>
void ft::vector<T, Allocator>::Iterator<T2>::yo() {
    std::cout << "yo" << std::endl;
}

/* -------------------------------------------------------------------------- */

template <typename T, class Allocator>
ft::vector<T, Allocator>::vector( const allocator_type &alloc )
    : _allocator( alloc ),
      _data( 0 ),
      _capacity( 0 ),
      _size( 0 ) {
    std::cout << "constructor" << std::endl;
}

template <typename T, class Allocator> ft::vector<T, Allocator>::~vector() {
    std::cout << "destructor" << std::endl;
}

/* -------------------------------------------------------------------------- */
