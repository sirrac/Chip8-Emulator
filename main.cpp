#include <iostream>
#include "chip8.hpp"

int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cout << "Please provide the path along with the main compilation." << std::endl;
    }

    const char* filePath = argv[1];
    Chip8 chip8 = Chip8();
    chip8.LoadRom(filePath);

    while (true) {
        //Need some sort of way to run the cycle inside the Chip8 and then call corresponding method, and then we can just call cycle on chip8. this is 
        //because instruction is contained in an individual chip8 object

        chip8.Cycle();
    }
}
