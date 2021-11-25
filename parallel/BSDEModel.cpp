#include "BSDEModel.h"
#include "Utility.h"

BSDEModel::BSDEModel(const BSDEConfiguration& config) : m_config(config)
{
	int m_rank; 
	int mpi_error = MPI_Comm_rank(MPI_COMM_WORLD, &m_rank); 
	
	if (mpi_error != MPI_SUCCESS) 
		throw std::runtime_error("MPI_Comm_rank failed with an error");
	
	int numOfSubnet = m_config.numTimeInterval - 1; 
	int L = m_config.subnetLayerNumber;     // layer number in each subnet  
	delta_t = m_config.totalTime / m_config.numTimeInterval;  
	root_rank = 0;  
	
	if (m_rank == root_rank) 
	{ 
		save_file_path = "output/" + m_config.modelSaveName + Utility<float>::currentTimeStampToString(); 
		SaveInit();  
	}
	
	std::default_random_engine generator; 
	std::uniform_real_distribution<float> distribution(m_config.yInitRange[0], m_config.yInitRange[1]);
	y_init = distribution(generator);  
	
	z_init.Resize(m_config.dim_input);  
	z_init.UniformDist((float)-0.1, (float)0.1);
	
	dy = 0.0;  
	dz.Resize(m_config.dim_input);  
	
	weights.resize(L * numOfSubnet);
	sum_weights.resize(L * numOfSubnet); 
	
	for (int t = 0; t < numOfSubnet; t++)  
	{ 
		// Each Subnet 
		for (int i = 0; i < L - 1; i++)  
		{
			int hidden = m_config.subnetHiddenLayerSizes[i]; 
			weights[i + t * L] = Matrix<float>(hidden, i == 0 ? m_config.dim_input : m_config.subnetHiddenLayerSizes[i - 1]);
		}
		weights[L - 1 + t * L] = Matrix<float>(m_config.dim_output, m_config.subnetHiddenLayerSizes.back()); 
		
		std::srand(123456789);  
		for (int i = 0; i < L; i++) 
		{
			weights[i + t * L].FanInFanOutRandomize(); 
			sum_weights[i + t * L].Resize(weights[i + t * L].Row(), weights[i + t * L].Col());  
		}
	}
	Setup();
	MPI_Barrier(MPI_COMM_WORLD);     
	start = MPI_Wtime();
}

bool BSDEModel::SaveInit() 
{ 
	int m_nprocs; 
	int mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &m_nprocs); 
	if (mpi_error != MPI_SUCCESS) 
		throw std::runtime_error("MPI_Comm_size failed with an error"); 
	
	std::ofstream fout(save_file_path, std::ios::app); 
	
	fout << "Dimension: " << m_config.dim_input << "\n"; 
	fout << "Total time: " << m_config.totalTime << "\n"; 
	fout << "Number of time intervals: " << m_config.numTimeInterval << "\n";
	fout << "y_init_range: " << m_config.yInitRange[0] << ", " << m_config.yInitRange[1] << "\n"; 
	fout << "Number of neurons in each hidden layer of each subnet: "; 
	for (int i = 0; i < m_config.subnetLayerNumber - 1; i++)
	{
		fout << m_config.subnetHiddenLayerSizes[i] << ", "; 
	}
	
	fout << "\nEpochs: " << m_config.train_epoch << "\n"; 
	fout << "Batch size: " << m_config.batchSize << "\n"; 
	fout << "Sample size: " << m_config.sampleSize << "\n"; 
	fout << "Logging frequency: " << m_config.logging_frequency << "\n"; 
	fout << "Number of processors: " << m_nprocs << "\n\n\nTraining:\n"; 
	
	return true;
}


bool BSDEModel::Save(int epoch)
{ 
	std::ofstream fout(save_file_path, std::ios::app); 
	end = MPI_Wtime(); 
	fout << "Epoch: " << epoch << ", Y0: " << y_init << ", Loss: " << total_loss << ", Runtime(sec): " << end - start << "\n";

	return true; 
}

void BSDEModel::Setup() 
{ 
	int numOfSubnet = m_config.numTimeInterval - 1;
	int L = m_config.subnetLayerNumber; 
	int dim = m_config.dim_input;  
	
	d_weights.resize(L * numOfSubnet);
	layers.resize(L * numOfSubnet);
	activate_layers.resize(L * numOfSubnet);
	gradient_layers.resize(L * numOfSubnet);
	y_layers.resize(numOfSubnet + 2); 
	z_layers.resize(numOfSubnet + 1);
	
	y_layers[0].Resize(1);
	z_layers[0].Resize(m_config.dim_input);
	
	for (int t = 0; t < numOfSubnet; t++) 
	{
		for (int i = 0; i < L; i++)
		{
			layers[i + t * L].Resize(dim); 
			activate_layers[i + t * L].Resize(dim);
			gradient_layers[i + t * L].Resize(dim); 
			d_weights[i + t * L].Resize(weights[i + t * L].Row(), weights[i + t * L].Col());
		} 
		y_layers[t + 1].Resize(1);
		z_layers[t + 1].Resize(m_config.dim_input); 
	}
	y_layers[numOfSubnet + 1].Resize(1); 
}

void BSDEModel::ComputeGradient(const Equation& equation, const float y, size_t index)
{
    int numOfSubnet = m_config.numTimeInterval - 1;
    int dim = m_config.dim_input;
    int L = m_config.subnetLayerNumber;

    float Yn_diffYn_1 = y_layers[numOfSubnet + 1][0] - y;
    
	for (int t = numOfSubnet - 1; t >= 0; t--) {
        Vector<float> diff_z;   
		equation.f_tf_diff_z(diff_z, z_layers[t + 1]); 
		
		if (diff_z.Size() == 0)   
		{  
			gradient_layers[(t + 1) * L - 1] = equation.GetDwSample()[index][t + 1]; 
		}
		else 
		{ 
			(gradient_layers[(t + 1) * L - 1] = diff_z).Mul(-delta_t).Add(equation.GetDwSample()[index][t + 1]); 
		}
		
		gradient_layers[(t + 1) * L - 1].Mul(Yn_diffYn_1).Mul((float)(1.0 / dim));

		//(gradient_layers[(t + 1) * L - 1] = equation.GetDwSample()[index][t + 1]).Mul(Yn_diffYn_1).Mul((float)(1.0 / dim));

		for (int i = L - 2; i >= 0; i--)
        {
            gradient_layers[i + t * L].AssignMulTMat(weights[i + 1 + t * L],
                gradient_layers[i + 1 + t * L]);
            gradient_layers[i + t * L].Mul(activate_layers[i + t * L])
                .MulScalarVecSub(1.0f, activate_layers[i + t * L]);

            /*Vector<float> reluDiff = activate_layers[i + t * L];
            gradient_layers[i + t * L].Mul(reluDiff.ReluDiff());*/
         }

        for (int i = 0; i < L; i++)
        {
            d_weights[i + t * L].AddMul(gradient_layers[i + t * L],
                i > 0 ? activate_layers[i - 1 + t * L] : equation.GetXSample()[index][t + 1]);
        }

        Yn_diffYn_1 *= 1 - delta_t * equation.f_tf_diff_y(y_layers[t + 1][0]);
	}
	
	Vector<float> diff_z;
	equation.f_tf_diff_z(diff_z, z_init); 
	if (diff_z.Size() == 0)
	{
		dz.AddMul(Yn_diffYn_1, equation.GetDwSample()[index][0]);
	}
	else  
	{
		diff_z.Mul(-delta_t).Add(equation.GetDwSample()[index][0]); 
		dz.AddMul(Yn_diffYn_1, diff_z); 
	}

	//dz.AddMul(Yn_diffYn_1, equation.GetDwSample()[index][0]);
	dy += Yn_diffYn_1 * (1 - delta_t * equation.f_tf_diff_y(y_layers[0][0]));

}

void BSDEModel::Update()
{
	int m_nprocs;
	int mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &m_nprocs);
	if (mpi_error != MPI_SUCCESS)       
		throw std::runtime_error("MPI_Comm_size failed with an error");
	int batch = m_config.batchSize / m_nprocs;
	batch = batch == 0 ? 1 : batch;

	int numOfSubnet = m_config.numTimeInterval - 1;
    int L = m_config.subnetLayerNumber;

    for (int t = 0; t < numOfSubnet; t++) {
        for (int i = 0; i < L; i++)
        {
            d_weights[i + t * L].Div((float)batch);
            weights[i + t * L].Sub(m_config.learning_rate, d_weights[i + t * L]);
        }

    }

	dy /= (float) batch;
	dz.Div((float) batch);

	y_init -= m_config.learning_rate * dy;
	z_init.Sub(m_config.learning_rate, dz);

}

float BSDEModel::Eval(const Equation& equation, size_t index)
{
    size_t numOfSubnet = m_config.numTimeInterval - 1;
    size_t dim = m_config.dim_input;
    size_t L = m_config.subnetLayerNumber;

    y_layers[0].Reset(y_init);
    //Vector<float> z(z_init);
    z_layers[0] = z_init;

	for (size_t t = 0; t < numOfSubnet; t++)
    {   
        //y_layers[t+1][0] = y_layers[t][0] - delta_t * equation.f_tf(0.0, equation.GetXSample()[index][t], y_layers[t][0], z) + Vector<float>::ReduceSum(z, equation.GetDwSample()[index][t]);
		
		y_layers[t+1][0] = y_layers[t][0] - delta_t * equation.f_tf(0.0, equation.GetXSample()[index][t], y_layers[t][0], z_layers[t]) + Vector<float>::ReduceSum(z_layers[t], equation.GetDwSample()[index][t]);
        
		for (size_t i = 0; i < L; i++)
        {
            layers[i + t * L].AssignMul(weights[i + t * L],
                i == 0 ? equation.GetXSample()[index][t + 1] : activate_layers[i - 1 + t * L]);

            activate_layers[i + t * L] = layers[i + t * L];
            if (i != L - 1) activate_layers[i + t * L].Sigmod();
            else activate_layers[i + t * L];
        }
		z_layers[t+1] = activate_layers[L - 1 + t * L];   
		z_layers[t+1].Mul((float)(1.0 / dim));

		//z = activate_layers[L - 1 + t * L];
        //z.Mul((float)(1.0 / dim));
    }

    //y_layers[numOfSubnet + 1][0] = y_layers[numOfSubnet][0] - delta_t * equation.f_tf(0.0, equation.GetXSample()[index][numOfSubnet], y_layers[numOfSubnet][0], z) + Vector<float>::ReduceSum(z, equation.GetDwSample()[index][numOfSubnet]);

	y_layers[numOfSubnet + 1][0] = y_layers[numOfSubnet][0] - delta_t * equation.f_tf(0.0, equation.GetXSample()[index][numOfSubnet], y_layers[numOfSubnet][0], z_layers[numOfSubnet]) + Vector<float>::ReduceSum(z_layers[numOfSubnet], equation.GetDwSample()[index][numOfSubnet]);
    
	y_hat = y_layers[numOfSubnet + 1][0];
    return y_hat;
}

void BSDEModel::ClearGradient()
{
    int numOfSubnet = m_config.numTimeInterval - 1;
    int L = m_config.subnetLayerNumber;

    for (int t = 0; t < numOfSubnet; t++) {
        for (int i = 0; i < L; i++)
        {
            d_weights[i + t * L] = 0;
        }
    }

	dy = 0.0;
	dz.Reset(0.0);
}

float BSDEModel::Loss(float y, float y_pred)
{
    return (float) (0.5 * pow(y_pred - y, 2.0));
}

