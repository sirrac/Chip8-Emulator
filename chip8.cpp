#include "chip8.hpp"

Chip8::Chip8() {
    //initialize 

    delayTimer = 0;
    soundTimer = 0; 

    //memory from 0x000 to 0x1FF is reserved, except 0x050 to 0x0A0 which is fonts (just a convention apparenlty)
    //0x200-0xFFF: Instructions from the ROM will be stored starting at 0x200, and anything left after the ROM’s space is free to use.
    PC = 0x200;
    indexRegister = 0;
    sp = 0;

    memset(stack, 0, sizeof(stack));

    memset(memory, 0, sizeof(memory));
    memset(display, 0, sizeof(display));



    uint8_t fonts[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    }; 
    //using convention
    for (int i = 0; i < 80; i++) {
        memory[0x050 + i] = fonts[i];
    }
};

Chip8::~Chip8() {
    //Not sure if this is needed lol everything is on the stack for now
}

void Chip8::OP_00E0() { 
    //Clear video display
    memset(display, 0, sizeof(display));
}

void Chip8::OP_00EE() {
    //RET
    PC = stack[sp];
    sp = sp - 1;
}

void Chip8::OP_1nnn() { 
    ///JUMP

    //jump to last 12 bits of whatever the thing is saying. nnn is last 12 bits
    PC = instruction & 0x0FFF;
}

void Chip8::OP_2nnn() {
    sp++;
    stack[sp] = PC;
    PC = instruction & 0x0FFF;
}



void Chip8::OP_6xnn() {
    //set
    //remember to shift so the bits are on the right most side!
    uint16_t reg = (instruction & 0x0F00) >> 8u;
    uint16_t val = instruction & 0x00FF;

    V[reg] = val;
}

void Chip8::OP_7xnn() {
    //add. Need to check stuff with the carry flag and overflow later
    uint16_t reg = (instruction & 0x0F00) >> 8u;
    uint16_t val = instruction & 0x00FF;
    
    V[reg] += val;
}