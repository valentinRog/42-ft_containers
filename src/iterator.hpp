namespace ft {
/* ---------------------------- Reverse iterator ---------------------------- */

template < typename T > class reverse_iterator {
    typedef typename T::reference         reference;
    typedef typename T::pointer           pointer;
    typedef typename T::difference_type   difference_type;
    typedef typename T::iterator_category iterator_category;

private:
    T _it;

public:
    reverse_iterator( pointer p ) : _it( T( p ) ) {}
    reverse_iterator( const reverse_iterator &other ) : _it( other._it ) {}

    reverse_iterator operator+( difference_type n ) const { return _it - n; }
    reverse_iterator operator-( difference_type n ) const { return _it + n; }
    difference_type operator-( const reverse_iterator &other ) const { return _it + other; }

    reverse_iterator operator++() { return --_it; }
    reverse_iterator operator--() { return ++_it; }
    reverse_iterator operator++( int ) { return _it--; }
    reverse_iterator operator--( int ) { return _it++; }

    void operator+=( difference_type n ) { _it -= n; };
    void operator-=( difference_type n ) { _it += n; };

    reference operator*() { return *_it; }
    reference operator[]( difference_type i ) { return _it[-i]; }
};
}