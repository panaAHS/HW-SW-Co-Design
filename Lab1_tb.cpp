#include <iostream>
#include <stdlib.h>
#include "mult_SW_HW.h"


// hardware matrix funtion

void MATRIX_MUL(input_type A[n][m], input_type B[m][p], output_type AB[n][p], int dim);

//	software matrix funtion

void MATRIX_MUL_SW(input_type A[n][m], input_type B[m][p],  output_type AB[n][p] , int dim)
{

	//Performs matrix multiplication out = 	A * B
	for(int i = 0; i < n; i++){
		for(int j = 0; j < p; j++){
			for(int k = 0; k < m; k++){
					   AB[i][j] += A[i][k] * B[k][j];
			       }
			  }
		}
}


int main(int argc, char** argv)
{
	int dim = 255;

	input_type A[n][m];
    input_type B[m][p];
    output_type hw_AB[n][p];
    output_type sw_AB[n][p];




    //---------------------------------------------------------------------TEST---------------------------------------------------------------------

    // Set Matrix A
    for(int i = 0; i < n; i++){
    		for(int j = 0; j < m; j++){
    			A[i][j] = rand() % dim;
    		}
    }

    // Set Matrix B
    for(int i = 0; i < m; i++){
        	for(int j = 0; j < p; j++){
        		B[i][j] = rand() % dim;
        		}
     }

    // Set HW & SW Matrixes (all elements == 0 at first)
    for(int i = 0; i < n; i++){
            for(int j = 0; j < p; j++){
            	hw_AB[i][j] = 0;
            	sw_AB[i][j] = 0;
            	}
   }

	// Run the hardware matrix funtion
    MATRIX_MUL(A, B, hw_AB, dim);


	//Run the software matrix funtion
    MATRIX_MUL_SW(A, B, sw_AB, dim);


	//Compare results:
	// if (it_works == 1)  => IT WORKS!!!!!!
	// else, something has gone wrong - a confict is detected (it_works == 0)
	
	bool it_works = 1;			
								
	for(int i = 0; i < n; i++){
	       for(int j = 0; j < p; j++){
	    	   if(hw_AB[i][j] != sw_AB[i][j]){
	    		   std::cout << "Conflict detected on " << "(row, col): (" << i << " ," << j << ")" << std::endl;
	    		   std::cout << "HW_OUT: "<< hw_AB[i][j] << "\t SW_OUT:" << sw_AB[i][j] << std::endl;
	    		   it_works = 0;
	    		   break;
	    	   }
	       }
	}


	if (it_works){
		for(int i = 0; i < n; i++){
			       for(int j = 0; j < p; j++){
			    		   std::cout << hw_AB[i][j] << "\t";
			       }
			       std::cout << std::endl;
			}
		std::cout << "TEST PASSED!" << std::endl;
	}
	else
		std::cout << "TEST FAILED...";

	return(it_works ? EXIT_SUCCESS: EXIT_FAILURE);	// Both EXIT_SUCCESS and the value zero indicate successful program execution status (see exit), although it is not required that EXIT_SUCCESS equals zero.
}
