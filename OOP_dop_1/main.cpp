#include <iostream>
#include "src/RegisterDriver/RegisterDriver.h"

std::string deleteExtensionFile(std::string str);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Incorrect arguments are specified";
    return 1;
  }

  try {
    std::string name = argv[1];
    std::string nameWrite = deleteExtensionFile(name);
    std::string nameHeaderFile = nameWrite + "h";
    std::string nameIniFile = nameWrite + "ini";

    my_register::RegisterDriver rd;
    rd.readFile(name.c_str());

    rd.writeToHeaderFile(nameHeaderFile.c_str());
    rd.writeToIniFile(nameIniFile.c_str());
  }
  catch (my_register::RegisterException &e) {
    std::cout << e.what();
  }
  return 0;
}

std::string deleteExtensionFile(std::string str) {
  while (str[str.size() - 1] != '.') {
    str.pop_back();

    if (str.size() == 0)
      break;
  }

  if (str.size() == 0) {
    throw std::invalid_argument("Invalid name file");
  }

  return str;
}