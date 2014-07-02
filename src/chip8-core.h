/*
 * chip8-core.h
 *
 *
 *      Author: mykola
 */

#ifndef CHIP_CORE
#define CHIP_CORE

#include <string>

class ChipCore
{
private:
        unsigned short opcode;
        unsigned char V[16];
        unsigned short I;
        unsigned short PC;
        unsigned char SP;
        unsigned short stack[16];
        unsigned char memory[4096];
        unsigned char delay_timer;
        unsigned char sound_timer;
        void drawSprite(unsigned char X, unsigned char Y, unsigned char N);
        unsigned char hp48_flags[8];

        void do00EE();
        void do00Cn();
        void do00E0();
        void do1nnn();
        void do2nnn();
        void do3xkk();
        void do4xkk();
        void do5xy0();
        void do6xkk();
        void do7xkk();
        void do8xy0();
        void do8xy1();
        void do8xy2();
        void do8xy3();
        void do8xy4();
        void do8xy5();
        void do8xy6();
        void do8xy7();
        void do8xyE();
        void do9xy0();
        void doAnnn();
        void doBnnn();
        void doCxkk();
        void doEx9E();
        void doExA1();
        void doFx07();
        void doFx0A();
        void doFx15();
        void doFx18();
        void doFx1E();
        void doFx29();
        void doFx30();
        void doFx33();
        void doFx55();
        void doFx65();
        void doFx75();
        void doFx85();
        void do00FB();
        void do00FC();
        void do00FD();
        void do00FE();
        void do00FF();

        void drawSpriteChip8Mode(char X, unsigned char Y, unsigned char N);
        void drawSpriteSCHIPMode(char X, unsigned char Y, unsigned char N);

public:
        int mode;
        unsigned char screen[128][64];
        unsigned char key[16];
        bool stop;
        ChipCore();
        void init();
        bool loadGame(const char *filename);
        void executeNextOpcode();
        void decreaseTimers();
};
#endif


