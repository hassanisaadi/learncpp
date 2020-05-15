#include "ncurses_display.h"
#include "system.h"

#include <iostream>
#include <vector>
#include <string>
#include "linux_parser.h"
#include "format.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
}