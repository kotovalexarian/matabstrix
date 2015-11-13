#ifndef _GL_HPP_
#define _GL_HPP_

#include <GL/glew.h>
#include <GL/glfw.h>

enum Attrib : GLuint
{
  position,
  normal,
  tex_coord,

  __attrib_count
};

static const GLchar *const attribs[] = {
  "position",
  "normal",
  "tex_coord",
};

enum Uniform : unsigned
{
  mvp,
  local_modelview,

  __uniform_count
};

static const GLchar *const uniforms[] = {
  "mvp",
  "local_modelview",
};

#endif // _GL_HPP_
