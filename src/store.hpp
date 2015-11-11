#ifndef _STORE_HPP_
#define _STORE_HPP_

#include <type_traits>
#include <string>
#include <map>

class Store
{
public:
  class Resource
  {
    friend class Store;

    __attribute__((unused)) // Used by friend class Store
    unsigned long _ref_count = 0;
  };

  template <class T>
  const T *load(const std::string &name);

private:
  std::map<std::string, Resource*> _resources;
};

template <class T>
const T *Store::load(const std::string &name)
{
  static_assert(std::is_convertible<T, Resource>::value
                  && !std::is_same<T, Resource>::value,
                "Store can load only Resource's children");

  if (_resources.find(name) == _resources.end())
    _resources[T::filename(name)] = new T(name);

  _resources[T::filename(name)]->_ref_count++;
  return (T*)_resources[T::filename(name)];
}

#endif // _STORE_HPP_
