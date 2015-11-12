#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "transformation.hpp"
#include "model.hpp"

#include <glm/glm.hpp>

class Object :
  public Transformation
{
public:
  Object(const Model &model) : _model(model) {};
  void draw(const glm::mat4 &mvp) const;

  bool visible = true;

private:
  const Model &_model;
};

#endif // _OBJECT_HPP_
