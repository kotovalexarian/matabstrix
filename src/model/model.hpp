#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "../resource.hpp"
#include "../gl.hpp"

#include <glm/glm.hpp>

class Model:
  public Resource
{
public:
  virtual void draw() const = 0;

protected:
  static GLuint create_array_buffer(GLenum type, GLsizeiptr size, const GLvoid *data);
};

#endif // _MODEL_HPP_
