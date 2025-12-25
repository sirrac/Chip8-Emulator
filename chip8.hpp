#ifndef CHIP8_EMULATOR_HPP
#define CHIP8_EMULATOR_HPP
#include <cstdint>

class Chip8 {

//a byte is represented by the unit8_t. Use this for 8 bit position stuff
private:
    //16 8-bit registers
    uint8_t V[16];
    //index register, program Counter
    uint16_t indexRegister, PC;
    //stack pointer
    uint8_t sp;
    //Delay timer, sound timer
    uint8_t delayTimer, soundTimer;
    //4096 bytes of RAM
    uint8_t memory[4096];
    //Keypad
    uint8_t keyPad[16];
    //Stack each level holds two instructions I believe, so need two bytes so uint16. can have 16 levels
    uint16_t stack[16];
    //Display
    uint8_t display[64 * 32];

public: 
    Chip8();
    ~Chip8();


};

#endif