#include <stdio.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <mpi.h>
#include "Matrix.h"
#include "Vector.h"
#include "Utility.h"

void pushTheElem(float* in, float* inout, int *len, MPI_Datatype *datatype) { 
	for(int i=0; i < *len; ++i){ 
		inout[i] += in[i]; 
	}
}

int main(int argc, char* argv[])
{
	
	
	std::cout << argv[1] << std::endl;
	if (argv[1] == NULL) 
	{
		printf("please enter the file name.\n");
		exit(0);
	}
	std::string filename = argv[1];
	std::cout << filename << std::endl;
	int myrank;
	int nprocs;
	int source;

	MPI_Status status;
	char message[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	std::vector<std::vector<float> > vec3(3, std::vector<float>(3, 2));
	for (int i = 0; i < vec3.size(); i++) { 
		for (int j = 0; j < vec3[0].size(); j++) {
			vec3[i][j] = i * vec3.size() + j + 1;  
		}  
	}

	Matrix<float> m(vec3);
	if (myrank == 0)
	{ 
		for (int i = 0; i < m.Row(); i++) {  
			for (int j = 0; j < m.Col(); j++) {  
				std::cout << m.Data()[i * m.Col() + j] << " ";  
			}  
			std::cout << std::endl;  
		} 
	}
	
	Matrix<float> sum_m(m.Row(), m.Col());
	if (myrank == 0)
	{ 
		for (int i = 0; i < sum_m.Row(); i++) {  
			for (int j = 0; j < sum_m.Col(); j++) {  
				sum_m.Data()[i * sum_m.Col() + j] += 100.1;  
			}  
		} 
	}

	MPI_Op myOp; 
	MPI_Op_create((MPI_User_function*)Utility<float>::addTheElem, true, &myOp); 
	
	MPI_Reduce(m.Data(), sum_m.Data(), m.Row() * m.Col(), MPI_FLOAT, myOp, 0, MPI_COMM_WORLD);   
	
	if (myrank == 0)
	{ 
		m = sum_m;
		
		for (int i = 0; i < m.Row(); i++) {  
			for (int j = 0; j < m.Col(); j++) {  
				std::cout << m.Data()[i * m.Col() + j] << " ";  
			}  
			std::cout << std::endl;  
		}
		
	}
	MPI_Bcast(m.Data(), m.Row() * m.Col(), MPI_FLOAT, 0, MPI_COMM_WORLD);
	if (myrank == 1)
	{ 
		for (int i = 0; i < m.Row(); i++) {  
			for (int j = 0; j < m.Col(); j++) {  
				std::cout << m.Data()[i * m.Col() + j] << " ";  
			}  
			std::cout << std::endl;  
		}
		
	}

	MPI_Barrier(MPI_COMM_WORLD);

	float w = myrank;

	if (myrank != 0)
	{
		strcpy(message, "Hello world!");
		MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 99, MPI_COMM_WORLD);
	}
	else
	{
		for (source = 1; source < nprocs; source++)
		{
			MPI_Recv(message, 100, MPI_CHAR, source, 99, MPI_COMM_WORLD, &status);
			printf("Receive from %d process: %s\n", source, message);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	
	float sum;
	for (int i = 0; i < 2; i++)
	{
	
		for (int j = 1; j < 17; j++)
		{
			if (j / nprocs != myrank) continue;    
			//printf("rank %i working on element %i\n", myrank, j);
			w = j;
		}
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Reduce(&w, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
		if (myrank == 0) 
		{
			sum /= nprocs;
			printf("w: %f\n", sum);
			w = sum;
		}
		
		MPI_Bcast(&w, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
		printf("rank %d: %f\n", myrank, w);

	}

	MPI_Barrier(MPI_COMM_WORLD);
	std::vector<double> test(3), result(3); 

	if (myrank==0) 
	{ 
		test[0] = 1; 
		test[1] = 2; 
		test[2] = 3;
	}
	else 
	{ 
		test[0] = 4;
		test[1] = 5;
		test[2] = 6; 
	}
	MPI_Reduce(&test[0], &result[0], 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
	
	printf("test from %d: %f\n", myrank, test[0]);
	printf("result from %d: %f\n", myrank, result[0]);

	MPI_Finalize();

	return 0;
}
