#include "BSDEModelMs.h"
#include "Utility.h"


// Use SGD
void BSDEModelMs::Fit(const Equation& equation)
{
	int m_rank;
	int mpi_error = MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);  
	
	if(mpi_error != MPI_SUCCESS)  
		throw std::runtime_error("MPI_Comm_rank failed with an error");
	
	int m_nprocs;
	mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &m_nprocs);     
	if(mpi_error != MPI_SUCCESS)
		throw std::runtime_error("MPI_Comm_size failed with an error");

	int numTimeInterval = m_config.numTimeInterval;
	int numOfSubnet = m_config.numTimeInterval - 1;
    int L = m_config.subnetLayerNumber;     // layer number in each subnet
	
	MPI_Op myOp;
	MPI_Op_create((MPI_User_function*)Utility<float>::addTheElem, true, &myOp);

	total_loss = 0.0;
	float total_y_init = 0.0;
	Vector<float> total_z_init(z_init.Size());
	int batch = m_config.batchSize / m_nprocs;
	if (batch == 0) 
	{
		batch = 1;
	}
	
	for (int i = 1; i <= m_config.train_epoch; i++)
    {   
		//std::cout << "echo: " << i << std::endl;
		float loss = 0.0;
        
		//float sum = 0.0;
		for (int j = 0; j < m_config.sampleSize; j++)
        {
			int chuckSize = m_config.sampleSize / m_nprocs; 
			if (j < chuckSize * m_rank || j >= chuckSize * (m_rank + 1)) continue;
			
			float y = equation.g_tf(0.0, equation.GetXSample()[j][numTimeInterval]);
			//sum += y;  

            if (j % batch == 0)
            {
                ClearGradient();
            }
			
            float pred = Eval(equation, j);
            ComputeGradient(equation, y, j);
            
			//Update();
			loss += Loss(y, pred);
			
			//std::cout << pred - y << std::endl;
            if (j % batch == (batch - 1) ||
                j == (chuckSize * (m_rank + 1) - 1))
            {
                Update();
			}
		}

		//std::cout << "y_init: " << y_init << std::endl;
		//std::cout << "true mean: " << sum / m_config.sampleSize << std::endl;
		
		MPI_Barrier(MPI_COMM_WORLD);
		
		MPI_Reduce(&loss, &total_loss, 1, MPI_FLOAT, MPI_SUM, root_rank, MPI_COMM_WORLD);
		MPI_Reduce(&y_init, &total_y_init, 1, MPI_FLOAT, MPI_SUM, root_rank, MPI_COMM_WORLD);

		// Send updated weights parameters in each process to root process.
		for (int k = 0; k < L * numOfSubnet; k++)
		{
			MPI_Reduce(weights[k].Data(), sum_weights[k].Data(), weights[k].Row() * weights[k].Col(), MPI_FLOAT, myOp, root_rank, MPI_COMM_WORLD);
		}
		
		MPI_Reduce(z_init.Data(), total_z_init.Data(), z_init.Size(), MPI_FLOAT, myOp, root_rank, MPI_COMM_WORLD);

		if (m_rank == root_rank)
		{
			for (int k = 0; k < L * numOfSubnet; k++)
			{
				(weights[k] = sum_weights[k]).Div(m_nprocs);
			}
			
			y_init = total_y_init / m_nprocs;
			(z_init = total_z_init).Div(m_nprocs);	
		}
		
		for (int k = 0; k < L * numOfSubnet; k++)
		{
			MPI_Bcast(weights[k].Data(), weights[k].Row() * weights[k].Col(), MPI_FLOAT, root_rank, MPI_COMM_WORLD);
		}

		MPI_Bcast(&y_init, 1, MPI_FLOAT, root_rank, MPI_COMM_WORLD);
		MPI_Bcast(z_init.Data(), z_init.Size(), MPI_FLOAT, root_rank, MPI_COMM_WORLD);
	
		if (m_rank == root_rank)
		{
			if (i % m_config.logging_frequency == 0)
			{
				Save(i);
				std::cout << "Epoch:" << i << std::endl;
				std::cout << "In training set, loss: " << total_loss << ", Y0: " << y_init << std::endl << std::endl;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Op_free(&myOp);
    
}


