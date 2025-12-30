#include <iostream>
#include <SDL3/SDL.h>
#include "chip8.hpp"


int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cout << "Please provide the path along with the main compilation." << std::endl;
    }

    const char* filePath = argv[1];
    Chip8 chip8 = Chip8();
    chip8.LoadRom(filePath);

    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
}
