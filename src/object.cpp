#include "object.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Object::draw(const glm::mat4 &mvp) const
{
  const glm::mat4 transform = mvp * transformation();
  glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, glm::value_ptr(transform));

  const glm::mat3 local_modelview = glm::transpose(glm::inverse(glm::mat3(transformation())));
  glUniformMatrix3fv(local_modelview_uniform, 1, GL_FALSE, glm::value_ptr(local_modelview));

  _model.draw();
}
