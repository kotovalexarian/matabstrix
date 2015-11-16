#ifndef _MODEL_STATIC_HPP_
#define _MODEL_STATIC_HPP_

#include "model.hpp"
#include "../meshes/textured.hpp"
#include "../material.hpp"
#include "../executable.hpp"

namespace Models
{
  struct Static:
    Model,
    protected TexturedMesh
  {
    RESOURCE(Static)

  public:
    void draw(const Scene &scene, const glm::mat4 &mvp, const glm::mat4 &transformation) const;

  private:
    GLuint positions_id;
    GLuint normals_id;
    GLuint tex_coords_id;

    GLuint id;

    const Material *_material;

    const Executable *exe;

    enum Attrib : GLuint
    {
      position,
      normal,
      tex_coord,

      __attrib_count
    };

    enum Uniform : unsigned
    {
      mvp,
      local_modelview,
      sun_direction,

      __uniform_count
    };

    static const GLchar *const attribs[];
    static const GLchar *const uniforms[];
  };
};

#endif // _MODEL_STATIC_HPP_
