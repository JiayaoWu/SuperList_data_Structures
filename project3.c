
/* 
 * Project 3 template: to be modified by students to incorporate 
 *                     command line arguments exclusively 
 *                     (no input while program is running)
 * Jiayao Wu
 */

#include <stdio.h>
#include <stdlib.h>
#include "sl_repository.h"
#include <string.h>

unsigned long int next =1;

static int prob;
static int num_op;
static int Max_range;
static unsigned int s;

void Get_next_op( char *random_op, int *random_int, int range );
int  rand( void ); 
void srand( unsigned int seed );
void    Usage( int argc, char *argv[] );

int main(int argc, char *argv[])
{
    Usage( argc, argv );
    int    rand_key; 
    char   rand_op;
    int    ret; 
    int           ret_data;
    int    i;
    unsigned int sum_data;
    int num_get, num_delete, num_update;
   

    printf("\nGenerating %d operations with range %d and random seed %d \n", num_op, Max_range, s);
    printf("-------------------------------------------------------------------------\n");

    srand( s );

    sum_data   = 0;

    num_get    = 0;
    num_delete = 0;
    num_update = 0;

    Repository_init(prob);

    for( i = 1; i <= num_op; i++ )
    {
        Get_next_op( &rand_op, &rand_key, Max_range );

        /* printf("%c %8d %d\n", rand_op, rand_key, Max_range ); */

        switch( rand_op )
        {
            case 'G':
                num_get++;
                ret = Repository_get( rand_key, &ret_data );
                if( ret < 0 ) {
                    printf("\nError in Get with return value %d \n", ret );
                }
                if( ret == 1 ) sum_data = sum_data + (ret_data%100);
                break;

            case 'U':
                num_update++;
                ret = Repository_update( rand_key, i );
                if( ret < 0 ) {
                    printf("\nError in Update with return value %d \n", ret );
                }
                break;

            case 'D':
                num_delete++;
                ret = Repository_delete( rand_key );
                if( ret < 0 ) {
                    printf("\nError in Delete with return value %d \n", ret );
                }
                break;

            default:
                printf("\nError!!!, Illegal operation %c\n", rand_op );
                return 0;
        }

        if( (i % (num_op/10)) == 0 )
        {
            printf("Printing after %d operations\n", i );
            if( Max_range > 100 ) Repository_print( 0 );
            else if (num_op <= 10000) {
		Repository_print(2);
            } else
		Repository_print( 1 );
        }
    }
    printf("Operation summary: get %d, delete %d, update %d\n", num_get, num_delete, num_update );
    printf("Sum of all Repository_get data modulo 100 is %d\n", sum_data );
}

/*in order to the command line arguments */
void    Usage(int argc, char *argv[]) {
    /*default values*/
    Max_range = 100;
    num_op = 1000000;
    prob = 50;
    s = 1;

    /* first argument is the name of the executable */
    while( --argc > 0 )
    {
        argv++;

        if( !strncmp( *argv, "-r", 2 ) )
        {
            sscanf(argv[1], "%d", &Max_range );
            argc--; argv++;
        }else if( !strncmp( *argv, "-o", 2 ) ){
            sscanf(argv[1], "%d", &num_op );
            argc--; argv++;
        }else if( !strncmp( *argv, "-s", 2 ) ){
            sscanf(argv[1], "%d", &s );
            argc--; argv++;
        }else if( !strncmp( *argv, "-p", 2 ) ){
            sscanf(argv[1], "%d", &prob );
            argc--; argv++;
        } else {            
            printf( "Usage: \n%s\n%s\n%s\n%s\n%s\n%s\n",
            "\t[-r <max range>] : max range, default is 100",
            "\t[-o <operation>] : with a default of 1,000,000 (for the number of opeartions)",
            "\t[-s <seed>]      :with a default of 1, for the starting seed",
            "\t[-p <probability percentage>] : with a default of 50");
            exit( -1 );
        }

    }



}

void Get_next_op( char *random_op, int *random_int, int range )
{
    int  random_number;
    int  limit;

    random_number = rand() % 4;

    switch( random_number )
    {
        case 0:
        case 1:
                *random_op = 'G';
                break;
        case 2:
                *random_op = 'D';
                break;
        case 3:
                *random_op = 'U';
    }

    limit = (32768 / range ) * range;
    while( ( random_number = rand() ) >= limit ) ;

    *random_int = random_number % range + 1;
}

/* Pseudo random generator on 0..32767 */
int  rand(void)
{
    next = next *1103515245 + 12345;
    return (unsigned int)(next/65536)%32768;
}

void srand( unsigned int seed )
{
    next = seed;
}

