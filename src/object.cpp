#include "object.hpp"

void Object::draw(const glm::mat4 &mvp) const
{
  if (!visible)
    return;

  _model.draw(mvp, transformation());
}
