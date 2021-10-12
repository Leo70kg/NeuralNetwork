#ifndef __BPNNDATASET_H__
#define __BPNNDATASET_H__

#include "DataSet.h"

class BPNNDataSet : public DataSet
{
public:
	BPNNDataSet() = default;
    BPNNDataSet(BPNNDataSet& dataSet, int num)
    {
        m_allData.resize(num);
        std::copy(dataSet.m_allData.begin(), dataSet.m_allData.begin() + num, m_allData.begin());
        this->m_targets.resize(num);
        std::copy(dataSet.m_targets.begin(), dataSet.m_targets.begin() + num, m_targets.begin());
    }
    bool Dump(const std::string& filePath);
    virtual bool Load(const std::string& dataFile, const std::string& labelFile) override;
protected:
    bool LoadData(const std::string& dataFile);
    bool LoadLabels(const std::string& labelFile);
    int row;
    int col;
};

#endif