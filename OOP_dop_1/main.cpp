#include <iostream>
#include "src/RegisterDriver/RegisterDriver.h"

int main() {
  try {
    my_register::RegisterDriver rd;
    rd.readFile("input_example_1.txt");

    rd.writeToHeaderFile("output_example_1.h");
    rd.writeToIniFile("output_example_1.ini");
  }
  catch (my_register::RegisterException &e) {
    std::cout << e.what();
  }
  return 0;
}