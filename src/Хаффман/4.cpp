// Метод Хаффмана
// Лабораторная работа по "ТИК"
// Мельник О. В.

#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef unsigned long ul;

const int ALF = 256;
const int HuffCodLen = 255;
const int RootLen = 1;
const int BYTE = 8;

ul table[ ALF ];
ul fsize, new_fsize;
int tsize, t_count;
const int BSIZE = 4096;
clock_t t1, t2;
ifstream source;
char * fille_name = "test";
char * new_fille_name = "test.nnm";
char * old_fille_name = "test_new";

struct Table
{
	int sym;
	ul prb;
	int code[ ALF ];
	int code_len;
	Table * left;
	Table * right;
} *huff_tree[ ALF + HuffCodLen + RootLen ], *root = 0;

struct Code
{
	unsigned char sym;
	unsigned long prb;
	int code[ ALF ];
	int code_len;
} *code_table;

void init( );
void read_file( );
void read_encode_file( );
void init_table( );
void sort_table( int, int );
void print_table( );
void make_tree( );
void make_code( );
void print_tree( );
void inc_code( Table * , int );
void encode( );
void decode( );
void delete_table( );

int main(int argc, char ** argv)
{

	if( argc > 1 )
	{
		if( strcmp( "e", argv[ 1 ] ) == 0 )
		{
			t1 = 0;
			t2 = 0;

			t1 = clock( );
			init( );
			read_file( );
			init_table( );
			sort_table( 0, tsize );
			make_tree( );
			make_code( );
			encode( );
			print_table( );
			delete_table( );
			t2 = clock( );
			cout << endl << "Час, що був затрачений на кодування (секунд): " << static_cast< double >( t2 - t1 ) / CLOCKS_PER_SEC << endl << endl;

		}
		if( strcmp( "d", argv[ 1 ] ) == 0 )
		{
			t1 = 0;
			t2 = 0;

			t1 = clock( );
			init( );
			read_encode_file( );
			sort_table( 0, tsize );
			make_tree( );
			make_code( );
			decode( );
			print_table( );
			delete_table( );
			t2 = clock( );
			cout << endl << "Час, що був затрачений на декодування (секунд): " << static_cast< double >( t2 - t1 ) / CLOCKS_PER_SEC << endl << endl;
		}
	}

}

void init( )
{

	fsize = 0;
	tsize = 0;
	new_fsize = 0;
	for( int i; i < ALF; ++i )
	{
		table[ i ] = 0;
	}

}

void read_file( )
{

	ifstream source( fille_name, ios::in | ios::binary );
	if( !source )
	{
		cerr << "Ошибка!\a" << endl;
		exit( 1 );
	}

	char buf[ BSIZE ];
	int n;
	for( ; ; )
	{
		source.read( buf,BSIZE );
		n = source.gcount( );
		for( int i = 0; i < n; ++i )
		{
			++table[ static_cast< unsigned char >( buf[ i ] ) ];
			++fsize;
		}
		if( n != BSIZE )
		{
			source.close( );
			break;
		}
	}

}

void read_encode_file( )
{

	source.open( new_fille_name, ios::in | ios::binary );
	if( !source )
	{
		cerr << "Ошибка!\a" << endl;
		exit( 1 );
	}

	int i;

	source >> fsize;
	source >> tsize;

	int sym;
	unsigned long prb;

	for( i = 0; i < tsize; ++i )
	{
		source >> sym >> prb;
		huff_tree[ i ] = new Table;
		huff_tree[ i ] -> sym = sym;
		huff_tree[ i ] -> prb = prb;
		huff_tree[ i ] -> left = 0;
		huff_tree[ i ] -> right = 0;
		huff_tree[ i ] -> code_len = 0;
	}
	i = source.get( );

}

void init_table( )
{

	for( int i = 0; i < ALF; ++i )
	{
		if( table[ i ] == 0 )
			continue;
		huff_tree[ tsize ] = new Table( );
		huff_tree[ tsize ] -> sym = i;
		huff_tree[ tsize ] -> prb = table[ i ];
		huff_tree[ tsize ] -> code_len = 0;
		huff_tree[ tsize ] -> left = 0;
		huff_tree[ tsize ] -> right = 0;
		++tsize;
	}

}

void sort_table( int start, int t_count )
{

	int i = start, j = start;
	Table * tmp;
	for( j; j < t_count; ++j )
	{
		for( i = 0; i < t_count - 1; ++i )
		{
			if( huff_tree[ i ] -> prb > huff_tree[ i + 1 ] -> prb )
			{
				tmp = huff_tree[ i ];
				huff_tree[ i ] = huff_tree[ i + 1 ];
				huff_tree[ i + 1 ] = tmp;
			}
		}
	}

}

void print_table( )
{

	cout << endl << "Символ\t" << "DEC\t" << "HEX\t" << "Кiлькiсть\t" << "Код" << endl << endl;
	for( int i = 0; i < tsize; ++i )
	{
		cout << endl << static_cast< unsigned char >( code_table[ i ].sym ) << "\t" << static_cast< int >( code_table[ i ].sym );
		cout << "\t" << hex << static_cast< int >( code_table[ i ].sym ) << dec << "\t";
		cout << code_table[ i ].prb << "\t" << "\t";
		for( int j = 0; j < code_table[ i ].code_len; ++j )
		{
			cout << " " << code_table[ i ].code[ j ];
		}
	}
	cout << endl << endl;

}

void make_tree()
{

	int k = 0;
	t_count = tsize;
	for( ; ; )
	{
		huff_tree[ t_count ] = new Table( );
		huff_tree[ t_count ] -> prb = huff_tree[ k ] -> prb + huff_tree[ k + 1 ] -> prb;
		huff_tree[ t_count ] -> sym = -1;
		huff_tree[ t_count ] -> right = huff_tree[ k + 1 ];
		huff_tree[ t_count ] -> left = huff_tree[ k ];
		inc_code( huff_tree[ k ], 0);
		inc_code( huff_tree[ k + 1], 1 );
		k += 2;
		++t_count;
		sort_table( k, t_count );
		if( huff_tree[ t_count - 1 ] -> prb == fsize )
			break;
	}
	root = huff_tree[ t_count - 1 ];

}

void make_code( )
{

	int k = 0, m = 0;
	code_table = new Code[ tsize ];
	for( int i = 0; i < t_count-1; ++i )
	{
		if( huff_tree[ i ] -> sym == -1 )
			continue;
		code_table[ k ].sym = huff_tree[ i ] -> sym;
		code_table[ k ].prb = huff_tree[ i ] -> prb;
		code_table[ k ].code_len = huff_tree[ i ] -> code_len;
		for( int j = huff_tree[ i ] -> code_len - 1; j > -1; --j, ++m )
		{
			code_table[ k ].code[ m ] = huff_tree[ i ] -> code[ j ];
		}
		++k;
		m = 0;
	}

}

void inc_code( Table *node, int bit )
{

	if( !node )
		return;
	Table *tmp = node;
	if( tmp -> sym != -1 )
	{
		tmp -> code[ tmp -> code_len ] = bit;
		tmp -> code_len++;
	}
	inc_code( tmp -> left, bit );
	inc_code( tmp -> right, bit );

}

void encode( )
{

	ofstream fout( new_fille_name, ios::out | ios::binary);
	fout << fsize << '\n' << tsize << '\n';
	for( int i = 0; i < tsize; ++i )
	{
		fout << static_cast< int >( code_table[ i ].sym ) << ' ' << code_table[ i ].prb << '\n';
	}
	source.open( fille_name, ios::in | ios::binary);
	char buf[ BSIZE ], outbuf[ BSIZE ];
	unsigned char byte = 0;
	int n, m = 0, i, j, k = 0, pos = 0;
	for( ; ; )
	{
		source.read( buf, BSIZE );
		n = source.gcount();
		for( i = 0; i < n; ++i )
		{
			for( j = 0; j < tsize; ++j )
				if( code_table[ j ].sym == static_cast< unsigned char >( buf[ i ] ) )
					break;
			for( ; ; )
			{
				if( pos == BYTE )
				{
					outbuf[ m ] = byte;
					++m;
					byte = 0;
					pos = 0;
				}
				if( m == BSIZE )
				{
					fout.write( outbuf, m );
					new_fsize += m;
					m = 0;
				}
				if( k == code_table[ j ].code_len )
				{
					k = 0;
					break;
				}
				byte <<= 1;
				byte |= code_table[ j ].code[ k ];
				++pos;
				++k;
			}
		}
		if( n != BSIZE )
		{
			source.close();
			break;
		}
	}
	if( m != BSIZE )
	{
		fout.write( outbuf, m );
		new_fsize +=  m;
	}
	if( pos != BYTE && pos != 0 )
	{
		for( i = pos; i < BYTE; ++i )
		{
			byte <<= 1;
			byte |= 0;
		}
		fout.put( byte );
		++new_fsize;
	}
	fout.close( );

}

void decode( )
{
	char buf[ BSIZE ], byte, outbuf[ BSIZE ];
	unsigned long fsize_count = 0;
	Table * tmp = root;
	ofstream fout( old_fille_name, ios::out | ios::binary);
	int n, i = 0, j = 0, pos = 0;
	for( ; ; )
	{
		source.read( buf, BSIZE );
		n = source.gcount( );
		for( i = 0; i < n; ++i )
		{
			byte = buf[ i ];
			for( pos = 0; pos < BYTE; ++pos )
			{
				if( !tmp )
					tmp = root;
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
				if( tmp -> sym != -1 )
				{
					outbuf[ j ] = tmp -> sym;
					++j;
					++fsize_count;
				}
				if( j == BSIZE )
				{
					fout.write( outbuf, j );
					j = 0;
				}
				if( fsize_count == fsize )
					break;
			}
		}
		if( n != BSIZE )
		{
			source.close( );
			break;
		}
	}
	if( j != BSIZE )
		fout.write( outbuf, j );
	fout.close( );

}

void delete_table()
{

	for(int i = 0; i < t_count; ++i )
		delete huff_tree[ i ];

}
