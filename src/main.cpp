#include "ncurses_display.h"
#include "system.h"
#include "system_factory.h"

int main() {
  SystemFactory system_factory = SystemFactory();
  std::unique_ptr<System> system = system_factory.createSystem(1, 1, 1, 1);
  NCursesDisplay::Display(*system);
}

