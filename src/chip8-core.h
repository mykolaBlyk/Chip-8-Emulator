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
        unsigned short opcode;          // двухбайтовый опкод
        unsigned char V[16];            // регистры общего назначения и флаг переноса VF
        unsigned short I;               // адрессный регистр
        unsigned short PC;              // указатель кода
        unsigned char SP;               // указаель стека

        unsigned short stack[16];       // массив хранящий в себе стек

        unsigned char memory[4096];     // память, в нее загружаем игру

        unsigned char delay_timer;      // таймеры задержки
        unsigned char sound_timer;      // и звука

        // функция рисующая спрайт в массив screen
        void drawSprite(unsigned char X, unsigned char Y, unsigned char N);

        unsigned char hp48_flags[8];    // флаги, необходимы для опкодов Fx75, Fx85

public:
        int mode;                       // переменная определяющая текущий режим:
                                        // 0 - CHIP-8
                                        // 1 - SCHIP

        unsigned char screen[128][64];  // массив представляющий экран

        unsigned char key[16];          // массив содержащий состояние клавиатуры:
                                        // 0 - клавиша отпущена
                                        // 1 - клавиша нажата

        bool stop;                      // переменная для опкода 00FD

        ChipCore();

        // инициализация переменных, загрузка шрифтов
        void init();

        // функция загрузки игры в память
        bool loadGame(const char *filename);

        // функция выполняющая опкод
        void executeNextOpcode();

        // функция уменьшающая значения таймеров
        void decreaseTimers();
};
#endif


