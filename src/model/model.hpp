#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "../resource.hpp"
#include "../gl.hpp"

#include <glm/glm.hpp>

struct Scene;

struct Model:
  Resource
{
  virtual void draw(const Scene &scene, const glm::mat4 &mvp, const glm::mat4 &transformation) const = 0;

protected:
  static GLuint create_array_buffer(GLenum type, GLsizeiptr size, const GLvoid *data);
};

#endif // _MODEL_HPP_
