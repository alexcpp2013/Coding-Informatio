#include "func.h"

int main( void )
{

	FILE *fp;
  	unsigned char buf[ CNT ];

  	clock_t t0, t1;

  	t0 = clock();

	printf( "\n���뢠�� 䠩�." );
	if( ( fp = fopen( "test.txt", "rb" ) ) == NULL )
	{

		perror( "\n\a�訡�� �� ����⨨ 䠩��! ��� ��室� ������ ����.\n" );
		getchar( );
	    exit(1);

	}
	else
	{

		printf("\n���� �����.");
		int count = 0; /* ������⢮ ��⠭�� ᨬ����� */

		printf( "\n  ��稭��� ���뢠��� 䠩��." );

		int i = 0;
		for( i = 0; !feof( fp ); ++i )
		{
			count = fread( buf, sizeof( char ), CNT, fp );
			printf( "\n    ��⠫� ���� ����� %i.", i );
			if( count != CNT )
			{
				if( feof( fp ) )
				{
					printf( "\n      ����� 䠩��." );
				}
				else
				{
					perror( "\n\a  �訡�� �� �⥭�� 䠩��! ��� ��室� ������ ����.\n" );
					fclose( fp );
					getchar( );
					exit( 1 );
				}
			}
			printf( "\n      ������塞 ⠡����." );
			fill_table( count, buf ); /* ������塞 ��䠢�� */
			count = 0;
		} /* for */

		printf( "\n      ������� ���������." );
		printf( "\n  ���� ��������� ��⠭." );
		fclose( fp );
		printf( "\n���� ������." );

		int h = 0;
		for( i = 0; i < CA; ++i )
		{
			if( MyStruct.numb_symb[ i ] != 0 )
			{
				h = 1;
				break;
			}
		}
		if( h )
		{
			printf( "\n\n����᪠�� ������.");
			alg( );
			printf( "\n\n������ ����祭." );
		}
		else
		{
			perror( "\n\a���� ����!\n" );
			getchar( );
			exit( 1 );
		}

	} /* else */

	t1 = clock();

	printf("\n\n�뫮 ����祭� �६���: %g ᥪ㭤�.\n", (double) (t1 - t0) / (double) CLOCKS_PER_SEC);
	printf( "\n��� ��室� ������ ����." );
	printf( "\n" );
	getchar( );
	return 0;

} /* main */
