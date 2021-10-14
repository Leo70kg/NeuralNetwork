#include "BSDEModel.h"
#include "Utility.h"

BSDEModel::BSDEModel(const BSDEConfiguration& config) : m_config(config)
{   
    int numOfSubnet = m_config.numTimeInterval - 1;
    int L = m_config.subnetLayerNumber;     // layer number in each subnet
    delta_t = m_config.totalTime / m_config.numTimeInterval;

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(m_config.yInitRange[0], m_config.yInitRange[1]);
    y_init = distribution(generator);

    z_init.Resize(m_config.dim_input);
    z_init.UniformDist((float)-0.1, (float)0.1);
	
	dy = 0.0;
	dz.Resize(m_config.dim_input);
	
    weights.resize(L * numOfSubnet);

    for (int t = 0; t < numOfSubnet; t++) {
    
        // Each Subnet
        for (int i = 0; i < L - 1; i++)
        {
            int hidden = m_config.subnetHiddenLayerSizes[i];

			weights[i + t * L] = Matrix<float>(hidden,
                i == 0 ? m_config.dim_input : m_config.subnetHiddenLayerSizes[i - 1]);
        }

        weights[L - 1 + t * L] = Matrix<float>(m_config.dim_output, m_config.subnetHiddenLayerSizes.back());
          
        std::srand(123456789);
        for (int i = 0; i < L; i++)
        {
            weights[i + t * L].FanInFanOutRandomize();
        }
    }

    Setup();
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

    y_layers[0].Resize(1);

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
        //float L_diff_Yn = y_layers[numOfSubnet + 1][0] - y;

        (gradient_layers[(t + 1) * L - 1] = equation.GetDwSample()[index][t + 1]).Mul(Yn_diffYn_1).Mul((float)(1.0 / dim));

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
		//dy *= 1 - delta_t * equation.f_tf_diff_y(y_layers[t+1][0]);
	}
	dz.AddMul(Yn_diffYn_1, equation.GetDwSample()[index][0]);
	//dy *= 1 - delta_t * equation.f_tf_diff_y(y_layers[0][0]);
	
	dy += Yn_diffYn_1 * (1 - delta_t * equation.f_tf_diff_y(y_layers[0][0]));


	//std::cout << "Yn_diffYn_1: " << dy << std::endl;
    // y_init_diff += Yn_diffYn_1 * (1 - delta_t * equation.f_tf_diff_y(y_layers[0][0]));
	
	// (z_init_diff = equation.GetDwSample()[index][0]).Mul(Yn_diffYn_1);
	//z_init_diff_mean.Add(z_init_diff);
}

void BSDEModel::Update()
{
    /*std::cout << "Update started" << std::endl;*/
    int numOfSubnet = m_config.numTimeInterval - 1;
    int L = m_config.subnetLayerNumber;

    for (int t = 0; t < numOfSubnet; t++) {
        for (int i = 0; i < L; i++)
        {
            d_weights[i + t * L].Div((float)m_config.batchSize);
            weights[i + t * L].Sub(m_config.learning_rate, d_weights[i + t * L]);
        }

    }

	dy /= (float) m_config.batchSize;
	dz.Div((float)m_config.batchSize);

	y_init -= 0.01 * dy;
	z_init.Sub(0.01, dz);

    /*std::cout << "Update finished" << std::endl;*/
}

float BSDEModel::Eval(const Equation& equation, size_t index)
{
    size_t numOfSubnet = m_config.numTimeInterval - 1;
    size_t dim = m_config.dim_input;
    size_t L = m_config.subnetLayerNumber;

    y_layers[0].Reset(y_init);
    Vector<float> z(z_init);

    for (size_t t = 0; t < numOfSubnet; t++)
    {   
        y_layers[t+1][0] = y_layers[t][0] - delta_t * equation.f_tf(0.0, equation.GetXSample()[index][t], y_layers[t][0], z) +
            Vector<float>::ReduceSum(z, equation.GetDwSample()[index][t]);

        for (size_t i = 0; i < L; i++)
        {
            layers[i + t * L].AssignMul(weights[i + t * L],
                i == 0 ? equation.GetXSample()[index][t + 1] : activate_layers[i - 1 + t * L]);

            activate_layers[i + t * L] = layers[i + t * L];
            if (i != L - 1) activate_layers[i + t * L].Sigmod();
            else activate_layers[i + t * L];
        }
        z = activate_layers[L - 1 + t * L];
        z.Mul((float)(1.0 / dim));
    }
    y_layers[numOfSubnet + 1][0] = y_layers[numOfSubnet][0] - delta_t * equation.f_tf(0.0, equation.GetXSample()[index][numOfSubnet], y_layers[numOfSubnet][0], z) +
        Vector<float>::ReduceSum(z, equation.GetDwSample()[index][numOfSubnet]);

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

bool BSDEModel::Save(int epoch)
{
	std::string folder_name = "output/";
	std::string file_path = folder_name + m_config.modelSavePath.c_str();
    std::ofstream fout(file_path, std::ios::app);

    fout << "\nEpoch: " << epoch << ", Y0: " << y_init << ", Loss: " << loss;
    
    return true;
}

float BSDEModel::Loss(float y, float y_pred)
{
    return (float) (0.5 * pow(y_pred - y, 2.0));
}

// Use SGD
void BSDEModel::Fit(const Equation& equation)
{
	int numTimeInterval = m_config.numTimeInterval;

	for (int i = 0; i < m_config.train_epoch; i++)
    {   
		//std::cout << "echo: " << i << std::endl;
		loss = 0.0;
        
		float sum = 0.0;
		for (int j = 0; j < m_config.sampleSize; j++)
        {
			float y = equation.g_tf(0.0, equation.GetXSample()[j][numTimeInterval]);
			sum += y;  

            if (j % m_config.batchSize == 0)
            {
                ClearGradient();
            }

            float pred = Eval(equation, j);
            ComputeGradient(equation, y, j);
            
			//Update();
			loss += Loss(y, pred);
			
			//std::cout << pred - y << std::endl;
            if (j % m_config.batchSize == m_config.batchSize - 1 ||
                j == m_config.sampleSize - 1)
            {
                Update();
			}
			
			//float new_pred = Eval(equation, j);
			
			/*
			if (j < 3) {
				std::cout << "pred: " << pred << std::endl;
				std::cout << "new pred: " << new_pred << std::endl;
				std::cout << "true: " << y << std::endl;
			}
			*/
			/*
			delta1 = (new_pred - y) * dy;
			delta2 = dz.Mul(new_pred - y);
			
			//std::cout << "delta1: " << delta1 << std::endl;
			//std::cout << "dy: " << dy << std::endl;
			y_diff -= (0.005 * delta1);
			z_diff.Sub(delta2.Mul(0.001));
			*/
			//std::cout << "y_init: " << y_init << std::endl << std::endl;
        }
		
		std::cout << "y_init: " << y_init << std::endl;
		std::cout << "true mean: " << sum / m_config.sampleSize << std::endl;
		
		//y_init = y_diff;
		//z_init = z_diff;
		
		//y_init -= delta / m_config.sampleSize;
		

        if (i % 100 == 0)
        {
            Save(i);
            std::cout << "Epoch:" << i << std::endl;
            std::cout << "In training set, loss: " << loss / equation.GetXSample().size() << ", Y0: " << y_init << std::endl << std::endl;
        }

    }
    
}
