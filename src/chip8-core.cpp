
#include "chip8-core.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

ChipCore::ChipCore()
{
        init();
        srand(time(NULL));
}

// Clear registers, memory and stack. Load fonts and redraw screen.
void ChipCore::init()
{
        PC = 0x200;
        SP = 0;
        I = 0;
        delay_timer = 0;
        sound_timer = 0;

        for (int i = 0; i < 4096; i++)
                memory[i] = 0;

        for (int y = 0; y < 64; y++)
                for (int x = 0; x < 128; x++)
                        screen[x][y] = 0;

        for (int i = 0; i < 16; i++)
                V[i] = 0;

        for (int i = 0; i < 16; i++)
                key[i] = 0;

        for (int i = 0; i < 16; i++)
                stack[i] = 0;

        unsigned char font[16*5] = {
                0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
                0x20, 0x60, 0x20, 0x20, 0x70,   // 1
                0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
                0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
                0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
                0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
                0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
                0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
                0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
                0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
                0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
                0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
                0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
                0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
                0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
                0xF0, 0x80, 0xF0, 0x80, 0x80    // F
        };
        // Load font
        for (int i = 0; i < 16*5; i++)
                memory[i] = font[i];

        unsigned char bigfont[16*10] = {
                0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF,     // 0
                0x18, 0x78, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF,     // 1
                0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF,     // 2
                0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF,     // 3
                0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 	// 4
                0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF,     // 5
                0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF,     // 6
                0xFF, 0xFF, 0x03, 0x03, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x18, 	// 7
                0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF,     // 8
                0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF,     // 9
                0x7E, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 	// A
                0xFC, 0xFC, 0xC3, 0xC3, 0xFC, 0xFC, 0xC3, 0xC3, 0xFC, 0xFC, 	// B
                0x3C, 0xFF, 0xC3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC3, 0xFF, 0x3C, 	// C
                0xFC, 0xFE, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFE, 0xFC, 	// D
                0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 	// E
                0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0  	// F
        };

        // Load big font
        for (int i=0; i < 16*10; i++)
                memory[i+80] = bigfont[i];

        for (int i=0; i < 8; i++)
                hp48_flags[i] = 0;

        stop = false;

        mode = 0;
}

void ChipCore::drawSprite(unsigned char X, unsigned char Y, unsigned char N)
{
        V[0xF] = 0;

        if(mode==0){
        	drawSpriteChip8Mode(X,Y,N);

        }
        if(mode==1){
        	drawSpriteSCHIPMode(X,Y,N);
        }
}

void ChipCore::drawSpriteChip8Mode(char X, unsigned char Y, unsigned char N){
	 if (N == 0) N = 16;
	                		for (int yline = 0; yline < N; yline++)
	                        {
	                                unsigned char data = memory[I + yline];
	                                for (int xpix = 0; xpix < 8; xpix++)
	                                {
	                                        if((data & (0x80 >> xpix)) != 0)
	                                        {
	                                                if ((V[X] + xpix) < 64 && (V[Y] + yline) < 32 && (V[X] + xpix) >= 0 && (V[Y] + yline) >= 0)
	                                                {
	                                                        if (screen[(V[X] + xpix)*2][(V[Y] + yline)*2] == 1) V[0xF] = 1;
	                                                        screen[(V[X] + xpix)*2][(V[Y] + yline)*2] ^= 1;
	                                                        screen[(V[X] + xpix)*2 + 1][(V[Y] + yline)*2] ^= 1;
	                                                        screen[(V[X] + xpix)*2][(V[Y] + yline)*2 + 1] ^= 1;
	                                                        screen[(V[X] + xpix)*2 + 1][(V[Y] + yline)*2 + 1] ^= 1;
	                                                }
	                                        }
	                                }
	                        }
}

void ChipCore::drawSpriteSCHIPMode(char X, unsigned char Y, unsigned char N){
	if (N == 0)
	                                for (int yline = 0; yline < 16; yline++)
	                                {
	                                        unsigned char data = memory[I + yline*2];
	                                        for (int xpix = 0; xpix < 8; xpix++)
	                                        {
	                                                if((data & (0x80 >> xpix)) != 0)
	                                                {
	                                                        if ((V[X] + xpix) < 128 && (V[Y] + yline) < 64 && (V[X] + xpix) >= 0 && (V[Y] + yline) >= 0)
	                                                        {
	                                                                if (screen[V[X] + xpix][V[Y] + yline] == 1) V[0xF] = 1;
	                                                                screen[V[X] + xpix][V[Y] + yline] ^= 1;
	                                                        }
	                                                }
	                                        }
	                                        data = memory[I + 1 + yline*2];
	                                        for (int xpix = 0; xpix < 8; xpix++)
	                                        {
	                                                if((data & (0x80 >> xpix)) != 0)
	                                                {
	                                                        if ((V[X] + xpix + 8) < 128 && (V[Y] + yline) < 64 && (V[X] + xpix + 8) >= 0 && (V[Y] + yline) >= 0)
	                                                        {
	                                                                if (screen[V[X] + xpix + 8][V[Y] + yline] == 1) V[0xF] = 1;
	                                                                screen[V[X] + xpix + 8][V[Y] + yline] ^= 1;
	                                                        }
	                                                }
	                                        }
	                                }
	                        else
	                                for (int yline = 0; yline < N; yline++)
	                                {
	                                        unsigned char data = memory[I + yline];
	                                        for (int xpix = 0; xpix < 8; xpix++)
	                                        {
	                                                if((data & (0x80 >> xpix)) != 0)
	                                                {
	                                                        if ((V[X] + xpix) < 128 && (V[Y] + yline) < 64 && (V[X] + xpix) >= 0 && (V[Y] + yline) >= 0)
	                                                        {
	                                                                if (screen[V[X] + xpix][V[Y] + yline] == 1) V[0xF] = 1;
	                                                                screen[V[X] + xpix][V[Y] + yline] ^= 1;
	                                                        }
	                                                }
	                                        }
	                                }
}

void ChipCore::do00EE(){
	PC = stack[--SP];
}
void ChipCore::do00Cn(){
	  int N = opcode & 0x000F;
	  for (int y = 63; y > N; y--)
	      for (int x = 0; x < 128; x++)
	    	  screen[x][y] = screen[x][y-N];

	  for (int y = 0; y < N; y++)
		  for (int x = 0; x < 128; x++)
			  screen[x][y] = 0;
}

void ChipCore::do00E0(){
	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 128; x++)
			screen[x][y] = 0;
}

void ChipCore::do00FB(){
	for (int y = 0; y < 64; y++){
		for (int x = 127; x > 3; x--)
			screen[x][y] = screen[x-4][y];

		screen[0][y] = 0;
	    screen[1][y] = 0;
	    screen[2][y] = 0;
	    screen[3][y] = 0;
	}
}

void ChipCore::do00FC(){
	for (int y = 0; y < 64; y++){
		for (int x = 0; x < 124; x++)
			screen[x][y] = screen[x+4][y];
		screen[124][y] = 0;
	    screen[125][y] = 0;
	    screen[126][y] = 0;
	    screen[127][y] = 0;
	}
}

void ChipCore::do00FD(){
	stop = true;
	cout << "Quit the emulator" << endl;
}

void ChipCore::do00FE(){
	 mode = 0;
}

void ChipCore::do00FF(){
	mode = 1;
}

void ChipCore::do1nnn(){
	PC = opcode & 0x0FFF;
}
void ChipCore::do2nnn(){
	 stack[SP++] = PC;
	 PC = opcode & 0x0FFF;
}

void ChipCore::do3xkk(){
	if (V[((opcode & 0x0F00)>>8)] == (opcode & 0x00FF))
		PC += 2;
}

void ChipCore::do4xkk(){
	if (V[((opcode & 0x0F00)>>8)] != (opcode & 0x00FF))
		PC += 2;
}

void ChipCore::do5xy0(){
	if (V[((opcode & 0x0F00)>>8)] == V[((opcode & 0x00F0)>>4)])
		PC += 2;
}

void ChipCore::do6xkk(){
	V[((opcode & 0x0F00)>>8)] = opcode & 0x00FF;
}

void ChipCore::do7xkk(){
	V[((opcode & 0x0F00)>>8)] += opcode & 0x00FF;
}

void ChipCore::do8xy0(){
	V[((opcode & 0x0F00)>>8)] = V[((opcode & 0x00F0)>>4)];
}


void ChipCore::do8xy1(){
	V[((opcode & 0x0F00)>>8)] |= V[((opcode & 0x00F0)>>4)];
}


void ChipCore::do8xy2(){
	V[((opcode & 0x0F00)>>8)] &= V[((opcode & 0x00F0)>>4)];
}

void ChipCore::do8xy3(){
	V[((opcode & 0x0F00)>>8)] ^= V[((opcode & 0x00F0)>>4)];
}

void ChipCore::do8xy4(){
	int i;
	i = static_cast<int>(V[((opcode & 0x0F00)>>8)]) + static_cast<int>(V[((opcode & 0x00F0)>>4)]);
    if (i > 255)
    	 V[0xF] = 1;
	else
		V[0xF] = 0;
    V[((opcode & 0x0F00)>>8)] = i;
}

void ChipCore::do8xy5(){
	if (V[((opcode & 0x0F00)>>8)] >= V[((opcode & 0x00F0)>>4)])
		V[0xF] = 1;
	else
		V[0xF] = 0;
	V[((opcode & 0x0F00)>>8)] -= V[((opcode & 0x00F0)>>4)];
}

void ChipCore::do8xy6(){
	V[0xF] = V[((opcode & 0x0F00)>>8)] & 0x1;
	V[((opcode & 0x0F00)>>8)] >>= 1;
}


void ChipCore::do8xy7(){
	if (V[((opcode & 0x00F0)>>4)] >= V[((opcode & 0x0F00)>>8)])
		V[0xF] = 1;
	else
		V[0xF] = 0;
	V[((opcode & 0x0F00)>>8)] = V[((opcode & 0x00F0)>>4)] - V[((opcode & 0x0F00)>>8)];
}

void ChipCore::do8xyE(){
	V[0xF] = (V[((opcode & 0x0F00)>>8)] >> 7) & 0x01;
	V[((opcode & 0x0F00)>>8)] <<= 1;
}

void ChipCore::do9xy0(){
	if (V[((opcode & 0x0F00)>>8)] != V[((opcode & 0x00F0)>>4)])
		PC += 2;
}

void ChipCore::doAnnn(){
	 I = opcode & 0x0FFF;
}

void ChipCore::doBnnn(){
	 PC = (opcode & 0x0FFF) + V[0];
}

void ChipCore::doCxkk(){
	V[((opcode & 0x0F00)>>8)] = (rand() % 255) & (opcode & 0x00FF);
}

void ChipCore::doEx9E(){
	if (key[V[((opcode & 0x0F00)>>8)]] == 1)
		PC += 2;
}

void ChipCore::doExA1(){
	if (key[V[((opcode & 0x0F00)>>8)]] == 0)
		PC += 2;
}

void ChipCore::doFx07(){
	V[((opcode & 0x0F00)>>8)] = delay_timer;
}

void ChipCore::doFx0A(){
	PC -= 2;
	for (unsigned char n=0; n < 16; n++){
		if (key[n] == 1)
		{
			V[((opcode & 0x0F00)>>8)] = n;
			PC += 2;
			break;
		}
	}
}

void ChipCore::doFx15(){
	 delay_timer = V[((opcode & 0x0F00)>>8)];
}

void ChipCore::doFx18(){
	sound_timer = V[((opcode & 0x0F00)>>8)];
}

void ChipCore::doFx1E(){
	if ((I += V[((opcode & 0x0F00)>>8)]) > 0xfff)
		V[0xF] = 1;
	else
		V[0xF] = 0;
}

void ChipCore::doFx29(){
	 I = V[((opcode & 0x0F00)>>8)] * 5;
}

void ChipCore::doFx30(){
	I = V[((opcode & 0x0F00)>>8)] * 10 + 80;
}

void ChipCore::doFx33(){
	int n;
	n = V[((opcode & 0x0F00)>>8)];
	memory[I] = (n - (n % 100)) / 100;
	n -= memory[I] * 100;
	memory[I+1] = (n - (n % 10)) / 10;
	n -= memory[I+1] * 10;
	memory[I+2] = n;
}

void ChipCore::doFx55(){
	for (int n=0; n <= ((opcode & 0x0F00)>>8); n++)
		memory[I+n] = V[n];
}

void ChipCore::doFx65(){
	for (int n=0; n <= ((opcode & 0x0F00)>>8); n++)
		V[n] = memory[I+n];
}

void ChipCore::doFx75(){
	for (int i=0; i <= ((opcode & 0x0F00)>>8); i++)
		hp48_flags[i] = V[i];
}

void ChipCore::doFx85(){
	for (int i=0; i <= ((opcode & 0x0F00)>>8); i++)
		V[i] = hp48_flags[i];
}


void ChipCore::executeNextOpcode()
{
        opcode = (memory[PC]<<8) + memory[PC+1];

        PC += 2;

        switch ((opcode & 0xF000)>>12)
        {
                case 0x0:
                        if ((opcode & 0x00F0)>>4 == 0xC) // 00CN - scroll display N lines down *SCHIP*
                        {
                        	do00Cn();
                        	break;
                        }

                        switch  (opcode & 0x00FF)
                        {
                                case 0xE0:              // 00E0 - clear the screen
                                		do00E0();
                                        break;
                                case 0xEE:              // 00EE - return from subroutine
                                		do00EE();
                                        break;
                                case 0xFB:              // 00FB - scroll display 4 pixels right *SCHIP*
                                		do00FB();
                                        break;
                                case 0xFC:              // 00FB - scroll display 4 pixels left *SCHIP*
                                        do00FC();
                                        break;
                                case 0xFD:              // 00FD - Quit the emulator
                                		do00FD();
                                        break;
                                case 0xFE:              // 00FE - disable extended screen mode *SCHIP*
                                		do00FE();
                                        break;
                                case 0xFF:              // 00FF - enable extended screen mode *SCHIP*
                                		do00FF();
                                        break;
                                default:
                                        cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
                        }
                        break;

                case 0x1: // 1NNN - jump to addr
                        do1nnn();
                        break;
                case 0x2: // 2NNN - call subroutine
                       	do2nnn();
                        break;
                case 0x3: // 3XKK - skip next instruction if VX == Byte
                        do3xkk();
                        break;
                case 0x4: // 4XKK - skip next instruction if VX != Byte
                        do4xkk();
                        break;
                case 0x5: // 5XY0 - skip next instruction if VX == VY
                        do5xy0();
                        break;
                case 0x6: // 6XKK - set VX = Byte
                		do6xkk();
                        break;
                case 0x7: // 7XKK - set VX = VX + Byte
                        do7xkk();
                        break;
                case 0x8:
                        switch (opcode & 0x000F)
                        {
                                case 0x0: // 8XY0 - set VX = VY
                                        do8xy0();
                                        break;
                                case 0x1: // 8XY1 - set VX = VX | VY
                                        do8xy1();
                                        break;
                                case 0x2: // 8XY2 - set VX = VX & VY
                                        do8xy2();
                                        break;
                                case 0x3: // 8XY3 - set VX = VX ^ VY
                                        do8xy3();
                                        break;
                                case 0x4: // 8XY4 - set VX = VX + VY, VF = carry
                                        do8xy4();
                                        break;
                                case 0x5: // 8XY5 - set VX = VX - VY, VF = !borrow
                                        do8xy5();
                                        break;
                                case 0x6: // 8XY6 - set VX = VX >> 1, VF = carry
                                        do8xy6();
                                        break;
                                case 0x7: // 8XY7 - set VX = VY - VX, VF = !borrow
                                        do8xy7();
                                        break;
                                case 0xE: // 8XYE - set VX = VX << 1, VF = carry
                                        do8xyE();
                                        break;

                                default:
                                        cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
                        }
                        break;

                case 0x9: // 9XY0 - skip next instruction if VX != VY
                        do9xy0();
                        break;
                case 0xA: // ANNN - set I = Addr
                       	doAnnn();
                        break;
                case 0xB: // BNNN - jump to Addr + V0
                       	doBnnn();
                        break;

                case 0xC: // CXKK - set VX = random & Byte
                        doCxkk();
                        break;

                case 0xD: // DXYN - Draw sprite
                        drawSprite(((opcode & 0x0F00)>>8), ((opcode & 0x00F0)>>4), (opcode & 0x000F));
                        break;

                case 0xE:
                        switch (opcode & 0x00FF)
                        {
                                case 0x9E: // EX9E - skip next instruction if key VX down
                                        doEx9E();
                                        break;
                                case 0xA1: // EXA1 - skip next instruction if key VX up
                                        doExA1();
                                        break;
                                default:
                                        cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
                        }
                        break;
                case 0xF:
                        switch (opcode & 0x00FF)
                        {
                                case 0x07: // FX07 - set VX = delaytimer
                                        doFx07();
                                        break;
                                case 0x0A: // FX0A - set VX = key, wait for keypress
                                        doFx0A();
                                        break;
                                case 0x15: // FX15 - set delaytimer = VX
                                        doFx15();
                                        break;
                                case 0x18: // FX18 - set soundtimer = VX
                                        doFx18();
                                        break;
                                case 0x1E: // FX1E - set I = I + VX; set VF if buffer overflow
                                        doFx1E();
                                        break;
                                case 0x29: // FX29 - point I to 5 byte numeric sprite for value in VX
                                        doFx29();
                                        break;

                                case 0x30: // FX30 - point I to 10 byte numeric sprite for value in VX *SCHIP*
                                        doFx30();
                                        break;
                                case 0x33: // FX33 - store BCD of VX in [I], [I+1], [I+2]
                                        doFx33();
                                        break;
                                case 0x55: // FX55 - store V0 .. VX in [I] .. [I+X]
                                        doFx55();
                                        break;

                                case 0x65: // FX65 - read V0 .. VX from [I] .. [I+X]
                                        doFx65();
                                        break;

                                case 0x75: // FX75 - save V0...VX (X<8) in the HP48 flags *SCHIP*
                                        doFx75();
                                        break;

                                case 0x85: // FX85 - load V0...VX (X<8) from the HP48 flags *SCHIP*
                                        doFx85();
                                        break;

                                default:
                                        cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
                        }
                        break;

                default:
                        cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
        }
}


bool ChipCore::loadGame(const char *filename)
{
        ifstream file(filename, ios::in | ios::binary | ios::ate);
        if (file.is_open())
        {
                ifstream::pos_type size;
                size = file.tellg();
                if (size > 0x0FFF - 0x200)
                {
                        cerr << "Error: file '" << filename << "' is too large." << endl;
                        return false;
                }

                file.seekg(0, ios::beg);
                file.read(reinterpret_cast<char*>(&memory[0x200]), size);
                file.close();

                cout << "File '" << filename << "' loaded." << endl;
                return true;
        }

        cerr << "Error: unable to open file '" << filename << "'" << endl;
        return false;
}

void ChipCore::decreaseTimers()
{
        if(delay_timer > 0)
                --delay_timer;

        if(sound_timer > 0)
                --sound_timer;
}



