#include <stdio.h>
#include "Vector.h"
#include "Matrix.h"
#include "BPNNDataSet.h"
#include <iostream>
#include <iomanip> 
#include "BPNNConfiguration.h"
#include "BPNNModel.h"
#include "LRModel.h"
#include "Configuration.h"


int main(int argc, char* argv[])
{
	
	//std::vector<int> vec1 = { 1,2,3 };
	//std::vector<int> vec2 = { 4,5,6 };
	//std::vector<std::vector<int>> vec3(3, std::vector<int>(3, 2));
	//for (int i = 0; i < vec3.size(); i++) {
	//	for (int j = 0; j < vec3[0].size(); j++) {
	//		vec3[i][j] = i * vec3.size() + j + 1;
	//	}
	//}

	//Vector<int> v1(vec1);
	//Vector<int> v2(vec2);

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

	DataSet* dataSet = new BPNNDataSet();
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
	delete dataSet;

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

	return 0;
}