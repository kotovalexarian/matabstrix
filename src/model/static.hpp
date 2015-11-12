#ifndef _MODEL_STATIC_HPP_
#define _MODEL_STATIC_HPP_

#include "model.hpp"
#include "../mesh/textured.hpp"
#include "../material.hpp"

namespace Models
{
  class Static:
    public Model,
    protected TexturedMesh
  {
    RESOURCE(Static)

  public:
    void draw() const;

  private:
    GLuint positions_id;
    GLuint tex_coords_id;
    GLuint normals_id;

    GLuint id;

    const Material *_material;
  };
};

#endif // _MODEL_STATIC_HPP_
