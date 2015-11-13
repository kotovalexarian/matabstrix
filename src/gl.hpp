#ifndef _GL_HPP_
#define _GL_HPP_

#include <GL/glew.h>
#include <GL/glfw.h>

enum Attrib : GLuint
{
  position,
  normal,
  tex_coord,

  __count
};

static const GLchar *const attribs[] = {
  "position",
  "normal",
  "tex_coord",
};

extern GLuint mvp_uniform;
extern GLuint local_modelview_uniform;

#endif // _GL_HPP_
