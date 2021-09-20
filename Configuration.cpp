#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cctype>
#include "Configuration.h"
#include "LRModel.h"
#include "BPNNModel.h"


bool Configuration::Load(const std::string& filePath)
{
    std::ifstream fin(filePath.c_str(), std::ifstream::binary);
    if (!fin)
        return false;

    std::vector<float> config_model_param;
    std::vector<std::string> config_file_param;
    
    int count = 0;
    while (!fin.eof())
    {   
        //float num;
        std::string s;
        getline(fin, s);
        size_t i = 0;
        for (; i < s.length(); i++) { if (s[i] == ':') break; }

        // remove the first chars, which aren't digits
        s = s.substr(i+1, s.length() - i);
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

        if (count < 5) 
        {
            // convert the remaining text to an integer
            config_model_param.push_back(std::stof(s.c_str()));
        }
        else
        {
            config_file_param.push_back(s);
        }
        ++count;

    }
    fin.close();
    this->feature_number = config_model_param[0];
    this->category_number = config_model_param[1];
    this->batchSize = config_model_param[2];
    this->learning_rate = config_model_param[3];
    this->train_epoch = config_model_param[4];
    this->type = config_file_param[0].compare("LR") == 0 ? ModelType::ModelType_LR : ModelType::ModelType_BPNN;
    this->modelSavePath = config_file_param[1];

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