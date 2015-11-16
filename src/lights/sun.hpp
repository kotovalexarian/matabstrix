#ifndef _LIGHT_SUN_HPP_
#define _LIGHT_SUN_HPP_

#include "light.hpp"

namespace Lights
{
  struct Sun:
    Light
  {
    glm::vec3 direction;
  };
};

#endif // _LIGHT_SUN_HPP_
