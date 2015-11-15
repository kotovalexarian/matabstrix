#ifndef _OBJECT_WITH_MODEL_HPP_
#define _OBJECT_WITH_MODEL_HPP_

#include "object.hpp"
#include "../model/model.hpp"

struct Scene;

namespace Objects
{
  struct WithModel:
    Object
  {
    WithModel(const Model &model) : _model(model) {};

  protected:
    void render(const Scene &scene, const glm::mat4 &mvp) const;

  private:
    const Model &_model;
  };
};

#endif // _OBJECT_WITH_MODEL_HPP_
