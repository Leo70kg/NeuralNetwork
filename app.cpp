#include <stdio.h>
#include <iostream>
#include <iomanip> 
#include "Vector.h"
#include "Matrix.h"
#include "BSDEModel.h"
#include "BSDEConfiguration.h"
#include "Utility.h"
#include "PricingDefaultRisk.h"


int main(int argc, char* argv[])
{
	clock_t start, end;
	start = clock();
	BSDEConfiguration* config = new BSDEConfiguration();
	config->Load("pricing_default_risk.txt");

	/*
	std::cout << config->feature_number << " " << config->category_number << std::endl;
	std::cout << config->learning_rate << " " << config->batchSize << std::endl;  
	std::cout << config->train_epoch << std::endl;  
	std::cout << config->totalTime << std::endl; 
	std::cout << config->numTimeInterval << std::endl; 
	std::cout << config->subnetLayerNumber << std::endl; 
	std::cout << config->sampleSize << std::endl;  
	std::cout << config->yInitRange[0] << " " << config->yInitRange[1] << std::endl; 
	std::cout << config->subnetHiddenLayerSizes[0] << " " << config->subnetHiddenLayerSizes[1] << std::endl;
	*/
	Equation* pricingDefaultRisk = new PricingDefaultRisk(*config);

	std::unique_ptr<BSDEModel> lr = config->CreateModel();
	lr->Fit(*pricingDefaultRisk);

	delete pricingDefaultRisk;
	end = clock();

	std::cout << "Time consuming: " << (double)(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
	return 0;
}

