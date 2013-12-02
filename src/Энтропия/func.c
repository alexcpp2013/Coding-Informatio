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
	printf("\n\n����ঠ��� ⠡����: \n");
	printf("\n������\tDEC\t������⢮\t�����\t\tlog2\t\tp*log2\n ");
	int i = 0;
	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
			printf( "\n%c\t%i\t%li\t\t%f\t%f\t%f", i, i, MyStruct.numb_symb[ i ], MyStruct.p[ i ], log2( MyStruct.p[ i ] ), MyStruct.p[ i ] * log2( MyStruct.p[ i ] ) );
	}
	printf( "\n\n������⢮ ᨬ����� � 䠩��:   %li", MyStruct.sum );

	for( i = 0; i < CA; ++i )
	{
		if( MyStruct.numb_symb[ i ] != 0 )
			++MyStruct.numb;
	}
	printf( "\n������⢮ ᨬ����� � ��䠢��:   %i", MyStruct.numb );
	printf( "\n���ய��:   %f", MyStruct.Ent );
	printf( "\n������⢮ ���ଠ樨:   %li * %i = %li ���", MyStruct.sum, BIT_BYTE, MyStruct.sum * BIT_BYTE );
	printf( "\n��⨬��쭮� ������⢮ ���ଠ樨:   %li * %f = %.0f ���", MyStruct.sum, MyStruct.Ent, ceil( ( double ) MyStruct.sum * MyStruct.Ent ) );
	if( MyStruct.Ent == 0 )
	{
		printf( "\n�����筮��� � ����:   ��᪮��筮���!" );
		printf( "\n�����筮��� � �����:   ��᪮��筮���!" );
	}
	else
	{
		printf( "\n�����筮��� � ����:   %f %%", MyStruct.I_b * 100.0);
		printf( "\n�����筮��� � �����:   %f %%", MyStruct.I_B * 100.0 );
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

	/* ��⠥� ����⭮�� */
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

	/* ��⠥� ��ய�� */
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
				perror( "\n\a�� �������� ������� ��ய�� �᫥��⢨� ����४��� ������ � ����⭮�� ����� ᨬ����� � 䠩��!" );
				perror( "\n��� ��室� ������ ����.\n" );
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



