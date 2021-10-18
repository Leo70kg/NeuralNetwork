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
	BSDEConfiguration* config = new BSDEConfiguration();
	config->Load("config/pricing_default_risk.txt");

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

	Equation* pricingDefaultRisk = new PricingDefaultRisk(*config);

	std::unique_ptr<BSDEModel> lr = config->CreateModel();
	lr->Fit(*pricingDefaultRisk);

	delete pricingDefaultRisk;
	
	return 0;
}

