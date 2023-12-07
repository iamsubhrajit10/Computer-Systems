/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#define blockSize 16
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
void optimal_rotate(int dim, pixel *source, pixel *destination)
{
    int i,j;
    int destination_base = (dim-1)*dim;
    destination += destination_base;

    for(i=0;i<dim;i+=blockSize)
    {
        //unrolling loop by 16 elements
        for(j=0;j<dim;j++)
        {
            *destination = *source;
            source += dim;
            destination++;
            *destination = *source;
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source; 
            source += dim; destination++;
            *destination = *source; 
            source += dim; 
            destination++;
            *destination = *source;
            source++;
            source -=(dim << 4) - dim;
            destination -= 15+dim;
        }
        destination += destination_base + dim;
        destination += 16;
        source += (dim<<4)-dim;
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


char opitmal_smooth_descr[]="optimal_smooth: Optimal smooth version";
/*
 * The function first calculates the average color value of the four pixels surrounding the top left corner pixel of the input
 * image. This is done by adding up the color values of the top, left, bottom, and right pixels and then dividing by four. The
 * average color value is then stored in the top left corner pixel of the output image.
 *
 * Similarly it does it for four pixels surrounding the top right corner pixel, bottom left corner pixel
 * , and bottom right corner pixel.
 *
 * The function then processes the remaining pixels in the input image. For each pixel, the function calculates the average
 * color value of the six pixels surrounding it. The average color value is then stored in the corresponding pixel of the
 * output image.
 *
 */
void optimal_smooth(int dim, pixel *source, pixel *destination)
{
    int i, j, limit, m;

    //four pixels surrounding the left corner pixel
    pixel src_0=source[0], src_1=source[1], src_dim_plus1=source[dim + 1], src_dim=source[dim];

    destination[0].red = (src_0.red + src_1.red + src_dim_plus1.red + src_dim.red) >> 2;
    destination[0].green = (src_0.green + src_1.green + src_dim.green + src_dim_plus1.green) >> 2;
    destination[0].blue = (src_0.blue + src_1.blue + src_dim.blue + src_dim_plus1.blue) >> 2;

    //four pixels surrounding the right corner pixel
    i = dim * 2 - 1;
    pixel src_dim_2=source[dim - 2], src_dim_1=source[dim-1], src_i_1=source[i-1], src_i=source[i];

    destination[dim - 1].red = (src_dim_2.red + src_dim_1.red + src_i_1.red + src_i.red) >> 2;
    destination[dim - 1].green = (src_dim_2.green + src_dim_1.green + src_i_1.green + src_i.green) >> 2;
    destination[dim - 1].blue = (src_dim_2.blue + src_dim_1.blue + src_i_1.blue + src_i.blue) >> 2;


    //four pixels surrounding the bottom left corner pixel
    j = dim*(dim - 1);
    i = dim*(dim - 2);
    pixel src_j=source[j], src_j_plus_1=source[j+1];
    src_i=source[i], src_i_1=source[i+1];

    destination[j].red = (src_j.red + src_j_plus_1.red + src_i.red + src_i_1.red) >> 2;
    destination[j].blue = (src_j.blue + src_j_plus_1.blue + src_i.blue + src_i_1.blue) >> 2;
    destination[j].green = (src_j.green + src_j_plus_1.green + src_i.green + src_i_1.green) >> 2;

    //four pixels surrounding the bottom right corner pixel
    j = dim*dim - 1;
    i = dim*(dim - 1) - 1;


    //processing the remaining pixels in the input image
    pixel src_j_minus_1=source[j-1], src_i_minus_1=source[i-1];
    src_j=source[j], src_i=source[i];
    destination[j].red = (src_j_minus_1.red + src_j.red + src_i_minus_1.red + src_i.red) >> 2;
    destination[j].blue = (src_j_minus_1.blue + src_j.blue + src_i_minus_1.blue + src_i.blue) >> 2;
    destination[j].green = (src_j_minus_1.green + src_j.green + src_i_minus_1.green + src_i.green) >> 2;


    //handling up edge case
    limit = dim - 1;
    i = dim + 1;
    for (j = 1;j < limit;)
    {
        src_j=source[j];
        src_j_minus_1=source[j-1];
        src_j_plus_1=source[j+1];
        src_i=source[i];
        src_i_1=source[i+1];
        src_i_minus_1=source[i-1];

        destination[j].red = (src_j.red + src_j_minus_1.red + src_j_plus_1.red + src_i.red + src_i_1.red + src_i_minus_1.red) / 6;
        destination[j].green = (src_j.green + src_j_minus_1.green + src_j_plus_1.green + src_i.green + src_i_1.green + src_i_minus_1.green) / 6;
        destination[j].blue = (src_j.blue + src_j_minus_1.blue + src_j_plus_1.blue + src_i.blue + src_i_1.blue + src_i_minus_1.blue) / 6;
        j++;i++;
    }
    //handling bottom edge case
    limit  = dim*dim - 1;

    for (j = dim*(dim - 1) + 1, i=j-dim;j < limit;)
    {
        src_j=source[j];
        src_j_minus_1=source[j-1];
        src_j_plus_1=source[j+1];
        src_i=source[i];
        src_i_1=source[i+1];
        src_i_minus_1=source[i-1];

        destination[j].red = (src_j.red + src_j_minus_1.red + src_j_plus_1.red + src_i.red + src_i_1.red + src_i_minus_1.red) / 6;
        destination[j].green = (src_j.green + src_j_minus_1.green + src_j_plus_1.green + src_i.green + src_i_1.green + src_i_minus_1.green) / 6;
        destination[j].blue = (src_j.blue + src_j_minus_1.blue + src_j_plus_1.blue + src_i.blue + src_i_1.blue + src_i_minus_1.blue) / 6;
        j++;i++;
    }
    //handling right edge case
    m = dim - 1;
    i = 3 * dim - 1;
    pixel src_m, src_m_1;

    for (j = 2 * dim - 1;j < limit;)
    {
        src_j=source[j];
        src_j_minus_1=source[j-1];
        src_m=source[m];
        src_m_1=source[m-1];
        src_i=source[i];
        src_i_minus_1=source[i-1];

        destination[j].red = (src_j.red + src_j_minus_1.red + src_m.red + src_m_1.red + src_i.red + src_i_minus_1.red) / 6;
        destination[j].green = (src_j.green + src_j_minus_1.green + src_m.green + src_m_1.green + src_i.green + src_i_minus_1.green) / 6;
        destination[j].blue = (src_j.blue + src_j_minus_1.blue + src_m.blue + src_m_1.blue + src_i.blue + src_i_minus_1.blue) / 6;
        j += dim;
        m += dim;
        i += dim;
    }
    //handling left edge case
    limit = limit - dim;
    m = 0;
    i = dim + dim;

    for (j = dim;j < limit;)
    {
        src_j=source[j];
        src_j_plus_1=source[j+1];
        src_m=source[m];
        src_m_1=source[m+1];
        src_i=source[i];
        src_i_1=source[i+1];

        destination[j].red = (src_j.red + src_j_plus_1.red + src_m.red + src_m_1.red + src_i.red + src_i_1.red) / 6;
        destination[j].green = (src_j.green + src_j_plus_1.green + src_m.green + src_m_1.green + src_i.green + src_i_1.green) / 6;
        destination[j].blue = (src_j.blue + src_j_plus_1.blue + src_m.blue + src_m_1.blue + src_i.blue + src_i_1.blue) / 6;
        j += dim;
        m += dim;
        i += dim;
    }


    int index = dim + 1;
    int index_u = 1;
    int index_d = (dim<<1)+1;
    int pre_red_1, pre_green_1, pre_blue_1, pre_red_2, pre_green_2, pre_blue_2, pre_red_3, pre_green_3, pre_blue_3;
    pixel src_indx_u, src_indx_1, src_indx_d_1;
    for (i = 1; i < dim - 1;i++)
    {
        //preparing the first 3 line_sum
        src_indx_u=source[index_u-1];
        src_indx_1=source[index - 1];
        src_indx_d_1=source[index_d - 1];
        pre_red_1 = src_indx_u.red + src_indx_1.red + src_indx_d_1.red;
        pre_green_1 = src_indx_u.green + src_indx_1.green + src_indx_d_1.green;
        pre_blue_1 = src_indx_u.blue + src_indx_1.blue + src_indx_d_1.blue;

        src_indx_u=source[index_u];
        src_indx_1=source[index];
        src_indx_d_1=source[index_d];

        pre_red_2 = src_indx_u.red + src_indx_1.red + src_indx_d_1.red;
        pre_green_2 = src_indx_u.green + src_indx_1.green + src_indx_d_1.green;
        pre_blue_2 = src_indx_u.blue + src_indx_1.blue + src_indx_d_1.blue;

        src_indx_u=source[index_u+1];
        src_indx_1=source[index+1];
        src_indx_d_1=source[index_d+1];

        pre_red_3 = src_indx_u.red + src_indx_1.red + src_indx_d_1.red;
        pre_green_3 = src_indx_u.green + src_indx_1.green + src_indx_d_1.green;
        pre_blue_3 = src_indx_u.blue + src_indx_1.blue + src_indx_d_1.blue;


        destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
        destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
        destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
        index++;
        index_u++;
        index_d++;

        for (j = 2; j < dim - 12;j+=12)
        {
            //block1
            //update pre_1

            pre_red_1 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;
            destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_1 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;
            destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_1 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;
            destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_2
            pre_red_2 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_2 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_2 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_3
            pre_red_3 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_3 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_3 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //block2
            //update pre_1
            pre_red_1 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_1 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_1 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_2
            pre_red_2 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_2 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_2 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_3
            pre_red_3 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_3 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_3 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //block3
            //update pre_1
            pre_red_1 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_1 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_1 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_2
            pre_red_2 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_2 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_2 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_3
            pre_red_3 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_3 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_3 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //block4
            //update pre_1
            pre_red_1 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_1 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_1 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_2
            pre_red_2 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_2 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_2 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_3
            pre_red_3 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_3 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_3 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;



        }
        //deal with the rest pixels of the line
        for (;j < dim - 3;j += 3) {

            //update pre_1
            pre_red_1 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_1 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_1 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_2
            pre_red_2 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_2 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_2 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

            //update pre_3
            pre_red_3 = source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red;destination[index].red = (pre_red_1 + pre_red_2 + pre_red_3) / 9;
            pre_green_3 = source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green;destination[index].green = (pre_green_1 + pre_green_2 + pre_green_3) / 9;
            pre_blue_3 = source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue;destination[index].blue = (pre_blue_1 + pre_blue_2 + pre_blue_3) / 9;
            index++;
            index_u++;
            index_d++;

        }
        for (;j < dim - 1;j++)
        {
            destination[index].red = (source[index_u - 1].red + source[index - 1].red + source[index_d - 1].red + source[index_u].red + source[index].red + source[index_d].red + source[index_u + 1].red + source[index + 1].red + source[index_d + 1].red) / 9;
            destination[index].green = (source[index_u - 1].green + source[index - 1].green + source[index_d - 1].green + source[index_u].green + source[index].green + source[index_d].green + source[index_u + 1].green + source[index + 1].green + source[index_d + 1].green) / 9;
            destination[index].blue = (source[index_u - 1].blue + source[index - 1].blue + source[index_d - 1].blue + source[index_u].blue + source[index].blue + source[index_d].blue + source[index_u + 1].blue + source[index + 1].blue + source[index_d + 1].blue) / 9;
            index++;
            index_u++;
            index_d++;

        }
        index+=2;
        index_u+=2;
        index_d+=2;
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

