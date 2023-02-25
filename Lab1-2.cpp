#include <ap_int.h>
#include "mult_SW_HW.h"

// First case: basic multiplication
/*
void MATRIX_MUL(input_type A[n][m], input_type B[m][p], output_type AB[n][p], int dim)
{

    for(int i = 0; i < n; i++){
        for(int j = 0; j < p; j++){
             int result = 0;
             for(int k = 0; k < m; k++){
                result += A[i][k] * B[k][j];
             }
             AB[i][j] = result;
        }
    }
}
*/
//---------------------------------------------------------------------OPTIMIZATIONS---------------------------------------------------------------------

// Loop unroll: execute all "result" calculations for the same i & j values at the same time 
// PIPELINE: after the first instruction is executed, the following are executed after a cycle at a time => total exec. time: (exec. time of the first instruction + num of remaining instructions)
// ARRAY PARTITION: partitions an array into smaller arrays or individual elements - necessary for loop unrolling

//-------------------------------------------------------------------------------------------------------------------------------------------------------

/*
void MATRIX_MUL(input_type A[n][m], input_type B[m][p], output_type AB[n][p], int dim)
{
	
	//constants
	    const int n_size = n;
	    const int p_size = p;
	    const int partition_factor = m >> 1;
		
	
#pragma HLS ARRAY_PARTITION variable = A cyclic factor = partition_factor dim = 2		// cyclic: the array is partitioned cyclically by putting one element into each new array 
#pragma HLS ARRAY_PARTITION variable = B cyclic factor = partition_factor dim = 1		// before coming back to the first array to repeat the cycle until the array is fully partitioned.
																						// factor: specifies the number of smaller arrays that are to be created.		
																				

	  
    for(int i = 0; i < n; i++){
	#pragma HLS loop_tripcount min = n_size max = n_size	// specify the total number of iterations performed by a loop - for analysis only
															// useful in cases where the loop latency is unknown or cannot be calculated
															// the loop_tripcount pragma lets us specify minimum and maximum iterations for a loop
				
        for(int j = 0; j < p; j++){
#pragma HLS loop_tripcount min = p_size max = p_size
        	int result = 0;
            for(int k = 0; k < m; k++){
				result += A[i][k] * B[k][j];
             }
             AB[i][j] = result;
        }
    }
}
*/
//---------------------------------------------------------------------unroll---------------------------------------------------------------------
/*

void MATRIX_MUL(input_type A[n][m], input_type B[m][p], output_type AB[n][p], int dim)
{
	
	//constants
	    const int n_size = n;
	    const int p_size = p;
	    const int partition_factor = m >> 1;
		
	
#pragma HLS ARRAY_PARTITION variable = A cyclic factor = partition_factor dim = 2		// cyclic: The array is partitioned cyclically by putting one element into each new array 
#pragma HLS ARRAY_PARTITION variable = B cyclic factor = partition_factor dim = 1		// before coming back to the first array to repeat the cycle until the array is fully partitioned.
																						// factor: specifies the number of smaller arrays that are to be created.	

	
	
    for(int i = 0; i < n; i++){
	#pragma HLS loop_tripcount min = n_size max = n_size	// specify the total number of iterations performed by a loop - for analysis only
															// useful in cases where the loop latency is unknown or cannot be calculated
															// the loop_tripcount pragma lets us specify minimum and maximum iterations for a loop
				
        for(int j = 0; j < p; j++){
#pragma HLS loop_tripcount min = p_size max = p_size
             int result = 0;
             for(int k = 0; k < m; k++){
#pragma HLS unroll factor = 64		// factor: the loop body is repeated the specified number of times, and the iteration information is adjusted accordingly
            	 result += A[i][k] * B[k][j];
             }
             AB[i][j] = result;
        }
    }
}
*/

//---------------------------------------------------------------------pipeline---------------------------------------------------------------------
/*
void MATRIX_MUL(input_type A[n][m], input_type B[m][p], output_type AB[n][p], int dim)
{
	
	//constants
	    const int n_size = n;
	    const int p_size = p;
	    const int partition_factor = m >> 1;
		
	
#pragma HLS ARRAY_PARTITION variable = A cyclic factor = partition_factor dim = 2		// cyclic: The array is partitioned cyclically by putting one element into each new array 
#pragma HLS ARRAY_PARTITION variable = B cyclic factor = partition_factor dim = 1		// before coming back to the first array to repeat the cycle until the array is fully partitioned.
																						// factor: specifies the number of smaller arrays that are to be created.	

	
	
    for(int i = 0; i < n; i++){
	#pragma HLS loop_tripcount min = n_size max = n_size	// specify the total number of iterations performed by a loop - for analysis only
															// useful in cases where the loop latency is unknown or cannot be calculated
															// the loop_tripcount pragma lets us specify minimum and maximum iterations for a loop
				
        for(int j = 0; j < p; j++){
		#pragma HLS loop_tripcount min = p_size max = p_size
#pragma HLS PIPELINE II = 1				// everything after this is unrolled and pipelined 	 -	 II: initiation interval
             int result = 0;
             for(int k = 0; k < m; k++){
            	 result += A[i][k] * B[k][j];
             }
             AB[i][j] = result;
        }
    }
}
*/

//---------------------------------------------------------------------using BRAMs---------------------------------------------------------------------

void MATRIX_MUL(input_type A[n][m], input_type B[m][p], output_type AB[n][p], int dim)
{
	//constants
	    const int n_size = n;
	    const int m_size = m;
	    const int p_size = p;
	    const int partition_factor = m >> 1;

	    input_type BRAM_A[n][m];
	    input_type BRAM_B[m][p];
		

	#pragma HLS ARRAY_PARTITION variable = BRAM_A cyclic factor = partition_factor dim = 2		// cyclic: The array is partitioned cyclically by putting one element into each new array 
	#pragma HLS ARRAY_PARTITION variable = BRAM_B cyclic factor = partition_factor dim = 1      // before coming back to the first array to repeat the cycle until the array is fully partitioned.
																								// factor: specifies the number of smaller arrays that are to be created.	


	    //Read data from DDR and write it to BRAMs
	    for(int i = 0; i < n; i++){
	#pragma HLS loop_tripcount min = n_size max = n_size

	        for(int j = 0; j < m; j++){
	        #pragma HLS loop_tripcount min = m_size max = m_size
	 #pragma HLS PIPELINE II = 1				// II: initiation interval
	        	BRAM_A[i][j] = A[i][j];
	        }
	    }
	    //Read data from DDR and write it to BRAMs
	     for(int i = 0; i < m; i++){
	    	#pragma HLS loop_tripcount min = m_size max = m_size

	    	        for(int j = 0; j < p; j++){
	    	        #pragma HLS loop_tripcount min = p_size max = p_size
	    	#pragma HLS PIPELINE II = 1				// II: initiation interval
	    	        	BRAM_B[i][j] = B[i][j];
	    	        }
	    	    }
	    //Read data from BRAMs, multiply and write back the result to DDR.
	    for(int i = 0; i < n; i++){
	    #pragma HLS loop_tripcount min = n_size max = n_size
		
	    	for(int j = 0; j < p; j++){
	    	#pragma HLS loop_tripcount min = p_size max = p_size		// specify the total number of iterations performed by a loop - for analysis only
			                                                            // useful in cases where the loop latency is unknown or cannot be calculated
			                                                            // the loop_tripcount pragma lets us specify minimum and maximum iterations for a loop
	#pragma HLS PIPELINE II = 1				// II: initiation interval

	    		int result = 0;
	             for(int k = 0; k < m; k++){

	                result += BRAM_A[i][k] * BRAM_B[k][j];
	             }
	             AB[i][j] = result;
	        }
	    }
}



