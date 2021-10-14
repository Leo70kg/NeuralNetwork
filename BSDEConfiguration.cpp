#include <cassert>
#include "BSDEConfiguration.h"
#include "BSDEModel.h"

bool BSDEConfiguration::Load(const std::string& filePath)
{
    std::ifstream fin(filePath.c_str(), std::ifstream::binary);
    if (!fin)
        return false;

    std::vector<float> config_model_param;
    std::vector<std::string> config_file_param;

    int count = 0;
    while (!fin.eof())
    {
        std::string s;
        getline(fin, s);

		if (s.length() == 0) break;
		size_t i = 0;
        for (; i < s.length(); i++) { if (s[i] == ':') break; }
		
        // remove the first chars, which aren't digits
        s = s.substr(i + 1, s.length() - i);
        s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

        if (count < 9)
        {
            // convert the remaining text to an integer
            config_model_param.push_back(std::stof(s.c_str()));
        }
        else if (count == 9)
        {
            size_t pos = 0;
            std::string token;
            std::string delimiter = ",";
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                this->yInitRange.push_back(std::stof(token));
                s.erase(0, pos + delimiter.length());
            }
            this->yInitRange.push_back(std::stof(s));
        }
        else if (count == 10)
        {
            size_t pos = 0;
            std::string token;
            std::string delimiter = ",";
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                this->subnetHiddenLayerSizes.push_back(std::stoi(token));
                s.erase(0, pos + delimiter.length());
            }
            this->subnetHiddenLayerSizes.push_back(std::stoi(s));
        }
        else
        {
            config_file_param.push_back(s);
		}
        ++count;
    }

    fin.close();
    this->dim_input = (int)config_model_param[0];
    this->dim_output = (int)config_model_param[1];
    this->batchSize = (int)config_model_param[2];
    this->learning_rate = config_model_param[3];
    this->train_epoch = (int)config_model_param[4];
    this->totalTime = config_model_param[5];
    this->numTimeInterval = (int)config_model_param[6];
    this->sampleSize = (int)config_model_param[7];
    this->subnetLayerNumber = (int)config_model_param[8];

    this->modelSavePath = config_file_param[0];
    std::cout << "Config data loaded successfully!" << std::endl;

    return true;
}

bool BSDEConfiguration::Save(const std::string& filePath)
{
    return true;
}

std::unique_ptr<BSDEModel> BSDEConfiguration::CreateModel()
{
    return std::unique_ptr<BSDEModel>(new BSDEModel(*this));
}

std::unique_ptr<BSDEConfiguration> BSDEConfiguration::CreateConfiguration(const std::string& filePath)
{
    BSDEConfiguration basicConfig;
    basicConfig.Load(filePath);
    std::unique_ptr<BSDEConfiguration> config = NULL;
    
    config = std::unique_ptr<BSDEConfiguration>(new BSDEConfiguration());

    assert(config != NULL);
    config->Load(filePath);
    return config;
}
