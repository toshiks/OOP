#include <iostream>
#include "src/IniParser/IniParser.h"
#include <stack>

int main ()
{
    IniParser parser;
    try {
        parser.initial("E:\\CLionProjects\\OOP\\OOP\\OOP_1\\lab01_input_correct_09.ini");
        float t = parser.getValue<float>("DEBUG", "PlentySockMaxQSize");
        std::cout << t;
    }
    catch (IniParserException e){
        std::cerr << e.what();
    }

    return 0;
}