/* Programa de multiplicación de matrices uniprocesador
   2001, Daniel Manrique. */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


// rellena una matriz aleatoriamente
void randomatrix(
    int *matrix,
    int row,
    int col,
    int maxval
){
    int i;
    for (i = 0; i < (row * col); i++) {
        *(matrix + i) = 1 + (int) ((float) maxval * random() / (RAND_MAX + 1.0));
        //      printf ("elemento %d es
        //      %d\n",i,*(matrix+i));
    }

}

// dado un arreglo lineal lo trata como matriz y
// regresa el elemento especificado. row,col son
// iniciando en 0 es decir si quieres el primer
// elemento es 0,0 especificamos x, y
int matrix_get_cell(
    int *matrix,
    int rows,
    int cols,
    int x,
    int y
){
    int valor_lineal;
    valor_lineal = (y * cols + x);
    return matrix[valor_lineal];
}

// similar pero fija el valor del elemento dado en la
// matriz;
int matrix_set_cell(
    int *matrix,
    int rows,
    int cols,
    int x,
    int y,
    int val
){
    int valor_lineal;
    valor_lineal = (y * cols + x);
    matrix[valor_lineal] = val;
    return 0;
}


// presentacion de matrices
int matrix_print_linear(
    int *matrix,
    int rows,
    int cols
){
    int x, y;
    for (x = 0; x < cols; x++) {
        for (y = 0; y < rows; y++) {
            printf(
                "%d,%d es %d\n",
                x,
                y,
                matrix_get_cell(matrix, rows, cols, x, y)
            );
        }
    }
    return 0;
}

int matrix_print(int *matrix, int rows, int cols)
{
    int x, y;
    for (y = 0; y < rows; y++) {
        for (x = 0; x < cols; x++) {
            printf(
                "%7d ",
                matrix_get_cell(matrix, rows, cols, x, y)
            );
        }
        printf("\n");
    }
    return 0;
}

int main(int argc, char **argv)
{
    int rv, row, col;
    int *matrix1=NULL;
    int *matrix2=NULL;
    int *matrix3=NULL;
    int x, y, i, k;
    int elem1, elem2, suma;
    int dimension;
    int seed = (unsigned int) (time(0) / 2);
    char optchar;
    struct timeval starttime, endtime;
    double dstart, dend;
    int opt_print = 0;


    // Este ciclo procesa las opciones de la línea de
    // comandos y fija las banderas necesarias del
    // programa.

    do {
	optchar = getopt(argc, argv, "fpd:");
	switch (optchar) {
	case 'p':
	    printf("imprimiendo matrices\n");
	    opt_print = 1;
	    break;
	case 'd':
	    dimension = atoi(optarg);
	    printf("dimension %d\n", dimension);
	    break;
	case 'f':
	    printf("matriz fija\n");
	    seed = 1;
	    break;
	}
    }
    while (optchar != -1);

    if (dimension == 0) {
	printf("dimension 0, pos no funcionara\n");
	return 0;
    }
    row = dimension;
    col = dimension;


    srandom(seed);

    /* Asignar memoria para tres matrices de de row x
       col */
    matrix1 = malloc(row * col * sizeof(int));
    matrix2 = malloc(row * col * sizeof(int));
    matrix3 = malloc(row * col * sizeof(int));
	if (matrix1 == NULL || matrix2 == NULL ||
	    matrix3 == NULL ){
	    if (matrix1!=NULL) free(matrix1);
	    if (matrix2!=NULL) free(matrix2);
	    if (matrix3!=NULL) free(matrix3);
	    exit(1);
	}

    /* llenamos las dos primeras matrices
       aleatoriamente..  la definicion de la funcion
       randomatrix esta en el archivo matrix.c */
    randomatrix(matrix1, row, col, 5);
    randomatrix(matrix2, row, col, 5);

    // obtener inicio
    rv = gettimeofday(&starttime, NULL);

    // mostrar matrices..
    if (opt_print) {
	matrix_print(matrix1, row, col);
	printf("\n");
	matrix_print(matrix2, row, col);
    }
    // Ir calculando renglon por renglon
    for (i = 0; i < row; i++) {
	printf("calculado row %d\n", i);
	/* el elemento que estoy calculando esta en y,x
	   entonces lo que va a variar va a ser la x
	   porque la y es fija por renglon */
	y = i;
	// aqui calculamos cada celda
	for (x = 0; x < col; x++) {
	    //      printf ("calculando elemento
	    //      (%d,%d)\n",x,y);
	    suma = 0;
	    for (k = 0; k < col; k++) {
		elem1 =
		    matrix_get_cell(matrix1, row, col,
				    k, y);
		elem2 =
		    matrix_get_cell(matrix2, row, col,
				    x, k);
		suma += elem1 * elem2;
		//          printf ("%d*%d +
		//          ",elem1,elem2);
	    }
	    matrix_set_cell(matrix3, row, col, x, i,
			    suma);
	}
    }
    // Terminamos el calculo, obtener tiempo...
    rv = gettimeofday(&endtime, NULL);

    // Mostramos matrices..
    if (opt_print) {
	matrix_print(matrix3, row, col);
    }
    // Calculamos el tiempo que tardamos, mostramos y
    // listo
    dend = (double) endtime.tv_sec;
    dend += (double) endtime.tv_usec * 0.000001;

    dstart = (double) starttime.tv_sec;
    dend += (double) starttime.tv_usec * 0.000001;

    printf("wall clock time = %f\n", dend - dstart);
    if (matrix1!=NULL) free(matrix1);
    if (matrix2!=NULL) free(matrix2);
    if (matrix3!=NULL) free(matrix3);
    return 0;
}
