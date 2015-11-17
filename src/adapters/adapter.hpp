#ifndef _ADAPTER_HPP_
#define _ADAPTER_HPP_

#include "../store.hpp"

struct Adapter
{
  Adapter(Store &store): _store(store) {};

  template <class T>
  const T *load(const std::string &name) const;

  template <class T>
  const std::string filename(const std::string &name) const;

private:
  Store &_store;
};

template <class T>
const T *Adapter::load(const std::string &name) const
{
  static_assert(std::is_convertible<T, Resource>::value
                  && !std::is_same<T, Resource>::value,
                "Adapter can deal with only Resource's children");

  return _store.load<T>(*this, name);
}

template <class T>
const std::string Adapter::filename(const std::string &name) const
{
  return T::filename(name);
}

#endif // _ADAPTER_HPP_
