#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include "store.hpp"
#include "gl.hpp"

#include <string>

class Program:
  public Store::Resource
{
  friend class Store;

  Program(const std::string &name);

  static const std::string filename(const std::string &name);

public:
  void use() const;
  GLuint get_uniform_location(const GLchar *name) const;

private:
  GLuint _id;
};

#endif // _PROGRAM_HPP_
