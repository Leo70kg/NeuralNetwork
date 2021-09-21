#ifndef __BASEDATASET_H__
#define __BASEDATASET_H__

#include <string>
#include "Vector.h"

class BaseDataSet
{
public:
    BaseDataSet() = default;
    BaseDataSet(int categoryNumber);
    virtual bool Load(const std::string & dataFile, const std::string & labelFile) = 0;
    const Vector<float>& GetData(size_t index) const;
    const OneHotVector& GetTarget(size_t index) const;
    size_t Size() const
    {
        return m_allData.size();
    }

protected:
    std::vector<Vector<float>> m_allData;
    std::vector<OneHotVector> m_targets;
    int m_categoryNumber;
};


#endif