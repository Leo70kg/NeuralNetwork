#ifndef __BSDECONFIGURATION_H__
#define __BSDECONFIGURATION_H__

#include <string>
#include <vector>
#include <memory>

class BSDEModel;

class BSDEConfiguration
{
public:
    
    BSDEConfiguration() = default;
	static std::unique_ptr<BSDEConfiguration> CreateConfiguration(const std::string & filePath);
    virtual bool Load(const std::string & filePath);
    virtual bool Save(const std::string & filePath);
    virtual std::unique_ptr<BSDEModel> CreateModel(const int rank, const int nprocs);
    
    int train_epoch;
    float learning_rate;

    // dimension of output value
    int dim_output;

    // dimension of input value
    int dim_input;
    int batchSize;
	int logging_frequency;
	std::string modelSaveName;

    float totalTime;
    int numTimeInterval;
    int subnetNumber;
    int subnetLayerNumber;
    int sampleSize;
    std::vector<float> yInitRange;
    std::vector<int> subnetHiddenLayerSizes;

};

#endif
