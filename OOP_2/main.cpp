#include <iostream>

#include "src/StatisticMultiset.h"

int main ()
{
    try {
        std::cout << "Lab 02" << std::endl;

        StatisticMultiset < int > ms1;

        ms1.addNum(89);
        ms1.addNum(54);
        ms1.addNum(54);
        ms1.addNum(4);

        ms1.addNum({ 1, 2, 3 });

        StatisticMultiset < int > ms2;
        std::vector < int > somedata = { 280 };
        ms2.addNum(somedata);

        StatisticMultiset < int > ms3;
        ms3.addNum(ms1);
        ms3.addNum(ms2);

        std::cout << "Min: " << ms3.getMin()
                  << " Avg: " << ms3.getAvg()
                  << " Max: " << ms3.getMax()
                  << '\n' << ms3.getCountUnder(10) << std::endl;
    }
    catch (StatisticMultisetException &e){
        std::cerr << e.what();
    }

    return 0;
}