#include <stdio.h>
#include <math.h>
#include <time.h>

#define CNT 16000 /* ������ ���� (����� ���) */
#define CA 256 /* ���ᨬ���� ࠧ��� ��䠢�� */
#define BIT_BYTE 8 /* ��� � ����� ���� (᪮�쪮 ��� � ᨬ����) */
typedef unsigned long int ul;

struct StructTable
{

	ul numb_symb[ CA ]; /* ������⢮ ������� ᨬ���� �� ��䠢�� */
	int numb; /* ����쭮� ������⢮ ᨬ����� � ��䠢�� */
	ul sum; /* ������⢮ �ᥣ� ᨬ����� */
	double Ent; /* ���ய�� */
	double p[ CA ]; /* ����⭮���, ���� */
	double I_b; /* �����筮��� � ���� */
	double I_B; /* �����筮��� � ����� */

	/*
	***  ������� ������쭠 (MyStruct), ��� �� �㦭� �७㤨⥫쭮 ������� ������ ***
	*/

} MyStruct;

void fill_table( int count, unsigned char buf[ ] );
void output( );
inline void p_( );
void cnt_entr( );
void alg( );
