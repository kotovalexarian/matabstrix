#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

#include "adapters/adapter.hpp"

struct Resource
{
private:
  friend struct Store;

  __attribute__((unused)) // Used by friend class Store
  unsigned long _ref_count = 0;
};

#define RESOURCE(T) \
  friend struct ::Store; \
  T(const Adapter &adapter, const std::string &name); \
  static const std::string filename(const std::string &name);

#endif // _RESOURCE_HPP_
