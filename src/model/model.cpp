#include "model.hpp"

GLuint Model::create_array_buffer(const GLenum type, const GLsizeiptr size, const GLvoid *const data)
{
  GLuint id;
  glGenBuffers(1, &id);
  glBindBuffer(type, id);
  glBufferData(type, size, data, GL_STATIC_DRAW);
  return id;
}
