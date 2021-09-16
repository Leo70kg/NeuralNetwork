#ifndef __BPNNCONFIGURATION_H__
#define __BPNNCONFIGURATION_H__

#include "Configuration.h"

class BPNNConfiguration : public Configuration
{
public:
	BPNNConfiguration() = default;
    virtual bool Load(const std::string& filePath);
    virtual bool Save(const std::string& filePath);
    int LayerNumber;
    std::vector<int> hiddenLayerSizes;
    virtual std::unique_ptr<Model> CreateModel() override;
};

#endif