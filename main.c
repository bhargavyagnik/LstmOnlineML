#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lstm.h"
#include "set.h"
#include "layers.h"
#include "utilities.h"

#define ITERATIONS 	1000000

int main(int argc, char *argv[])
{
	int i = 0, c;
	size_t file_size = 0, sz = 0;
	int *X_train, *Y_train;
	FILE * fp;
	set_T set;
	lstm_model_t * model;

	srand( time ( NULL ) );

	if ( argc < 2 ) {
		printf("Usage: ./binary datafile [-r name_of_net_to_load]\n");
		return -1;
	}

	initialize_set(&set);

	fp = fopen(argv[1], "r");
	while ( ( c = fgetc(fp) ) != EOF ) {
		set_insert_symbol(&set, (char)c );
		++file_size;
	}
	set_insert_symbol(&set, '.');
	fclose(fp);

	X_train = calloc(file_size+1, sizeof(int));
	if ( X_train == NULL )
		return -1;

	X_train[file_size] = (int) set_char_to_indx(&set,'.');

	Y_train = &X_train[1];

	fp = fopen(argv[1], "r");
	while ( ( c = fgetc(fp) ) != EOF ) 
		X_train[sz++] = set_char_to_indx(&set,c);
	fclose(fp);

	lstm_init_model(set_get_features(&set), NEURONS, &model, YES_FILL_IT_WITH_A_BUNCH_OF_RANDOM_NUMBER_PLEASE);

	if ( argc == 4 && !strcmp(argv[2], "-r") ) {
		lstm_read_net(model, argv[3]);
	}

	lstm_train_the_next(model, &set, file_size, X_train, Y_train, ITERATIONS);

	free(X_train);

	return 0;
}
