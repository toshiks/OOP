#include <iostream>

#include "src/WAV/Wav.h"
#include "src/WavException/WavException.h"

using namespace std;

int main(int argc, char *argv[])
{
    try {
        Wav w;
        w.createFromFile("E:\\CLionProjects\\OOP\\OOP\\OOP_3\\0.Wav");
        w.printInfo();
        w.cutEnd(47);
        w.save("E:\\CLionProjects\\OOP\\OOP\\OOP_3\\2.Wav");
        w.printInfo();
    }
    catch (WavException &e){
        std::cerr << e.what();
    }

    return 0;
}
