#include <cstdlib>

#include <GL/glfw.h>

#include <emscripten/emscripten.h>

static void iterate();

int main()
{
  if (!glfwInit())
    exit(EXIT_FAILURE);

  if (!glfwOpenWindow(640, 480, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  emscripten_set_main_loop(iterate, 60, 1);
}

void iterate()
{
  glfwPollEvents();
}
