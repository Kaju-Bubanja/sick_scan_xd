#include "sick_scan/sick_scan_base.h" /* Base definitions included in all header files, added by add_sick_scan_base_header.py. Do not edit this line. */
#ifndef BOOST_SMART_PTR_SHARED_ARRAY_HPP_INCLUDED
#define BOOST_SMART_PTR_SHARED_ARRAY_HPP_INCLUDED

//
//  shared_array.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002, 2012 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/ for documentation.
//

#include <memory>             // TR1 cyclic inclusion fix

#include <cstddef>            // for std::ptrdiff_t
#include <algorithm>          // for std::swap
#include <functional>         // for std::less

#include <boost_wrap/checked_delete.hpp>
#include <boost_wrap/shared_count.hpp>

typedef decltype(nullptr) sp_nullptr_t;

template< class Y, class T > inline void sp_assert_convertible() noexcept
{
    T* p = static_cast< Y* >( 0 );
    (void)p;
}

namespace boost
{

//
//  shared_array
//
//  shared_array extends shared_ptr to arrays.
//  The array pointed to is deleted when the last shared_array pointing to it
//  is destroyed or reset.
//

template<class T> class shared_array
{
private:

    // Borland 5.5.1 specific workarounds
    typedef checked_array_deleter<T> deleter;
    typedef shared_array<T> this_type;

public:

    typedef T element_type;

    shared_array() noexcept : px( 0 ), pn()
    {
    }

#if !defined( BOOST_NO_CXX11_NULLPTR )

    shared_array( sp_nullptr_t ) noexcept : px( 0 ), pn()
    {
    }

#endif

    template<class Y>
    explicit shared_array( Y * p ): px( p ), pn( p, checked_array_deleter<Y>() )
    {
        sp_assert_convertible< Y[], T[] >();
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_array will release p by calling d(p)
    //

    template<class Y, class D> shared_array( Y * p, D d ): px( p ), pn( p, d )
    {
        sp_assert_convertible< Y[], T[] >();
    }

    // As above, but with allocator. A's copy constructor shall not throw.

    template<class Y, class D, class A> shared_array( Y * p, D d, A a ): px( p ), pn( p, d, a )
    {
        sp_assert_convertible< Y[], T[] >();
    }

//  generated copy constructor, destructor are fine...

#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

// ... except in C++0x, move disables the implicit copy

    shared_array( shared_array const & r ) noexcept : px( r.px ), pn( r.pn )
    {
    }

    shared_array( shared_array && r ) noexcept : px( r.px ), pn()
    {
        pn.swap( r.pn );
        r.px = 0;
    }

#endif

    // conversion

    template<class Y>
    shared_array( shared_array<Y> const & r )

    noexcept : px( r.px ), pn( r.pn )
    {
        sp_assert_convertible< Y[], T[] >();
    }

    // aliasing

    template< class Y >
    shared_array( shared_array<Y> const & r, element_type * p ) noexcept : px( p ), pn( r.pn )
    {
    }

    // assignment

    shared_array & operator=( shared_array const & r ) noexcept
    {
        this_type( r ).swap( *this );
        return *this;
    }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1400)

    template<class Y>
    shared_array & operator=( shared_array<Y> const & r ) noexcept
    {
        this_type( r ).swap( *this );
        return *this;
    }

#endif

#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

    shared_array & operator=( shared_array && r ) noexcept
    {
        this_type( static_cast< shared_array && >( r ) ).swap( *this );
        return *this;
    }

    template<class Y>
    shared_array & operator=( shared_array<Y> && r ) noexcept
    {
        this_type( static_cast< shared_array<Y> && >( r ) ).swap( *this );
        return *this;
    }

#endif

    void reset() noexcept
    {
        this_type().swap( *this );
    }

    template<class Y> void reset( Y * p ) // Y must be complete
    {
        assert( p == 0 || p != px ); // catch self-reset errors
        this_type( p ).swap( *this );
    }

    template<class Y, class D> void reset( Y * p, D d )
    {
        this_type( p, d ).swap( *this );
    }

    template<class Y, class D, class A> void reset( Y * p, D d, A a )
    {
        this_type( p, d, a ).swap( *this );
    }

    template<class Y> void reset( shared_array<Y> const & r, element_type * p ) noexcept
    {
        this_type( r, p ).swap( *this );
    }

    T & operator[] (std::ptrdiff_t i) const noexcept
    {
        assert(px != 0);
        assert(i >= 0);
        return px[i];
    }
    
    T * get() const noexcept
    {
        return px;
    }

// implicit conversion to "bool"
    explicit operator bool () const noexcept
    {
        return px != 0;
    }
    bool operator! () const noexcept
    {
        return px == 0;
    }

    bool unique() const noexcept
    {
        return pn.unique();
    }

    long use_count() const noexcept
    {
        return pn.use_count();
    }

    void swap(shared_array<T> & other) noexcept
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }

    void * _internal_get_deleter( std::type_info const & ti ) const noexcept
    {
        return pn.get_deleter( ti );
    }

private:

    template<class Y> friend class shared_array;

    T * px;                     // contained pointer
    detail::shared_count pn;    // reference counter

};  // shared_array

template<class T> inline bool operator==(shared_array<T> const & a, shared_array<T> const & b) noexcept
{
    return a.get() == b.get();
}

template<class T> inline bool operator!=(shared_array<T> const & a, shared_array<T> const & b) noexcept
{
    return a.get() != b.get();
}

#if !defined( BOOST_NO_CXX11_NULLPTR )

template<class T> inline bool operator==( shared_array<T> const & p, sp_nullptr_t ) noexcept
{
    return p.get() == 0;
}

template<class T> inline bool operator==( sp_nullptr_t, shared_array<T> const & p ) noexcept
{
    return p.get() == 0;
}

template<class T> inline bool operator!=( shared_array<T> const & p, sp_nullptr_t ) noexcept
{
    return p.get() != 0;
}

template<class T> inline bool operator!=( sp_nullptr_t, shared_array<T> const & p ) noexcept
{
    return p.get() != 0;
}

#endif

template<class T> inline bool operator<(shared_array<T> const & a, shared_array<T> const & b) noexcept
{
    return std::less<T*>()(a.get(), b.get());
}

template<class T> void swap(shared_array<T> & a, shared_array<T> & b) noexcept
{
    a.swap(b);
}

template< class D, class T > D * get_deleter( shared_array<T> const & p ) noexcept
{
    return static_cast< D * >( p._internal_get_deleter( typeid(D) ) );
}

} // namespace boost

#endif  // #ifndef BOOST_SMART_PTR_SHARED_ARRAY_HPP_INCLUDED
