#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "model.hpp"

#include <glm/glm.hpp>

class Object
{
public:
  Object(const Model &model) : _model(model) {};
  void draw(const glm::mat4 &mvp) const;

  glm::vec3 position;
  glm::vec3 rotation;

private:
  const Model &_model;
};

#endif // _OBJECT_HPP_
