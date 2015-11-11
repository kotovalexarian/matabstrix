#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "store.hpp"
#include "gl.hpp"

#include <string>

class Texture:
  public Store::Resource
{
  friend class Store;

  Texture(const std::string &name);

  static const std::string filename(const std::string &name);

public:

  void use() const;

private:
  GLuint _id;
};

#endif // _TEXTURE_HPP_
