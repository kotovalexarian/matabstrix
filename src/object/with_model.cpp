#include "with_model.hpp"

using namespace Objects;

void WithModel::render(const Scene &scene, const glm::mat4 &mvp) const
{
  _model.draw(scene, mvp, transformation());
}
