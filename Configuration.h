#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__
#include <string>
#include <vector>
#include <memory>

enum class ModelType
{
    ModelType_LR,
    ModelType_BPNN
};

class Model;

class Configuration
{
public:
    Configuration() = default;
    static std::unique_ptr<Configuration> CreateConfiguration(const std::string & filePath);
    virtual bool Load(const std::string & filePath);
    virtual bool Save(const std::string & filePath);
    virtual std::unique_ptr<Model> CreateModel();
    size_t train_epoch;
    float learning_rate;

    // dimension of output value
    int category_number;

    // dimension of inout value
    int feature_number;
    int batchSize;

    std::string trainingDataPath;
    std::string trainingLabelPath;
    std::string validateDataPath;
    std::string validateLabelPath;
    std::string modelSavePath;
    ModelType type;

};


#endif