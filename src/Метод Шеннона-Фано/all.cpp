// Метод Шеннона-Фано
// Лабораторная работа по "ТИК"
// Мельник О. В.

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

typedef unsigned long int ul;

/*
	Эллементы не нужно обнулять, они уже обнулены (глобальные)
*/

const int ALF = 256; // Размер алфавита
ul TABLE_OF_CHAR[ ALF ]; // Массив с символами
ul FILE_SIZE; // Размер файла данных (нужен для восстановления)
ul NEW_FILE_SIZE; // Новый размер (нужен для востановления)

int REAL_TABLE_SIZE; // Реальное количесвто элементо в алфавите
const int BUFFER_SIZE = 4096; // Размер буфера для данных
const int CODE_SIZE = 128; // Максимальная длина кода
const int BYTE = 8; // Размер символа

struct StructTable
{

	unsigned char symbol;
	ul count;
	short code[ CODE_SIZE ];
	unsigned int code_length;

} TEMP, MyStruct[ ALF ]; // Временная структура и массив структур для каждого нового символа

clock_t start_time, end_time;

char * fille_name = "test"; // Имя файла для обработки
char * new_fille_name = "test.nnm"; // Имя нового файла для обработки

void READ( ); // Считывани файла
void sort( );
void code( int, int, ul ); // Составление кода
void encode( ); // Кодирование
inline void print( );

//-----------------------------------------------------------------------------------------------

using namespace std;

int main( int argc, char * argv[ ] )
{

	start_time = clock();

	READ( );

	sort( );

	code( 0, REAL_TABLE_SIZE - 1, FILE_SIZE );

	encode( );

	end_time = clock( );

	print( );

	cout << endl << "Час, що був затрачений на кодування (секунд): " << static_cast< double >( end_time - start_time ) / CLOCKS_PER_SEC << endl;

}

//----------------------------------------------------------------------------------------------------------------

void READ( )
{

	ifstream SimplyFile( fille_name, ios :: in | ios :: binary );
	if( !SimplyFile )
	{
		cerr << "Неможливо вiдкрити файл! \a" << std :: endl;
		exit( 1 );
	}

	char buffer[ BUFFER_SIZE ];
	int count_fille;
	for( ; ; )
	{
		SimplyFile.read( buffer, BUFFER_SIZE );
		count_fille = SimplyFile.gcount( );
		for( int i = 0; i < count_fille; ++i )
		{
			++TABLE_OF_CHAR[ static_cast< unsigned char >( buffer[ i ] ) ];
			++FILE_SIZE;
		}
		if( count_fille != BUFFER_SIZE )
		{
			SimplyFile.close( );

			//Скопируем в массив структур новые данные про файл

			for( int i = 0; i < ALF; ++i )
			{
				if( TABLE_OF_CHAR[ i ] != 0 )
				{
					++REAL_TABLE_SIZE;
				}
			}

			for( int i = 0, k = 0; i < ALF; ++i )
			{
				if( TABLE_OF_CHAR[ i ] != 0 )
				{
					MyStruct[ k ].symbol = i;
					MyStruct[ k ].count = TABLE_OF_CHAR[ i ];
					MyStruct[ k ].code_length = 0;
					++k;
				}
			}

			break;
		}
	}

}

void sort( ) // В связи с незначительным размером массива используем Пузырек
{

	for( int i = 0; i < REAL_TABLE_SIZE; ++i )
	{
		for( int j = 0; j < REAL_TABLE_SIZE - 1; ++j )
		{
			if( MyStruct[ j ].count < MyStruct[ j + 1 ].count )
			{
				TEMP = MyStruct[ j ];
				MyStruct[ j ] = MyStruct[ j + 1 ];
				MyStruct[ j + 1 ] = TEMP;
			}
		}
	}

}

void code( int start, int end, ul sum ) // Рекурсивно создаем коды для каждого символа в таблице
{

	if( start == end )
	{
		return;
	}
	if( end == start + 1 )
	{
		MyStruct[ start ].code[ MyStruct[ start ].code_length++ ] = 1;
		MyStruct[ end ].code[ MyStruct[ end ].code_length++ ] = 0;
		return;
	}

	ul a = 0, b = 0;
	int m = start, n = end;
	for( ; a + b != sum; )
	{
		if( a < b )
		{
			a += MyStruct[ m ].count;
			MyStruct[ m ].code[ MyStruct[ m ].code_length++ ] = 1;
			++m;
		}
		else
		{
			b += MyStruct[ n ].count;
			MyStruct[ n ].code[ MyStruct[ n ].code_length++ ] = 0;
			--n;
		}
	}

	code( start, m - 1, a );
	code( m, end, b );

}

void encode( )
{

	ofstream Archv( new_fille_name, ios :: out | ios :: binary );

	// Храним таблицу в файле

	Archv << FILE_SIZE << '\n' << REAL_TABLE_SIZE << '\n';
	for( int i = 0; i < REAL_TABLE_SIZE; ++i )
	{
		Archv << static_cast< int >( MyStruct[ i ].symbol ) << ' ' << MyStruct[ i ].count << '\n';
	}

	ifstream SimplyFile( fille_name, ios :: in | ios :: binary );
	char buffer[ BUFFER_SIZE ], outbuffer[ BUFFER_SIZE ];
	unsigned char byte = 0;
	int n = 0, m = 0, k = 0, pos = 0;
	int j = 0;
	for( ; ; )
	{
		SimplyFile.read( buffer, BUFFER_SIZE );
		n = SimplyFile.gcount( );
		for( int i = 0; i < n; ++i )
		{
			for( j = 0; j < REAL_TABLE_SIZE; ++j )
			{
				if( MyStruct[ j ].symbol == static_cast< unsigned char >( buffer[ i ] ) )
				{
					break;
				}
			}
			for( ; ; )
			{
				if( pos == BYTE )
				{
					outbuffer[ m ] = byte;
					++m;
					byte = 0;
					pos = 0;
				}
				if( m == BUFFER_SIZE )
				{
					Archv.write( outbuffer, m );
					NEW_FILE_SIZE += m;
					m = 0;
				}
				if( k == MyStruct[ j ].code_length )
				{
					k = 0;
					break;
				}
				byte <<= 1;
				byte |= MyStruct[ j ].code[ k ];
				++pos;
				++k;
			}
		}
		if( n != BUFFER_SIZE )
		{
			SimplyFile.close( );
			break;
		}
	}

	if( m != BUFFER_SIZE )
	{
		Archv.write( outbuffer, m );
		NEW_FILE_SIZE +=  m;
	}

	if( pos != BYTE && pos != 0 )
	{
		for( int i = pos; i < BYTE; ++i )
		{
			byte <<= 1;
			byte |= 0;
		}
		Archv.put( byte );
		++NEW_FILE_SIZE;
	}
	Archv.close( );

}

inline void print( )
{

	/* --------------------------------------------------------------------------------- */

		/* Запись в файл для Екселя */

		/*

		FILE *FP;
		FP = fopen( "forExcel.txt", "w" );
		int k = 0;
		for( k = 0; k < REAL_TABLE_SIZE; ++k )
		{
			fprintf( FP, "%i\t%li\n", MyStruct[ k ].symbol, MyStruct[ k ].count );
		}
	  	fclose(FP);

		*/

	/* --------------------------------------------------------------------------------- */

	cout << endl << "Символ\t" << "DEC\t" << "HEX\t" << "Кiлькiсть\t" << "Код" << endl << endl;
	for( int i = 0; i < REAL_TABLE_SIZE; ++i )
	{
		cout << MyStruct[ i ].symbol << '\t'  << static_cast< unsigned int >( MyStruct[ i ].symbol );
		cout << '\t' << hex << static_cast< unsigned int >( MyStruct[ i ].symbol );
		cout << '\t' << dec << MyStruct[ i ].count << '\t' << '\t';
		for( int j = 0; j < MyStruct[ i ].code_length; ++j )
		{
			cout << MyStruct[ i ].code[ j ];
		}
		cout << endl;
	}

}
