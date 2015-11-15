#include "object.hpp"

void Object::draw(const Scene &scene, const glm::mat4 &mvp) const
{
  if (!visible)
    return;

  _model.draw(scene, mvp, transformation());
}
