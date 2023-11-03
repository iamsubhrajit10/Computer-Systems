/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#define blockSize 8
/*
 * Please fill in the following team struct
 */
team_t team = {
    "23210106",              /* Team name */

    "Subhrajit Das",     /* First member full name */
    "subhrajit.das@iitgn.ac.in",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/
char optimal_rotate_descr[] = "optimal_rotate: Optimal implementation";
void optimal_rotate(int dimension, pixel *source, pixel *destination) {
    for (int i = 0; i < dimension; i += blockSize){
        for (int j = 0; j < dimension; j += blockSize){
            for (int k = i; k < i+blockSize; k++){
                destination[RIDX(dimension-1-j, k, dimension)] = source[RIDX(k, j+0, dimension)];
                destination[RIDX(dimension-2-j, k, dimension)] = source[RIDX(k, j+1, dimension)];
                destination[RIDX(dimension-3-j, k, dimension)] = source[RIDX(k, j+2, dimension)];
                destination[RIDX(dimension-4-j, k, dimension)] = source[RIDX(k, j+3, dimension)];
                destination[RIDX(dimension-5-j, k, dimension)] = source[RIDX(k, j+4, dimension)];
                destination[RIDX(dimension-6-j, k, dimension)] = source[RIDX(k, j+5, dimension)];
                destination[RIDX(dimension-7-j, k, dimension)] = source[RIDX(k, j+6, dimension)];
                destination[RIDX(dimension-8-j, k, dimension)] = source[RIDX(k, j+7, dimension)];
            }
        }
    }
}



/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dimension, pixel *source, pixel *destination)
{
    int i, j;

    for (i = 0; i < dimension; i++)
	for (j = 0; j < dimension; j++)
	    destination[RIDX(dimension-1-j, i, dimension)] = source[RIDX(i, j, dimension)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dimension, pixel *source, pixel *destination)
{
    optimal_rotate(dimension, source, destination);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
    //add_rotate_function(&optimal_rotate,optimal_rotate_descr);
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dimension, int i, int j, pixel *source)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dimension-1); ii++)
	for(jj = max(j-1, 0); jj <= min(j+1, dimension-1); jj++)
	    accumulate_sum(&sum, source[RIDX(ii, jj, dimension)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dimension, pixel *source, pixel *destination)
{
    int i, j;

    for (i = 0; i < dimension; i++)
	for (j = 0; j < dimension; j++)
	    destination[RIDX(i, j, dimension)] = avg(dimension, i, j, source);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */

static void set_top(int dimension, pixel *source, pixel *destination, int j){
    destination[j].blue = (source[j].blue+source[j+dimension].blue+source[j-1].blue+source[j+1].blue+source[j+dimension-1].blue+source[j+dimension+1].blue)/6;
    destination[j].green = (source[j].green+source[j+dimension].green+source[j-1].green+source[j+1].green+source[j+dimension-1].green+source[j+dimension+1].green)/6;
    destination[j].red = (source[j].red+source[j+dimension].red+source[j-1].red+source[j+1].red+source[j+dimension-1].red+source[j+dimension+1].red)/6;
}
static void set_bottom(int dimension, pixel *source, pixel *destination, int j){
    destination[j].blue = (source[j].blue+source[j-dimension].blue+source[j-1].blue+source[j+1].blue+source[j-dimension-1].blue+source[j-dimension+1].blue)/6;
    destination[j].green = (source[j].green+source[j-dimension].green+source[j-1].green+source[j+1].green+source[j-dimension-1].green+source[j-dimension+1].green)/6;
    destination[j].red = (source[j].red+source[j-dimension].red+source[j-1].red+source[j+1].red+source[j-dimension-1].red+source[j-dimension+1].red)/6;
}
static void set_left(int dimension, pixel *source, pixel *destination, int i){
    destination[i].blue = (source[i].blue+source[i-dimension].blue+source[i-dimension+1].blue+source[i+1].blue+source[i+dimension].blue+source[i+dimension+1].blue)/6;
    destination[i].green = (source[i].green+source[i-dimension].green+source[i-dimension+1].green+source[i+1].green+source[i+dimension].green+source[i+dimension+1].green)/6;
    destination[i].red = (source[i].red+source[i-dimension].red+source[i-dimension+1].red+source[i+1].red+source[i+dimension].red+source[i+dimension+1].red)/6;
}
static void set_right(int dimension, pixel *source, pixel *destination, int i){
    destination[i].blue = (source[i].blue+source[i-dimension].blue+source[i-dimension-1].blue+source[i-1].blue+source[i+dimension].blue+source[i+dimension-1].blue)/6;
    destination[i].green = (source[i].green+source[i-dimension].green+source[i-dimension-1].green+source[i-1].green+source[i+dimension].green+source[i+dimension-1].green)/6;
    destination[i].red = (source[i].red+source[i-dimension].red+source[i-dimension-1].red+source[i-1].red+source[i+dimension].red+source[i+dimension-1].red)/6;
}
static void set_in(int dimension, pixel *source, pixel *destination, int k){
    destination[k].blue = (source[k].blue+source[k-1].blue+source[k+1].blue+source[k+dimension-1].blue+source[k+dimension].blue+source[k+dimension+1].blue+source[k-dimension-1].blue+source[k-dimension].blue+source[k-dimension+1].blue)/9;
    destination[k].green = (source[k].green+source[k-1].green+source[k+1].green+source[k+dimension-1].green+source[k+dimension].green+source[k+dimension+1].green+source[k-dimension-1].green+source[k-dimension].green+source[k-dimension+1].green)/9;
    destination[k].red = (source[k].red+source[k-1].red+source[k+1].red+source[k+dimension-1].red+source[k+dimension].red+source[k+dimension+1].red+source[k-dimension-1].red+source[k-dimension].red+source[k-dimension+1].red)/9;
}
char opitmal_smooth_descr[]="optimal_smooth: Optimal smooth version";
void optimal_smooth(int dimension, pixel *source, pixel *destination){
    int a1,a2,a3,cc;
    a1=1;
    a2=dimension;
    a3=dimension+1;
    cc=0;
    destination[cc].blue = (source[cc].blue+source[a1].blue+source[a2].blue+source[a3].blue) >> 2;
    destination[cc].green = (source[cc].green+source[a1].green+source[a2].green+source[a3].green) >> 2;
    destination[cc].red = (source[cc].red+source[a1].red+source[a2].red+source[a3].red) >> 2;

    cc=dimension-1;
    a1=dimension-2;
    a2=dimension+dimension-2;
    a3=dimension+dimension-1;
    destination[cc].blue = (source[cc].blue+source[a1].blue+source[a2].blue+source[a3].blue) >> 2;
    destination[cc].green = (source[cc].green+source[a1].green+source[a2].green+source[a3].green) >> 2;
    destination[cc].red = (source[cc].red+source[a1].red+source[a2].red+source[a3].red) >> 2;

    cc=RIDX(dimension-1, 0, dimension);
    a1=RIDX(dimension-1, 1, dimension);
    a2=RIDX(dimension-2, 0, dimension);
    a3=RIDX(dimension-2, 1, dimension);
    destination[cc].blue = (source[cc].blue+source[a1].blue+source[a2].blue+source[a3].blue) >> 2;
    destination[cc].green = (source[cc].green+source[a1].green+source[a2].green+source[a3].green) >> 2;
    destination[cc].red = (source[cc].red+source[a1].red+source[a2].red+source[a3].red) >> 2;

    cc=RIDX(dimension-1, dimension-1, dimension);
    a1=RIDX(dimension-1, dimension-2, dimension);
    a2=RIDX(dimension-2, dimension-2, dimension);
    a3=RIDX(dimension-2, dimension-1, dimension);
    destination[cc].blue = (source[cc].blue+source[a1].blue+source[a2].blue+source[a3].blue) >> 2;
    destination[cc].green = (source[cc].green+source[a1].green+source[a2].green+source[a3].green) >> 2;
    destination[cc].red = (source[cc].red+source[a1].red+source[a2].red+source[a3].red) >> 2;


    for(int j = 1; j <= dimension-2; j++){
        set_top(dimension, source, destination, j);
        set_bottom(dimension, source, destination, dimension*dimension-dimension+j);
        set_left(dimension, source, destination, j*dimension);
        set_right(dimension, source, destination, j*dimension+dimension-1);

    }

    for(int i = 1; i <= dimension-2; i++){
        for(int j = 1; j <= dimension-2; j++){
            set_in(dimension, source, destination, i*dimension+j);
        }
    }
}
char smooth_descr[] = "smooth: Current working version";
void smooth(int dimension, pixel *source, pixel *destination)
{
    optimal_smooth(dimension, source, destination);
}



/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

