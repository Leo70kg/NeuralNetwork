#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cctype>
#include "BaseConfiguration.h"
#include "MnistModel.h"


bool BaseConfiguration::Load(const std::string& filePath)
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
        s = s.substr(i + 1, s.length() - i);
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

        if (count < 6)
        {
            // convert the remaining text to an integer
            config_model_param.push_back(std::stof(s.c_str()));
        }
        else if (count == 6)
        {
            size_t pos = 0;
            std::string token;
            std::string delimiter = ",";
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                this->hiddenLayerSizes.push_back(std::stoi(token));
                s.erase(0, pos + delimiter.length());
            }
            this->hiddenLayerSizes.push_back(std::stoi(s));
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
    this->LayerNumber = config_model_param[5];
    this->modelSavePath = config_file_param[0];
    std::cout << "Config data loaded successfully!" << std::endl;

    return true;
}

bool BaseConfiguration::Save(const std::string& filePath)
{
    return true;
}

std::unique_ptr<BaseModel> BaseConfiguration::CreateModel()
{
    return std::unique_ptr<BaseModel>(new MnistModel(*this));
}

std::unique_ptr<BaseConfiguration> BaseConfiguration::CreateConfiguration(const std::string& filePath)
{
    BaseConfiguration basicConfig;
    basicConfig.Load(filePath);
    std::unique_ptr<BaseConfiguration> config = NULL;
    
    config = std::unique_ptr<BaseConfiguration>(new BaseConfiguration());
    

    assert(config != NULL);
    config->Load(filePath);
    return config;
}