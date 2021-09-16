#ifndef __DATASET_H__
#define __DATASET_H__

#include <string>
#include "Vector.h"

class DataSet
{
public:
    DataSet() = default;
    virtual bool Load(const std::string & dataFile, const std::string & labelFile) = 0;
    const Vector<float>& GetData(size_t index) const;
    const Vector<float>& GetTarget(size_t index) const;

    size_t Size() const
    {
        return m_allData.size();
    }

protected:
    std::vector<Vector<float>> m_allData;
    std::vector<Vector<float>> m_targets;
   
};


#endif