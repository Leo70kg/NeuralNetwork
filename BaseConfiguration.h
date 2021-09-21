#ifndef __BASECONFIGURATION_H__
#define __BASECONFIGURATION_H__

#include <string>
#include <vector>
#include <memory>

class BaseModel;

class BaseConfiguration
{
public:
    BaseConfiguration() = default;
    static std::unique_ptr<BaseConfiguration> CreateConfiguration(const std::string& filePath);
    virtual bool Load(const std::string& filePath);
    virtual bool Save(const std::string& filePath);
    virtual std::unique_ptr<BaseModel> CreateModel();
    size_t train_epoch;
    float learning_rate;
    int category_number;
    int feature_number;
    int batchSize;
    int LayerNumber;
    std::vector<int> hiddenLayerSizes;

    std::string trainingDataPath;
    std::string trainingLabelPath;
    std::string validateDataPath;
    std::string validateLabelPath;
    std::string modelSavePath;

};


#endif