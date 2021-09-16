#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Configuration.h"
#include "LRModel.h"
#include "BPNNModel.h"


bool Configuration::Load(const std::string& filePath)
{
    std::ifstream fin(filePath.c_str(), std::ifstream::binary);
    if (!fin)
        return false;

    std::vector<float> config;
    while (!fin.eof())
    {
        float num;
        std::string sTmp;
        fin >> num;
        getline(fin, sTmp);//略过第一行
        config.push_back(num);
    }
    fin.close();
    this->feature_number = config[0];
    this->category_number = config[1];
    this->batchSize = config[2];
    this->learning_rate = config[3];
    this->train_epoch = config[4];
        
    std::cout << "Config data loaded successfully!" << std::endl;

    return true;
}

bool Configuration::Save(const std::string& filePath)
{
    return true;
}

std::unique_ptr<Model> Configuration::CreateModel()
{
    return std::unique_ptr<Model>(new LRModel(*this));
}

std::unique_ptr<Configuration> Configuration::CreateConfiguration(const std::string& filePath)
{
    Configuration basicConfig;
    basicConfig.Load(filePath);
    std::unique_ptr<Configuration> config = NULL;
    if (basicConfig.type == ModelType::ModelType_LR)
    {
        config = std::unique_ptr<Configuration>(new Configuration());
    }
    else if (basicConfig.type == ModelType::ModelType_BPNN)
    {
        config = std::unique_ptr<Configuration>(new BPNNConfiguration());
    }

    assert(config != NULL);
    config->Load(filePath);
    return config;
}