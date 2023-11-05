/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{

    int i, j, row, col, tmp, subMatrixSize, dia;
    /*
    * Dividing the whole matrice into sub matrices.
    * For 32x32 8x8 submatrices are taken
    * For 64x64 4x4 submatrices are taken
    * For 61x67 16x16 submatrices are taken
    */
    if (N == 64)
    {
     subMatrixSize = 4;
     //variables used for assignmnets within the 64x64 case because we have to assign 
     //elements in each row individually and are unable to modify the matrix A 
     int a0, a1, a2, a3, a4; 
     //Optimal blocksize 
     for(row = 0; row < N; row+=subMatrixSize){ 
       for(col = 0; col < M; col+=subMatrixSize){ 
  
         //store elements from A because we are unable to modify them 
         a0 = A[row][col]; 
         a1 = A[row+1][col]; 
         a2 = A[row+2][col]; 
         a3 = A[row+2][col+1]; 
         a4 = A[row+2][col+2]; 
  
         //B[column+3] 
         B[col+3][row] = A[row][col+3]; 
         B[col+3][row+1] = A[row+1][col+3]; 
         B[col+3][row+2] = A[row+2][col+3]; 
  
         //B[column+2] 
         B[col+2][row] = A[row][col+2]; 
         B[col+2][row+1] = A[row+1][col+2]; 
         B[col+2][row+2] = a4; 
         a4 = A[row+1][col+1]; 
  
         //B[column+1] 
         B[col+1][row] = A[row][col+1]; 
         B[col+1][row+1] = a4; 
         B[col+1][row+2] = a3; 
  
         //B[column] 
         B[col][row] = a0; 
         B[col][row+1] = a1; 
         B[col][row+2] = a2; 
  
         B[col][row+3] = A[row+3][col]; 
         B[col+1][row+3] = A[row+3][col+1]; 
         B[col+2][row+3] = A[row+3][col+2]; 
         a0 = A[row+3][col+3]; 
  
         B[col+3][row+3] = a0; 
       } 
     }
        return;
    }
    else
    {
        if (N == 32)
            subMatrixSize = 8;
        else
            subMatrixSize = 16;
    }

    // acessing sub-matrices in column-major order
    for (j = 0; j < M; j += subMatrixSize)
    { // j controls the sub-matrix column wise starting point
        for (i = 0; i < N; i += subMatrixSize)
        { // j controls the sub-matric row wise starting point

            // accessing elements in row-major order in each sub-matrice
            for (row = i; row < i + subMatrixSize && row < N; row++)
            { // row takes the lead from i, and takes 8 successive rows within max row limit
                for (col = j; col < j + subMatrixSize && col < M; col++)
                { // col takes the lead from j, and takes 8 successive coloumns with max col limit
                
                    // doing this op if it's not diagonal element
                    if (row != col)
                    {
                        B[col][row] = A[row][col]; // sub-matrix transpose, storing it to B as we can't change A
                    }
                    else
                    {
                        tmp = A[row][col]; // if its a diagonal element, we're storing it to temp such that it doesn't unnecessarily cause cache eviction of B
                        dia = row;         // storing the row number for which the diagonal element is temporarily stored
                    }
                }
                // for each row there will be only one diagonal element
                // and diagonal element comes with starting element of diagonal sub-matrices
                if (i == j)
                {
                    B[dia][dia] = tmp;
                }
            }
        }
    }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
