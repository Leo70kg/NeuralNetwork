#include <mpi.h>
#include "Vector.h"
#include "Matrix.h"
#include "BSDEModelRing.h"
#include "BSDEConfiguration.h"
#include "collectives.h"
#include "Utility.h"
#include "PricingDefaultRisk.h"
#include "HJBLQ.h"
#include "AllenCahn.h"


int main(int argc, char* argv[])
{
	if (argv[1] == NULL || argv[2] == NULL)
	{
		printf("Please enter the file name or equation name.\n");
		exit(0);
	}
	std::string filepath;
	filepath += "config/";
	filepath += argv[1];
	
	BSDEConfiguration* config = new BSDEConfiguration();
	config->Load(filepath);
	
	Equation* equation;

	switch(argv[2][0]) 
	{    
		case 'H':
			equation = new HJBLQ(*config);
			break; 

		case 'P':
			equation = new PricingDefaultRisk(*config);   
			break; 

		case 'A':
			equation = new AllenCahn(*config);
			break;

		default:
			printf("Please choose a PDE\n");
			exit(0);
	}
	
	int myrank, nprocs;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	if (myrank == 0)
	{
		std::cout << "Ring-AllReduce Mode\n\n";
		std::cout << "Dimension: " << config->dim_input << "\n";
		std::cout << "Total time: " << config->totalTime << "\n";
		std::cout << "Number of time intervals: " << config->numTimeInterval << "\n";
		std::cout << "Learning rate: " << config->learning_rate << "\n";
		std::cout << "y_init_range: " << config->yInitRange[0] << ", " << config->yInitRange[1] << "\n";
		std::cout << "Number of neurons in each hidden layer of each subnet: ";
		for (int i = 0; i < config->subnetLayerNumber - 1; i++)
		{
			std::cout << config->subnetHiddenLayerSizes[i] << ", ";
		}
	
		std::cout << "\nEpochs: " << config->train_epoch << "\n";
		std::cout << "Batch size: " << config->batchSize << "\n";
		std::cout << "Sample size: " << config->sampleSize << "\n";
		std::cout << "Logging frequency: " << config->logging_frequency << "\n";
		std::cout << "Model name: " << config->modelSaveName << "\n";
		std::cout << "Number of processors: " << nprocs << "\n\n\nTraining:\n";
	}
	
	/*
	std::cout << config->dim_input << " " << config->dim_output << std::endl;
	std::cout << config->learning_rate << " " << config->batchSize << std::endl;  
	std::cout << config->train_epoch << std::endl;  
	std::cout << config->totalTime << std::endl; 
	std::cout << config->numTimeInterval << std::endl; 
	std::cout << config->subnetLayerNumber << std::endl; 
	std::cout << config->sampleSize << std::endl;  
	std::cout << config->yInitRange[0] << " " << config->yInitRange[1] << std::endl; 
	std::cout << config->subnetHiddenLayerSizes[0] << " " << config->subnetHiddenLayerSizes[1] << std::endl;
	std::cout << config->logging_frequency << std::endl;
	*/

	std::unique_ptr<BSDEModelRing> lr = config->CreateModelRing(myrank, nprocs);
	lr->Fit(*equation);
	
	delete equation;

	MPI_Finalize();

	return 0;
}

