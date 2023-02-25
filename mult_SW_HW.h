#ifndef MULT_SW_HW_H
#define MULT_SW_HW_H

//typedef uint8 input_type;
#include <ap_int.h>
#define uint8 ap_uint<8>
#define uint32 ap_uint<32>
typedef uint8 input_type;
typedef uint32 output_type;

//Array size to access
#define lm 7
#define ln 7
#define lp 7
#define m 1<<lm
#define n 1<<ln
#define p 1<<lp


//define BRAM


//Declaring the hardware function
void mult_SW_HW(input_type in1[n][m], input_type in2[m][p], output_type out[n][p], int dim);



#endif
