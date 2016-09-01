// Copyright (c) 2016 Alex Howlett
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//! @file ultraii.h Simple resource handles for implementing clean RAII

#ifndef BITCOIN_UTILRAII_H
#define BITCOIN_UTILRAII_H

//! A deleter object that wraps an arbitrary resource cleanup function.
template <class T, class FREE> struct resource_deleter
{
  resource_deleter (FREE cleanup) : free (cleanup) {}
  void operator() (T *ptr) { if (ptr) free(ptr); }
  FREE free;
};

//! A smart pointer for wrapping RAII around legacy interfaces.

//! Uses a `unique_ptr` to store a resource and its deleter.
//! Can pass resource_ptr directly to functions that accept raw pointers.
//!
//! DO NOT pass `resource_ptr` directly to a legacy function that consumes
//! the resource.  If you do, the shared ownership will cause the resource to
//! be free'd twice.  Instead, `release` the `resource_ptr`.
//!
//! Example: `legacy_consume(res.release());`
template <
  class T,
  class FREE,
  class D = resource_deleter<T, FREE>,
  class PTR = std::unique_ptr<T, D>
>
class resource_ptr : public PTR
{
  using pointer = typename PTR::pointer;
public:
  resource_ptr(T * resource, FREE cleanup) :
    PTR (resource, D(cleanup)) {}

  operator pointer () { return PTR::get(); }
};

template <class T, class FREE>
resource_ptr<T,FREE> make_resource(T * resource, FREE cleanup)
{
  return resource_ptr<T,FREE>(resource, cleanup);
}

#endif // BITCOIN_UTILRAII.H
