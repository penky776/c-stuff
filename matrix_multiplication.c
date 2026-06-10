#include <stdio.h>
#include <stdlib.h>

typedef struct{
    size_t row;
    size_t col;
} Dim;

void readMatrix(
    size_t dim1, 
    size_t dim2, 
    double mat[dim1][dim2],
    char fileName[]
) {
    FILE *fileptr = fopen(fileName, "r");
    if (fileptr == NULL) {
        printf("File doesnt exist.\n");
    }

    for (int r = 0; r < dim1; r++) {
        for (int c = 0; c < dim2; c++) {
            fscanf(fileptr, "%lf", &mat[r][c]);
        }
    }
}

void matPrint(
    Dim dim, 
    double mat[][dim.col], 
    char fmt[]
) {
    size_t dim1 = dim.row; 
    size_t dim2 = dim.col;

    for (int r = 0; r < dim1; r++) {
        for (int c = 0; c < dim2; c++) {
            printf(fmt, mat[r][c]);
        }
        printf("\n");
    }    

}

void matMply(
    Dim dim1, 
    Dim dim2, 
    double mat1[dim1.row][dim1.col],
    double mat2[dim2.row][dim2.col], 
    double outMat[dim1.row][dim2.col]
) {
    size_t out_r = dim1.row;
    size_t out_c = dim2.col;

    for (size_t r = 0; r < out_r; r++) {
        for (size_t c = 0; c < out_c; c++) {

            outMat[r][c] = 0.0;

            for (size_t k = 0; k < dim1.col; k++) {
                outMat[r][c] += mat1[r][k] * mat2[k][c];
            }
        }
    }   
}

int main(void) {
    Dim dim1;
    dim1.row = 4;
    dim1.col = 4;
    double matrix1[dim1.row][dim1.col];

    Dim dim2;
    dim2.row = 4;
    dim2.col = 3;
    double matrix2[dim2.row][dim2.col];
    double out_mat[dim1.row][dim2.col];

    // read matrix1
    readMatrix(dim1.row, dim1.col, matrix1, "matrix.dat");

    // read matrix2
    readMatrix(dim2.row, dim2.col, matrix2, "matrix_two.dat");

    // multiply them
    matMply(
        dim1,
        dim2,
        matrix1,
        matrix2,
        out_mat
    );

    Dim out_dim;
    out_dim.row = dim1.row;
    out_dim.col = dim2.col;

    // print out matrix
    matPrint(
        out_dim,
        out_mat,
        "%6.2lf "
    );

    return 0;
}