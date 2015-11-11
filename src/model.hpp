#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "store.hpp"
#include "gl.hpp"
#include "material.hpp"

#include <string>
#include <vector>

#include <glm/glm.hpp>

class Model:
  public Store::Resource
{
  friend class Store;

  Model(const std::string &name);

  static const std::string filename(const std::string &name);

public:
  void draw() const;

private:
  std::vector<glm::vec3> positions;
  GLuint positions_id;

  std::vector<glm::vec2> tex_coords;
  GLuint tex_coords_id;

  std::vector<glm::vec3> normals;
  GLuint normals_id;

  std::vector<GLushort> elements;
  GLuint id;

  const Material *_material;

  static GLuint create_array_buffer(GLenum type, GLsizeiptr size, const GLvoid *data);
};

#endif // _MODEL_HPP_
