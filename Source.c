#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define FN "db.txt"
#define SIZE 100

struct c_line {
	const char* name;
	const char* author;
	int year;
	const char* genre;
	const char* date;
};

typedef struct c_line line;

void init( line** data, const char* file );
int add( line** data, char* name, char* author, int year, char* genre, char* date );
void Delete( line** data, char* name, char* author, int year, char* genre, char* date );
void edit( line** data, int id, char* name, char* author, int year, char* genre, char* date, char* new_name, char* new_author, int new_year, char* new_genre, char* new_date );
void display( line* data );
int compare_by_year( const void* a, const void* b );
int compare_by_date( const void* a, const void* b );
void find( line* data, const char* name, const char* author, int year, const char* genre, const char* date );

char* build_field( const char* field, int max_field );

char* build_line( const char* id, const char* name, const char* author, const char* year, const char* genre, const char* date,
	int max_id, int max_name, int max_author, int max_year, int max_genre, int max_date );

void input_args(int* id, char* name, char* author, int* year, char* genre, char* date);

void main( ) {
	setlocale( 0, "ru" );
	system( "chcp 1251" );
	system( "cls" );

	line* data;
	data = ( line* )malloc( SIZE * sizeof( line ) );

	for ( int i = 0; i < SIZE; ++i ) {
		data[i].name = NULL;
		data[i].author = NULL;
		data[i].year = 0;
		data[i].genre = NULL;
		data[i].date = NULL;
	}

	init( &data, FN );

	int should_exit = 0;

	while ( TRUE ) {
		if ( should_exit ) break;

		printf( "Тема: Разработка файловой базы данных \"Библиотека\"\n\
Разработал: Кудрявцев Ю. А.\n\
Программа предназначена для хранения записей о книгах. Записи состоят из следующих полей:\n\
1. Название книги\n\
2. Автор книги\n\
3. Год издания\n\
4. Жанр книги\n\
5. Дата выдачи читателю\n\
\n\
Для обработки базы данных пользователь может использовать 6 доступных команд:\n\
Добавление, удаление, изменение, вывод, сортировку, поиск записей.\n\n" );

		display( data );

		printf( "\nВыберите действие, которое вы хотите выполнить:\n\
1. Добавить запись в базу данных\n\
2. Удалить записи из базы данных\n\
3. Изменить данные в полях\n\
4. Вывести все записи\n\
5. Отсортировать\n\
6. Поиск по заданным параметрам\n\
7. Выход из программы\n\
Для выбора действия введите соответствующую цифру: " );

		int action;
		scanf( "%d", &action );

		switch ( action ) {
		case 1: {
			char name[50];
			char author[50];
			int year;
			char genre[30];
			char date[20];

			input_args( 0, name, author, &year, genre, date );

			if ( add( &data, name, author, year, genre, date ) ) {
				system( "cls" );

				printf( "Строка успешно добавлена.\n\n" );
			} else {
				system( "cls" );

				printf( "Ошибка при открытии файла.\n\n" );
			}

		} break;
		case 2: {
			int id;
			char name[50];
			char author[50];
			int year;
			char genre[30];
			char date[20];

			input_args( &id, name, author, &year, genre, date );
			Delete( &data, id, name, author, year, genre, date );

			system( "cls" );

			printf( "Найденные записи были удалены.\n\n" );
		} break;
		case 3: {
			int id;
			char name[50];
			char author[50];
			int year;
			char genre[30];
			char date[20];

			printf( "Введите данные полей, которые нужно изменить.\n" );

			input_args( &id, name, author, &year, genre, date );

			char new_name[50];
			char new_author[50];
			int new_year;
			char new_genre[30];
			char new_date[20];

			printf( "\nТеперь введите поля, которые будут заменены.\n" );

			input_args( 0, new_name, new_author, &new_year, new_genre, new_date );
			edit( &data, id, name, author, year, genre, date, new_name, new_author, new_year, new_genre, new_date );

			system( "cls" );

			printf( "Изменения успешно выполнены.\n\n" );
		} break;
		case 4: {
			system( "cls" );

			display( data );

			printf( "Нажмите Enter для перехода в главное меню." );
			getchar( );
			getchar( );
			system( "cls" );
		} break;
		case 5: {
			system( "cls" );

			printf( "Отсортировать по:\n1. Году выпуска\n2. Дате выдачи\nВведите вариант: " );

			int v;
			scanf( "%d", &v );

			if ( v == 1 ) {
				qsort( data, SIZE, sizeof( line ), compare_by_year );
				system( "cls" );
				printf( "Данные отсортированы по году выпуска.\n\n" );
			}
			else if ( v == 2 ) {
				qsort( data, SIZE, sizeof( line ), compare_by_date );
				system( "cls" );
				printf( "Данные отсортированы по дате выдачи.\n\n" );
			}
		} break;
		case 6: {
			char name[50];
			char author[50];
			int year;
			char genre[30];
			char date[20];

			input_args( 0, name, author, &year, genre, date );
			find( data, name, author, year, genre, date );
		} break;
		case 7: {
			should_exit = 1;
		} break;
		}
	}

	for ( int i = 0; i < SIZE; i++ ) {
        free( data[i].name );
        free( data[i].author );
        free( data[i].genre );
        free( data[i].date );
    }
    free( data );

	exit( 0 );
}

int compare_by_year( const void* a, const void* b ) {
	const line *dataA = ( const line* )a;
	const line *dataB = ( const line* )b;

	int yearA = dataA->year;
	int yearB = dataB->year;

	if ( yearA == NULL || yearB == NULL ) {
		return -1;
	}

	if ( yearA < yearB ) return -1;
	else if ( yearA > yearB ) return 1;

	return 0;
}

int compare_by_date(const void* a, const void* b) {
    const line *dataA = (const line *)a;
    const line *dataB = (const line *)b;

	if ( dataA->date == NULL || dataB->date == NULL ) {
		return -1;
	}

    return strcmp( dataA->date, dataB->date );
}

void init( line** data, const char* file ) {
	FILE* fp = fopen( file, "r" );
    if ( !fp ) {
        fp = fopen( file, "w" );
    }

	int index = 0;
	while ( !feof( fp ) ) {
		char name[50], author[50], genre[30], date[20];
        int year;

		if ( fscanf( fp, "%49[^,],%49[^,],%d,%29[^,],%19s", name, author, &year, genre, date ) == 5 ) {
			( *data )[index].name = ( char* )malloc( ( strlen( name ) + 1 ) * sizeof( char ) );
			strcpy( ( *data )[index].name, name );

			( *data )[index].author = ( char* )malloc( ( strlen( author ) + 1 ) * sizeof( char ) );
			strcpy( ( *data )[index].author, author );

			( *data )[index].year = year;

			( *data )[index].genre = ( char* )malloc( ( strlen( genre ) + 1 ) * sizeof( char ) );
			strcpy( ( *data )[index].genre, genre );

			( *data )[index].date = ( char* )malloc( ( strlen( date ) + 1 ) * sizeof( char ) );
			strcpy( ( *data )[index].date, date );

			index++;
		}
	}

	fclose( fp );
}

int add( line** data, char* name, char* author, int year, char* genre, char* date ) {
	FILE* fp = fopen( FN, "a" );

	if ( !fp ) {
		return 0;
	}

	for ( int i = 0; i < SIZE; ++i ) {
		if ( ( *data )[i].name == NULL ) {
			( *data )[i].name = ( char* )malloc( ( strlen( name ) + 1 ) * sizeof( char ) );
			strcpy( ( *data )[i].name, name );

			( *data )[i].author = ( char* )malloc( ( strlen( author ) + 1 ) * sizeof( char ) );
			strcpy( ( *data )[i].author, author );

			( *data )[i].year = year;

			( *data )[i].genre = ( char* )malloc( ( strlen( genre ) + 1 ) * sizeof( char ) );
			strcpy( ( *data )[i].genre, genre );

			( *data )[i].date = ( char* )malloc( ( strlen( date ) + 1 ) * sizeof( char ) );
			strcpy( ( char* )( *data )[i].date, date );

			break;
		}
	}

	fprintf( fp, "%s,%s,%d,%s,%s\n", name, author, year, genre, date );

	fclose( fp );

	return 1;
}

void edit( line** data, int id, char* name, char* author, int year, char* genre, char* date, char* new_name, char* new_author, int new_year, char* new_genre, char* new_date ) {
	FILE* fp = fopen( FN, "r" );
	FILE* temp = fopen( "temp.txt", "w" );

	while ( !feof( fp ) ) {
		char _name[50], _author[50], _genre[30], _date[20];
		int _year;

		if ( fscanf( fp, "%49[^,],%49[^,],%d,%29[^,],%19s", _name, _author, &_year, _genre, _date ) == 5 ) {
			if ( 
				( strlen( name ) == 1 || strstr( _name, name ) ) &&
				( strlen( author ) == 1 || strstr( _author, author ) ) &&
				( year == 0 || _year == year ) &&
				( id == 0 || strstr( _name, ( *data )[id - 1].name ) ) &&
				( strlen( genre ) == 1 || strstr( _genre, genre ) ) &&
				( strlen( date ) == 1 || strstr( _date, date ) )
			) {
				for ( int i = 0; i < SIZE; ++i ) {
					if ( ( *data )[i].name != NULL ) {
						if ( 
							( strlen( name ) == 1 || strstr( ( *data )[i].name, name ) ) &&
							( strlen( author ) == 1 || strstr( ( *data )[i].author, author ) ) &&
							( year == 0 || ( *data )[i].year == year ) &&
							( id == 0 || ( i + 1 ) == id ) &&
							( strlen( genre ) == 1 || strstr( ( *data )[i].genre, genre ) ) &&
							( strlen( date ) == 1 || strstr( ( *data )[i].date, date ) )
						) {
							strcpy( ( *data )[i].name, strlen( new_name ) == 1 ? _name : new_name );
							strcpy( ( *data )[i].author, strlen( new_author ) == 1 ? _author : new_author );
							strcpy( ( *data )[i].genre, strlen( new_genre ) == 1 ? _genre : new_genre );
							strcpy( ( *data )[i].date, strlen( new_date ) == 1 ? _date : new_date );
							( *data )[i].year = new_year == 0 ? _year : new_year;

							fprintf( temp, "%s,%s,%d,%s,%s\n", strlen( new_name ) == 1 ? _name : new_name, strlen( new_author ) == 1 ? _author : new_author, new_year == 0 ? _year : new_year, strlen( new_genre ) == 1 ? _genre : new_genre, strlen( new_date ) == 1 ? _date : new_date );
						}
					}
				}
			} else {
				fprintf( temp, "%s,%s,%d,%s,%s\n", strtok( _name, "\n" ), _author, _year, _genre, _date );
			}
		}
	}

	fclose( fp );
	fclose( temp );

	remove( FN );
	rename( "temp.txt", FN );
}

void Delete( line** data, int id, char* name, char* author, int year, char* genre, char* date ) {
	FILE* fp = fopen( FN, "r" );
	FILE* temp = fopen( "temp.txt", "w" );

	while ( !feof( fp ) ) {
		char _name[50], _author[50], _genre[30], _date[20];
        int _year;

		if ( fscanf( fp, "%49[^,],%49[^,],%d,%29[^,],%19s", _name, _author, &_year, _genre, _date ) == 5 ) {
			if ( 
				( strlen( name ) == 1 || strstr( _name, name ) ) &&
				( strlen( author ) == 1 || strstr( _author, author ) ) &&
				( year == 0 || _year == year ) &&
				( id == 0 || strstr( _name, ( *data )[id - 1].name ) ) &&
				( strlen( genre ) == 1 || strstr( _genre, genre ) ) &&
				( strlen( date ) == 1 || strstr( _date, date ) )
			) {
				
			} else {
				fprintf( temp, "%s,%s,%d,%s,%s\n", strtok( _name, "\n" ), _author, _year, _genre, _date );
			}
		}
	}

	for ( int i = 0; i < SIZE; ++i ) {
		if ( ( *data )[i].name != NULL ) {
			if ( 
				( strlen( name ) == 1 || strstr( ( *data )[i].name, name ) ) &&
				( strlen( author ) == 1 || strstr( ( *data )[i].author, author ) ) &&
				( year == 0 || ( *data )[i].year == year ) &&
				( id == 0 || ( i + 1 ) == id ) &&
				( strlen( genre ) == 1 || strstr( ( *data )[i].genre, genre ) ) &&
				( strlen( date ) == 1 || strstr( ( *data )[i].date, date ) )
			) {
				( *data )[i].name = NULL;
				( *data )[i].author = NULL;
				( *data )[i].year = 0;
				( *data )[i].genre = NULL;
				( *data )[i].date = NULL;
			}
		}
	}

	fclose( fp );
	fclose( temp );

	remove( FN );
	rename( "temp.txt", FN );
}

void display( line* data ) {
	int max_id = strlen( "Айди" ), max_name = strlen( "Название" ), max_author = strlen( "Автор" ), max_year = strlen( "Год" ), max_genre = strlen( "Жанр" ), max_date = strlen( "Дата выдачи" );

	for ( int i = 0; i < SIZE; ++i ) {
		if ( data[i].name != NULL ) {
			max_id = max( max_id, max( ( int )log10( abs( i + 1 ) ) + 1, strlen( "Айди" ) ) );
			max_name = max( max_name, max( strlen( data[i].name ), strlen( "Название" ) ) );
			max_author = max( max_author, max( strlen( data[i].author ), strlen( "Автор" ) ) );
			max_genre = max( max_genre, max( strlen( data[i].genre ), strlen( "Жанр" ) ) );
			max_date = max( max_date, max( strlen( data[i].date ), strlen( "Дата выдачи" ) ) );
			max_year = max( max_year, max( ( int )log10( abs( data[i].year ) ) + 1, strlen( "Год" ) ) );
		}
	}

	int total_length = max_id + max_name + max_author + max_genre + max_date + max_year + 19;
	char* separator = ( char* )malloc( ( total_length + 1 ) * sizeof( char ) );
	char* title = ( char* )malloc( ( total_length + 1 ) * sizeof( char ) );

	strcpy( title, build_line( "Айди", "Название", "Автор", "Год", "Жанр", "Дата выдачи", max_id, max_name, max_author, max_year, max_genre, max_date ) );

	memset( separator, '-', total_length );
	separator[total_length] = '\0';
	
	printf( "%s\n", separator );
	printf( "%s\n", title );
	printf( "%s\n", separator );

	for ( int i = 0; i < SIZE; ++i ) {
		if ( data[i].name != NULL ) {
			char* year_str = ( char* )malloc( 20 * sizeof( char ) );
			char* id_str = ( char* )malloc( 20 * sizeof( char ) );
			char* result = ( char* )malloc( ( total_length + 1 ) * sizeof( char ) );
			sprintf( year_str, "%d", data[i].year );
			sprintf( id_str, "%d", i + 1 );

			strcpy( result, build_line( id_str, strtok( data[i].name, "\n" ), data[i].author, year_str, data[i].genre, data[i].date, max_id, max_name, max_author, max_year, max_genre, max_date ) );

			printf( "%s\n%s\n", result, separator );
		}
	}
}

char* build_field( const char* field, int max_field ) {
	char* result = ( char* )malloc( ( max_field + 1 ) * sizeof( char ) );

	int index = 0;
	memset( result + index, '|', 1 );
	memset( result + index + 1, ' ', 1 );
	index += 2;

	strcpy( result + index, field );
	index += strlen( field );

	memset( result + index, ' ', max_field - strlen( field ) + 1 );

	return result;
}

char* build_line( const char* id, const char* name, const char* author, const char* year, const char* genre, const char* date,
	int max_id, int max_name, int max_author, int max_year, int max_genre, int max_date ) {
	
	int total_length = max_id + max_name + max_author + max_genre + max_date + max_year + 19;
	char* result = ( char* )malloc( ( total_length + 1 ) * sizeof( char ) );

	strcpy( result, build_field( id, max_id ) );
	strcpy( result + max_id + 3, build_field( name, max_name ) );
	strcpy( result + max_id + max_name + 6, build_field( author, max_author ) );
	strcpy( result + max_id + max_name + max_author + 9, build_field( year, max_year ) );
	strcpy( result + max_id + max_name + max_author + max_year + 12, build_field( genre, max_genre ) );
	strcpy( result + max_id + max_name + max_author + max_year + max_genre + 15, build_field( date, max_date ) );
	memset( result + max_id + max_name + max_author + max_year + max_genre + 18 + max_date, '|', 1 );
	result[total_length] = '\0';

	return result;
}

void find( line* data, const char* name, const char* author, int year, const char* genre, const char* date ) {
	system( "cls" );

	int max_id = 0, max_name = 0, max_author = 0, max_year = 0, max_genre = 0, max_date = 0;

	for ( int i = 0; i < SIZE; ++i ) {
		if ( data[i].name != NULL && (
			( strlen( name ) == 1 || strstr( data[i].name, name ) ) &&
			( strlen( author ) == 1 || strstr( data[i].author, author ) ) &&
			( year == 0 || data[i].year == year ) &&
			( strlen( genre ) == 1 || strstr( data[i].genre, genre ) ) &&
			( strlen( date ) == 1 || strstr( data[i].date, date ) )
			) ) {
			
			max_id = max( max_id, max( ( int )log10( abs( i + 1 ) ) + 1, strlen( "Айди" ) ) );
			max_name = max( max_name, max( strlen( data[i].name ), strlen( "Название" ) ) );
			max_author = max( max_author, max( strlen( data[i].author ), strlen( "Автор" ) ) );
			max_genre = max( max_genre, max( strlen( data[i].genre ), strlen( "Жанр" ) ) );
			max_date = max( max_date, max( strlen( data[i].date ), strlen( "Дата выдачи" ) ) );
			max_year = max( max_year, max( ( int )log10( abs( data[i].year ) ) + 1, strlen( "Год" ) ) );
		}
	}

	int total_length = max_id + max_name + max_author + max_genre + max_date + max_year + 19;
	char* separator = ( char* )malloc( ( total_length + 1 ) * sizeof( char ) );
	char* title = ( char* )malloc( ( total_length + 1 ) * sizeof( char ) );

	strcpy( title, build_line( "Айди", "Название", "Автор", "Год", "Жанр", "Дата выдачи", max_id, max_name, max_author, max_year, max_genre, max_date ) );

	memset( separator, '-', total_length );
	separator[total_length] = '\0';
	
	printf( "%s\n", separator );
	printf( "%s\n", title );
	printf( "%s\n", separator );

	for ( int i = 0; i < SIZE; ++i ) {
		if ( data[i].name != NULL && (
			( strlen( name ) == 1 || strstr( data[i].name, name ) ) &&
			( strlen( author ) == 1 || strstr( data[i].author, author ) ) &&
			( year == 0 || data[i].year == year ) &&
			( strlen( genre ) == 1 || strstr( data[i].genre, genre ) ) &&
			( strlen( date ) == 1 || strstr( data[i].date, date ) )
			) ) {
			
			char* year_str = ( char* )malloc( 20 * sizeof( char ) );
			char* id_str = ( char* )malloc( 20 * sizeof( char ) );
			char* result = ( char* )malloc( ( total_length + 1 ) * sizeof( char ) );
			sprintf( year_str, "%d", data[i].year );
			sprintf( id_str, "%d", i + 1 );

			strcpy( result, build_line( id_str, strtok( data[i].name, "\n" ), data[i].author, year_str, data[i].genre, data[i].date, max_id, max_name, max_author, max_year, max_genre, max_date ) );

			printf( "%s\n%s\n", result, separator );
		}
	}

	printf( "\nНажмите Enter для перехода в главное меню." );
	getchar( );
	getchar( );
	system( "cls" );
}

void input_args(int* id, char* name, char* author, int* year, char* genre, char* date) {
	printf( "\nВпишите 0, если не хотите, чтобы поле учитывалось.\n" );

	if ( id ) {
		printf( "Айди: " );
		scanf( "%d", id );
	}
	
	printf( "Название: " );
	scanf( "%s", name );

	printf( "Автор: " );
	scanf( "%s", author );

	printf( "Год: " );
	scanf( "%d", year );

	printf( "Жанр: " );
	scanf( "%s", genre );

	printf( "Дата выдачи: " );
	scanf( "%s", date );
}