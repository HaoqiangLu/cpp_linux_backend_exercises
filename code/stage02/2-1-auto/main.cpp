#include "AutoExercises.h"
#include <iostream>

int main() {
    auto data = generateData(10);

    autoIteratorDemo(data);
    std::cout << std::endl;

    std::map<std::string, int> map_data = {
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4}
    };
    autoMapDemp(map_data);
}