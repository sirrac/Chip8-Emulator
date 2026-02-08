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
            
            chip8.Cycle();
            std::cout << "Instruction done!" << std::endl;
        }
}
