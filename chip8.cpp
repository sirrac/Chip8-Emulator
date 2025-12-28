#include "chip8.hpp"

Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()), randByte(0, 255) {
    //initialize 

    delayTimer = 0;
    soundTimer = 0; 

    //memory from 0x000 to 0x1FF is reserved, except 0x050 to 0x0A0 which is fonts (just a convention apparenlty)
    //0x200-0xFFF: Instructions from the ROM will be stored starting at 0x200, and anything left after the ROM’s space is free to use.
    PC = 0x0200;
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

void Chip8::LoadRom(const char* filePath) {
    //initialize a stream, specifiy it's binary, put pointer at end
    std::ifstream inputFile(filePath, std::ios::binary | std::ios::ate);
    
    if (inputFile.is_open()) {
        //create stream position object to essentially get size of file
        std::streampos size = inputFile.tellg(); //tellg returns position of where you are in the stream
        char* bufferArray = new char[size]; //dynamically allocated as we don't size at compile time

        //Now, that we know size, we can use ifstream .read() to copy into array
        inputFile.seekg(std::ios::beg); //use seekg to move the get pointer at the beginning of the file
        inputFile.read(bufferArray, size);

        //Fix this, size is a std::streampos which can't be iterated with int I'm pretty sure
        for (int i = 0; i < size; i++) {
            memory[0x200 + i] = bufferArray[i];
        }

        delete[] bufferArray; 

    }
}

void Chip8::Cycle() {
    //Instructions are two bytes

    //Fetch
    uint8_t firstByte = memory[PC];
    uint8_t secondByte = memory[PC + 1];

    uint16_t currentInstruction = static_cast<uint16_t>(firstByte) << 8u | secondByte;

    PC = PC + 2;


    switch 


}

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

void Chip8::OP_3xnn() {
    uint8_t reg = (instruction & 0x0F00) >> 8u;
    uint8_t val = instruction & 0x00FF;

    if (V[reg] == val) {
        PC += 2;
    }
}

void Chip8::OP_4xnn() {
    uint8_t reg = (instruction & 0x0F00) >> 8u;
    uint8_t val = instruction & 0x00FF;

    if (V[reg] != val) {
        PC += 2;
    }
}

void Chip8::OP_5xy0() {
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    if (V[regOne] == V[regTwo]) {
        PC += 2;
    }
}

void Chip8::OP_6xnn() {
    //set
    //remember to shift so the bits are on the right most side!
    uint8_t reg = (instruction & 0x0F00) >> 8u;
    uint8_t val = instruction & 0x00FF;

    V[reg] = val;
}

void Chip8::OP_7xnn() {
    //add. Need to check stuff with the carry flag and overflow later
    uint8_t reg = (instruction & 0x0F00) >> 8u;
    uint8_t val = instruction & 0x00FF;
    
    V[reg] += val;
}

void Chip8::OP_8xy0() {
    //Assignment of one register to another
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    V[regOne] = V[regTwo];
}

void Chip8::OP_8xy1() {
    //Assignment of bitwise OR to first register
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    V[regOne] = V[regOne] | V[regTwo];
}

void Chip8::OP_8xy2() {
    //Assignment of bitwise AND to first register
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    V[regOne] = V[regOne] & V[regTwo];
}

void Chip8::OP_8xy3() {
    //Assignment of bitwise XOR to first register
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    V[regOne] = V[regOne] ^ V[regTwo];
}

void Chip8::OP_8xy4() {
   // Set Vx = Vx + Vy, set VF = carry.
    //The values of Vx and Vy are added together. 
    //If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;
    uint16_t added = V[regOne] + V[regTwo];

    if (added > 255) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }
    uint8_t added8bit = added & 0xFF;
    V[regOne] = added8bit;

}

void Chip8::OP_8xy5() {
   //Set Vx = Vx - Vy, set VF = NOT borrow.

//If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    if (V[regOne] > V[regTwo]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[regOne] = V[regOne] - V[regTwo];
}

void Chip8::OP_8xy6() {
    //bitwise shift right
    uint8_t regOne = (instruction & 0x0F00) >> 8u;

    if (V[regOne] % 2 == 1) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[regOne] = V[regOne] >> 1;
}

void Chip8::OP_8xy7() {
   // Set Vx = Vy - Vx, set VF = NOT borrow.

    //If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    if (V[regTwo] > V[regOne]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[regOne] = V[regTwo] - V[regOne];

}

void Chip8::OP_8xyE() {
    //bitwise shift left
    uint8_t regOne = (instruction & 0x0F00) >> 8u;

    V[0xF] = (V[regOne] & 0xF0) >> 7u; //we only care about the MSB, so move that to appropriate space 

    V[regOne] = V[regOne] << 1;
}


void Chip8::OP_9xy0() {

    //inequality for two registers

    uint8_t regOne = (instruction & 0x0F00) >> 8u;
    uint8_t regTwo = (instruction & 0x00F0) >> 4u;

    if (V[regOne] != V[regTwo]) {
        PC += 2;
    }
}

void Chip8::OP_Annn() {
    indexRegister = (instruction & 0x0FFF);
}

void Chip8::OP_Bnnn() {
    uint16_t reg = (instruction & 0x0FFF) + V[0];
    PC = reg;
}

void Chip8::OP_Ex9E() {
    uint8_t reg = (instruction & 0x0F00) >> 8u;

    if (keyPad[V[reg]]) {
        PC = PC + 2;
    }
}

void Chip8::OP_ExA1() {
    uint8_t reg = (instruction & 0x0F00) >> 8u;

    if (!keyPad[V[reg]]) {
        PC = PC + 2;
    }
}

void Chip8::OP_Fx0A() {
   // All execution stops until a key is pressed, then the value of that key is stored in Vx.

}

void Chip8::OP_Fx07() {
    uint8_t reg = (instruction & 0x0F00) >> 8u;
    V[reg] = delayTimer;
}

void Chip8::OP_Fx15() {
    uint8_t reg = (instruction & 0x0F00) >> 8u;
    delayTimer = V[reg];
}


void Chip8::OP_Fx18() {
    uint16_t reg = (instruction & 0x0F00) >> 8u;
    soundTimer = V[reg];
}

void Chip8::OP_Fx1E() {
    uint16_t reg = (instruction & 0x0F00) >> 8u;
    indexRegister += V[reg];
}


void Chip8::OP_Fx55() {

    //Store registers V0 through Vx in memory starting at location I.

    //The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.

    uint8_t reg = (instruction & 0x0F00) >> 8u;

    for (uint8_t i = 0; i < reg + 1; i++) { //inclusive of Vx i believe
        memory[indexRegister + i] = V[i];
    }
}


void Chip8::OP_Fx65() {

  // Read registers V0 through Vx from memory starting at location I.

    //The interpreter reads values from memory starting at location I into registers V0 through Vx.
    uint8_t reg = (instruction & 0x0F00) >> 8u;

    for (uint8_t i = 0; i < reg + 1; i++) {
        V[i] = memory[i + indexRegister];
    }
}





