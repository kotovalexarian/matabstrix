#ifndef _MTLLIB_HPP_
#define _MTLLIB_HPP_

#include "store.hpp"
#include "material.hpp"

#include <string>
#include <map>

class Mtllib:
  public Store::Resource
{
  friend class Store;

  Mtllib(const std::string &name);

  static const std::string filename(const std::string &name);

public:
  std::map<std::string, const Material*> materials;
};

#endif // _MTLLIB_HPP_
