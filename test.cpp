#include <stdio.h>
#include <boost\math\differentiation\autodiff.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include "Vector.h"
#include "Matrix.h"
#include "BPNNDataSet.h"
#include <iostream>
#include <iomanip> 
#include "BPNNConfiguration.h"
#include "BPNNModel.h"
#include "BSDEDataSet.h"
#include "BSDEModel.h"
#include "BSDEConfiguration.h"
#include "LRModel.h"
#include "Configuration.h"
#include "MnistDataSet.h"
#include "BaseDataSet.h"
#include "MnistModel.h"
#include "Utility.h"
#include "PricingDefaultRisk.h"

using namespace boost::math::differentiation;

promote<float> f(float t, const Vector<float>& x, float y, const Vector<float>& z)
{
	float sigma = 0.2;
	float rate = 0.02;   // interest rate R
	float delta = 2.0 / 3;
	float gammah = 0.2;
	float gammal = 0.02;
	float mu_bar = 0.02;
	float vh = 50.0;
	float vl = 70.0;
	float slope = (gammah - gammal) / (vh - vl);

	float piecewise_linear = Utility<float>::Relu(Utility<float>::Relu(y - vh) * slope + gammah - gammal) + gammal;

	return (-(1 - delta) * piecewise_linear - rate) * y;

	
}

int main(int argc, char* argv[])
{
	/*std::vector<float> vec1 = { 1.0,2.0 ,3.0 };
	std::vector<float> vec2 = { 4.0,5.0,6.0 };*/
	//std::vector<std::vector<int>> vec3(3, std::vector<int>(3, 2));
	//for (int i = 0; i < vec3.size(); i++) {
	//	for (int j = 0; j < vec3[0].size(); j++) {
	//		vec3[i][j] = i * vec3.size() + j + 1;
	//	}
	//}

	/*Vector<float> x(vec1);
	Vector<float> z(vec2);*/

	//v1.Add(v2);
	//v1.Print();

	//v1.Add(2, v2);
	//v1.Print();

	//v1.Sub(2, v2);
	//v1.Print();
	//v1.Sub(v2);
	//v1.Print();
	//
	//v1.Mul(v2);
	//v1.Print();

	//v1.Div(2);
	//v1.Print();

	///*v1.MulScalarVecSub(2, v2);
	//v1.Print();*/
	//std::cout << std::endl;

	//Matrix<int> m(vec3);
	//std::cout << m.Row() << " " << m.Col() << std::endl;
	//for (int i = 0; i < m.Row(); i++) {
	//	for (int j = 0; j < m.Col(); j++) {
	//		std::cout << m.Data()[i * m.Col() + j] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	//Vector<int> v3(v1.Size());
	//v3.Print();
	///*v3.AddMul(m, v1);
	//v3.Print();*/
	//
	//v3.AssignMulTMat(m, v1);
	//v3.Print();

	//Matrix<int> m1(3, 3);
	//m1.AddMul(v1, v2);
	//std::cout << std::endl;
	//for (int i = 0; i < m1.Row(); i++) {
	//	for (int j = 0; j < m1.Col(); j++) {
	//		std::cout << m1.Data()[i * m1.Col() + j] << " ";
	//	}
	//	std::cout << std::endl;
	//}
	///*
	//Vector<int> v1;
	//Vector<double> v2(10);
	//Vector<int> v5(v4);
	//Vector<int> v6(v5.Data(), v5.Size());

	//v1.Resize(5);
	//

	//v1.Print();
	//v2.Print();
	//v4.Print();
	//v5.Print();
	//v6.Print();

	//v6 = 3;
	//v6.Print();

	//v6 += v5;
	//v6.Print();

	//std::pair<int, size_t> pair = v6.Max();
	//std::cout << pair.first << " " << pair.second << std::endl;
	//v6.Add(v5);
	//v6.Print();

	//v6 -= v5;
	//v6.Print();*/

	/*DataSet* dataSet = new BPNNDataSet();
	dataSet->Load("data.txt", "label.txt");
	
	Vector<float> v(dataSet->GetData(0));
	v.Print();
	Vector<float> t(dataSet->GetTarget(0));
	t.Print();

	Configuration* config = new Configuration();
	config->Load("config.txt");
	
	std::cout << config->feature_number << " " << config->category_number << std::endl;
	std::cout << config->learning_rate << " " << config->batchSize << std::endl;
	std::cout << config->train_epoch << std::endl;
	
	std::unique_ptr<Model> lr = config->CreateModel();
	
	lr->Fit(*dataSet, *dataSet);

	delete config;
	delete dataSet;*/

	/*DataSet* dataSet = new BPNNDataSet();
	dataSet->Load("data.txt", "label.txt");

	Vector<float> v(dataSet->GetData(0));
	v.Print();
	Vector<float> t(dataSet->GetTarget(0));
	t.Print();

	BPNNConfiguration* config = new BPNNConfiguration();
	config->Load("config1.txt");

	std::unique_ptr<Model> lr = config->CreateModel();

	lr->Fit(*dataSet, *dataSet);

	delete config;
	delete dataSet;*/

	//BaseDataSet* dataSet = new MnistDataSet(10);
	//dataSet->Load("mnist_train_data.csv", "mnist_train_label.csv");

	//BaseDataSet* dataSetTest = new MnistDataSet(10);
	//dataSetTest->Load("mnist_test_data.csv", "mnist_test_label.csv");

	//Vector<float> v(dataSet->GetData(0));
	////v.Print();
	//OneHotVector t(dataSet->GetTarget(0));

	//BaseConfiguration* config = new BaseConfiguration();
	//config->Load("test.txt");

	//std::cout << config->feature_number << " " << config->category_number << std::endl;
	//std::cout << config->learning_rate << " " << config->batchSize << std::endl;
	//std::cout << config->train_epoch << std::endl;

	//std::unique_ptr<BaseModel> lr = config->CreateModel();

	//lr->Fit(*dataSet, *dataSetTest);

	//delete config;
	//delete dataSet;
	//delete dataSetTest;

	//using namespace boost::math::differentiation;

	//constexpr unsigned Order = 5;                  // Highest order derivative to be calculated.
	//auto const x = make_fvar<double, Order>(2.0);  // Find derivatives at x=2.
	//auto const y = fourth_power(x);
	//for (unsigned i = 0; i <= Order; ++i)
	//	std::cout << "y.derivative(" << i << ") = " << y.derivative(i) << std::endl;

	clock_t start, end;
	start = clock();
	BSDEConfiguration* config = new BSDEConfiguration();
	config->Load("pricing_default_risk_d100.txt");

	std::cout << config->feature_number << " " << config->category_number << std::endl;
	std::cout << config->learning_rate << " " << config->batchSize << std::endl;
	std::cout << config->train_epoch << std::endl;
	std::cout << config->totalTime << std::endl;
	std::cout << config->numTimeInterval << std::endl;
	std::cout << config->subnetLayerNumber << std::endl;
	std::cout << config->sampleSize << std::endl;
	std::cout << config->yInitRange[0] << " " << config->yInitRange[1] << std::endl;
	std::cout << config->subnetHiddenLayerSizes[0] << " " << config->subnetHiddenLayerSizes[1] << std::endl;

	Equation* pricingDefaultRisk = new PricingDefaultRisk(*config);

	std::unique_ptr<BSDEModel> lr = config->CreateModel();
	lr->Fit(*pricingDefaultRisk);

	delete config;
	delete pricingDefaultRisk;
	end = clock();

	std::cout << "Time consuming: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
	return 0;
}

