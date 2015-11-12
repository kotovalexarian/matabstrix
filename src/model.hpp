#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "resource.hpp"
#include "textured_mesh.hpp"
#include "gl.hpp"
#include "material.hpp"

#include <string>

#include <glm/glm.hpp>

class Model:
  public Resource,
  public TexturedMesh
{
  RESOURCE(Model)

public:
  void draw() const;

private:
  GLuint positions_id;
  GLuint tex_coords_id;
  GLuint normals_id;

  GLuint id;

  const Material *_material;

  static GLuint create_array_buffer(GLenum type, GLsizeiptr size, const GLvoid *data);
};

#endif // _MODEL_HPP_
