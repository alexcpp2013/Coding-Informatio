#include "func.h"

int main( void )
{

	FILE *fp;
  	unsigned char buf[ CNT ];

  	clock_t t0, t1;

  	t0 = clock();

	printf( "\nОткрываем файл." );
	if( ( fp = fopen( "test.txt", "rb" ) ) == NULL )
	{

		perror( "\n\aОшибка при открытии файла! Для выхода нажмите ВВОД.\n" );
		getchar( );
	    exit(1);

	}
	else
	{

		printf("\nФайл открыт.");
		int count = 0; /* Количество считаных символов */

		printf( "\n  Начинаем считывание файла." );

		int i = 0;
		for( i = 0; !feof( fp ); ++i )
		{
			count = fread( buf, sizeof( char ), CNT, fp );
			printf( "\n    Считали блок номер %i.", i );
			if( count != CNT )
			{
				if( feof( fp ) )
				{
					printf( "\n      Конец файла." );
				}
				else
				{
					perror( "\n\a  Ошибка при чтении файла! Для выхода нажмите ВВОД.\n" );
					fclose( fp );
					getchar( );
					exit( 1 );
				}
			}
			printf( "\n      Заполняем таблицы." );
			fill_table( count, buf ); /* Заполняем алфавит */
			count = 0;
		} /* for */

		printf( "\n      Таблицы заполнены." );
		printf( "\n  Файл полностью считан." );
		fclose( fp );
		printf( "\nФайл закрыт." );

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
			printf( "\n\nЗапускаем алгоритм.");
			alg( );
			printf( "\n\nАлгоритм окончен." );
		}
		else
		{
			perror( "\n\aФайл пуст!\n" );
			getchar( );
			exit( 1 );
		}

	} /* else */

	t1 = clock();

	printf("\n\nБыло затрачено времени: %g секунды.\n", (double) (t1 - t0) / (double) CLOCKS_PER_SEC);
	printf( "\nДля выхода нажмите ВВОД." );
	printf( "\n" );
	getchar( );
	return 0;

} /* main */
