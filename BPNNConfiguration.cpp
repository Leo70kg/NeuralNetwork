#include <cassert>
#include "BPNNConfiguration.h"
#include "BPNNModel.h"

bool BPNNConfiguration::Load(const std::string& filePath)
{
    Configuration::Load(filePath);

    return true;
}

bool BPNNConfiguration::Save(const std::string& filePath)
{
    return true;
}

std::unique_ptr<Model> BPNNConfiguration::CreateModel()
{
    return std::unique_ptr<Model>(new BPNNModel(*this));
}