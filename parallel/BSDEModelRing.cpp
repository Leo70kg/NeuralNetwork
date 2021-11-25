#include "BSDEModelRing.h"
#include "collectives.h"
#include "Utility.h"


// Use SGD
void BSDEModelRing::Fit(const Equation& equation)
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
	
	float total_y_init = 0;
	Vector<float> total_z_init(z_init.Size());
	int batch = m_config.batchSize / m_nprocs;
	batch = batch == 0 ? 1 : batch;

	for (int i = 1; i <= m_config.train_epoch; i++)
    {   
		float loss = 0.0;
        
		for (int j = 0; j < m_config.sampleSize; j++)
        {
			int chuckSize = m_config.sampleSize / m_nprocs; 
			if (j < chuckSize * m_rank || j >= chuckSize * (m_rank + 1)) continue;
			
			float y = equation.g_tf(0.0, equation.GetXSample()[j][numTimeInterval]);

            if (j % batch == 0)
            {
                ClearGradient();
            }
			
            float pred = Eval(equation, j);
            ComputeGradient(equation, y, j);
            
			loss += Loss(y, pred);
			
            if (j % batch == (batch - 1) ||
                j == (chuckSize * (m_rank + 1) - 1))
            {
                Update();
			}
		}
	
		MPI_Barrier(MPI_COMM_WORLD);
		
		RingAllreduce(&loss, 1, &total_loss);
		RingAllreduce(&y_init, 1, &total_y_init);
		
		// Send updated weights parameters in each process to root process.
		for (int k = 0; k < L * numOfSubnet; k++)
		{
			RingAllreduce(weights[k].Data(), weights[k].Row() * weights[k].Col(), sum_weights[k].Data());
		
		}
			
		RingAllreduce(z_init.Data(), z_init.Size(), total_z_init.Data());

		for (int k = 0; k < L * numOfSubnet; k++)
		{
			(weights[k] = sum_weights[k]).Div(m_nprocs);
		}
			
		y_init = total_y_init / m_nprocs;
		(z_init = total_z_init).Div(m_nprocs);
		
		if (m_rank == root_rank)
		{
			if (i % m_config.logging_frequency == 0)
			{
				Save(i);
				std::cout << "Epoch:" << i << std::endl;
				std::cout << "In training set, loss: " << total_loss << ", Y0: " << y_init << std::endl << std::endl;
			}
		}
	}

}


