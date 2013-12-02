// Метод Шеннона-Фано
// Лабораторная работа по "ТИК"
// Мельник О. В.

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

using namespace std;

typedef unsigned long int ul;

const int ALF = 256; // Размер алфавита
//ul TABLE_OF_CHAR[ ALF ]; // Массив с символами
ul FILE_SIZE; // Размер файла данных (нужен для восстановления)
//ul NEW_FILE_SIZE; // Новый размер (нужен для востановления)

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

} MyStruct[ ALF ]; // Временная структура и массив структур для каждого нового символа

clock_t start_time, end_time;

ifstream source;

char * fille_name = "test_new"; // Имя файла для обработки
char * new_fille_name = "test.nnm"; // Имя нового файла для обработки

struct TreeNode //используем структуру для упрощения работы чем с экземляром класса
{

	TreeNode() : data( -1 ), left( 0 ), right( 0 )
	{
		;
	}
	int data;
	TreeNode * left;
	TreeNode * right;

} *root = 0;

void READ( ); // Считывани файла
void code( int , int, ul, TreeNode ** ); // Составление кода
void decode( ); // Декодирование
inline void print( );
void delete_tree( TreeNode * );

//----------------------------------------------------------------------------------------------------------------------

int main( int argc, char * argv[ ] )
{

	start_time = clock();

	READ( );

	code( 0, REAL_TABLE_SIZE - 1, FILE_SIZE, &root );

	decode( );

	delete_tree( root );

	end_time = clock( );

	print( );

	cout << endl << "Час, що був затрачений на кодування (секунд): " << static_cast< double >( end_time - start_time ) / CLOCKS_PER_SEC << endl;

}

//----------------------------------------------------------------------------------------------------------------

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
			cout << MyStruct[ i ].code[ j ] << " ";
		}
		cout << endl;
	}

}

void READ( )
{

	source.open( new_fille_name, ios::in | ios::binary );
	if( !source )
	{
		cerr << "Ошибка при открытии контейнера! \a" << endl;
		exit( 1 );
	}

	source >> FILE_SIZE;
	source >> REAL_TABLE_SIZE;

	int sym;
	ul cnt;

	int i = 0;
	for( i = 0; i < REAL_TABLE_SIZE; ++i )
	{
		source >> sym >> cnt;
		MyStruct[ i ].symbol = static_cast<unsigned char>( sym );
		MyStruct[ i ].count = cnt;
	}
	i = source.get();

}

void code( int begin, int end, ul sum, TreeNode ** ptr )
{

	*ptr = new TreeNode( );
	if( begin == end )
	{
		( *ptr ) -> data = MyStruct[begin].symbol;
		return;
	}

	if(end == begin + 1)
	{
		MyStruct[ begin ].code[ MyStruct [ begin ].code_length ] = 1;
		++MyStruct[ begin ].code_length;
		MyStruct[ end ].code[ MyStruct[ end ].code_length ] = 0;
		++MyStruct[ end ].code_length;
		( *ptr ) -> left = new TreeNode( );
		( *ptr ) -> right = new TreeNode( );
		( *ptr ) -> left -> data = MyStruct[ end ].symbol;
		( *ptr ) -> right -> data = MyStruct[ begin ].symbol;
		return;
	}

	int m = begin, n = end;
	ul p1 = 0, p2 = 0;
	for( ; p1+p2 != sum; )
	{
		if( p1 < p2 )
		{
			p1 += MyStruct[ m ].count;
			MyStruct[ m ].code[ MyStruct[ m ].code_length ] = 1;
			++MyStruct[ m ].code_length;
			++m;
		}
		else
		{
			p2 += MyStruct[ n ].count;
			MyStruct[ n ].code[ MyStruct[ n ].code_length ] = 0;
			++MyStruct[ n ].code_length;
			--n;
		}
	}

	code( begin, m-1, p1, &( *ptr ) -> right );
	code( m, end, p2, &( *ptr ) -> left );

}

void delete_tree( TreeNode * ptr )
{

	TreeNode * tmp = ptr;
	if( tmp == 0 )
	{
		return;
	}

	delete_tree( tmp-> left );
	delete_tree( tmp-> right );
	delete tmp;

}

void decode( )
{

	char buffer[ BUFFER_SIZE ], byte, outbuffer[ BUFFER_SIZE ];
	ul fsize_count( 0 );
	TreeNode * tmp = root;
	ofstream fout( fille_name, ios::out | ios::binary );
	int n, j = 0, pos = 0;
	for( ; ; )
	{
		source.read( buffer, BUFFER_SIZE );
		n = source.gcount( );
		for( int i = 0; i < n; ++i )
		{
			byte = buffer[ i ];
			for( pos = 0; pos < BYTE; ++pos )
			{
				if( !tmp )
				{
					tmp = root;
				}
				if( byte & ( 1 << BYTE ) )
				{
					tmp = tmp -> right;
				}
				else
				{
					tmp = tmp -> left;
				}
				if( !tmp )
				{
					--pos;
					continue;
				}
				byte <<= 1;
				if( tmp -> data != -1 )
				{
					outbuffer[ j ] = tmp -> data;
					++j;
					++fsize_count;
				}
				if( j == BUFFER_SIZE )
				{
					fout.write( outbuffer, j );
					j = 0;
				}
				if( fsize_count == FILE_SIZE )
				{
					break;
				}
			}
		}
		if( n != BUFFER_SIZE )
		{
			source.close( );
			break;
		}
	}
	if( j != BUFFER_SIZE )
	{
		fout.write( outbuffer, j );
	}
	fout.close( );

}

