#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "../transformation.hpp"

#include <glm/glm.hpp>

struct Scene;

struct Object:
  Transformation
{
  inline void draw(const Scene &scene, const glm::mat4 &mvp) const;

  bool visible = true;

protected:
  virtual void render(const Scene &scene, const glm::mat4 &mvp) const = 0;
};

void Object::draw(const Scene &scene, const glm::mat4 &mvp) const
{
  if (visible)
    render(scene, mvp);
}

#endif // _OBJECT_HPP_
