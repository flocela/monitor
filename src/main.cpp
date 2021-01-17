#include "ncurses_display.h"
#include "system.h"
#include "system_factory.h"

#include "linux_parser.h"
#include "format.h"
#include "processor.h"
#include <iostream>


int main() {
  SystemFactory system_factory = SystemFactory();
  System system = system_factory.createSystem(1, 1, 1);
  NCursesDisplay::Display(system);
}

