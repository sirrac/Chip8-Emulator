#ifndef CHIP8_EMULATOR_HPP
#define CHIP8_EMULATOR_HPP
#include <cstdint>
#include <string.h>

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

    //16 bit instruction
    uint16_t instruction;

public: 
    Chip8();
    ~Chip8();
    void LoadRom();

    //instructions
    void OP_00E0(); //CLEAR
    void OP_00EE(); //RET
    void OP_1nnn(); //JUMP
    void OP_2nnn(); //CALL

    void OP_3xnn(); //skip next instruction if Vx = nn
    void OP_4xnn(); //skip next instruction if Vx != nn
    void OP_5xy0(); //skip next instruction if Vx = Vy
    void OP_6xnn(); //Set value register
    void OP_7xnn(); //add value to this register


};

#endif