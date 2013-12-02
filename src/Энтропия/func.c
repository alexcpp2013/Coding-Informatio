#include "func.h"

void fill_table( int count, unsigned char buf[ ] )
{

	int j = 0;
	for( j; j < count; ++j )
	{
		++MyStruct.numb_symb[ (int) buf[ j ] ];
	}

	return;

} /* fill_table() */

void output( )
{

	printf("\n\n--------------------------------------");
	printf("\n\nСодержание таблицы: \n");
	printf("\nСимвол\tDEC\tКоличество\tЧастота\t\tlog2\t\tp*log2\n ");
	int i = 0;
	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
			printf( "\n%c\t%i\t%li\t\t%f\t%f\t%f", i, i, MyStruct.numb_symb[ i ], MyStruct.p[ i ], log2( MyStruct.p[ i ] ), MyStruct.p[ i ] * log2( MyStruct.p[ i ] ) );
	}
	printf( "\n\nКоличество символов в файле:   %li", MyStruct.sum );

	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
			++MyStruct.numb;
	}
	printf( "\nКоличество символов в алфавите:   %i", MyStruct.numb );
	printf( "\nЭнтропия:   %f", MyStruct.Ent );
	printf( "\nКоличество информации:   %li * %i = %li бит", MyStruct.sum, BIT_BYTE, MyStruct.sum * BIT_BYTE );
	printf( "\nОптимальное количество информации:   %li * %f = %.0f бит", MyStruct.sum, MyStruct.Ent, ceil( ( double ) MyStruct.sum * MyStruct.Ent ) );
	if( MyStruct.Ent == 0 )
	{
		printf( "\nИзбыточность в битах:   бесконечность!" );
		printf( "\nИзбыточность в байтах:   бесконечность!" );
	}
	else
	{
		printf( "\nИзбыточность в битах:   %f %%", MyStruct.I_b * 100.0);
		printf( "\nИзбыточность в байтах:   %f %%", MyStruct.I_B * 100.0 );
	}
	printf("\n\n--------------------------------------");

/* --------------------------------------------------------------------------------- */

	/*

	FILE *FP;
	FP=fopen("forExcel.txt", "w");
	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
			fprintf( FP, "%i\t%li\n", i, MyStruct.numb_symb[ i ] );
	}
  	fclose(FP);

	*/

/* --------------------------------------------------------------------------------- */

	return;

} /* output() */

void alg( )
{

	int i = 0;
	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
			MyStruct.sum += MyStruct.numb_symb[ i ];
	}

	p_( );

	cnt_entr();

	output();

	return;

} /* alg() */

inline void p_( )
{

	/* Считаем вероятности */
	int i = 0;
	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
			MyStruct.p[ i ] = ( double ) MyStruct.numb_symb[ i ] / MyStruct.sum;
	}

	return;

} /* p_() */

void cnt_entr( )
{

	/* Считаем энтропию */
	int i = 0;
	MyStruct.Ent = 0.0;
	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
		{
			if( MyStruct.p[ i ] > 0.0 )
			{
				MyStruct.Ent += MyStruct.p[ i ] * log2( MyStruct.p[ i ] );
			}
			else
			{
				perror( "\n\aНе возможно посчитать энтропию вследствии некоректных данных о вероятности встречи символов в файле!" );
				perror( "\nДля выхода нажмите ВВОД.\n" );
				getchar( );
	    		exit( 1 );
			}
		}
	}

	MyStruct.Ent = ( -1.0 ) * MyStruct.Ent;

	if( MyStruct.Ent != 0.0 )
	{
		double b = ceil( ( double ) MyStruct.sum * MyStruct.Ent );
		double N = ( double ) MyStruct.sum * BIT_BYTE;
		MyStruct.I_b = N / b;
		MyStruct.I_B = MyStruct.sum / ceil( b / BIT_BYTE );
	}

	return;

} /* cnt_entr() */



