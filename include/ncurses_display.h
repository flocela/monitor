#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include "process.h"
#include "system.h"
#include <memory>
#include <curses.h>

namespace NCursesDisplay {
void Display(System& system, int n = 10);
void DisplaySystem(System& system, WINDOW* window);
void DisplayProcesses(std::vector<Process const*>const & processes, WINDOW* window, int n);
std::string ProgressBar(float percent);
};  // namespace NCursesDisplay

#endif