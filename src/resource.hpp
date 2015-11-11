#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

#include "store.hpp"

class Resource
{
  friend class Store;

  __attribute__((unused)) // Used by friend class Store
  unsigned long _ref_count = 0;
};

#define RESOURCE(T) \
private: \
  friend class Store; \
  T(Store &store, const std::string &name); \
  static const std::string filename(const std::string &name);

#endif // _RESOURCE_HPP_
