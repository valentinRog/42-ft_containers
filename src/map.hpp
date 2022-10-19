#pragma once

#include "rb_tree.hpp"

namespace ft {
template < typename Key,
           typename T,
           typename Less  = std::less< Key >,
           typename Alloc = std::allocator< std::pair< const Key, T > > >
class map {

public:
    typedef Key                                      key_type;
    typedef T                                        mapped_type;
    typedef std::pair< const key_type, mapped_type > value_type;
    typedef Less                                     key_compare;
    //value compare ?
    typedef Alloc                                    allocator_type;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef std::size_t                              size_type;
};
}