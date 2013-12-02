#include <stdio.h>
#include <math.h>
#include <time.h>

#define CNT 16000 /* Размер буфера (меньше инта) */
#define CA 256 /* Максимальный размер алфавита */
#define BIT_BYTE 8 /* Бит в одном байте (сколько бит в символе) */
typedef unsigned long int ul;

struct StructTable
{

	ul numb_symb[ CA ]; /* Количество каждого символа из алфавита */
	int numb; /* Реальное количество символов в алфавите */
	ul sum; /* Количесвтво всего символов */
	double Ent; /* Энтропия */
	double p[ CA ]; /* Вероятность, частота */
	double I_b; /* Избыточность в битах */
	double I_B; /* Избыточность в байтах */

	/*
	***  Структура глобальна (MyStruct), нам не нужно пренудительно обнулять элементы ***
	*/

} MyStruct;

void fill_table( int count, unsigned char buf[ ] );
void output( );
inline void p_( );
void cnt_entr( );
void alg( );
