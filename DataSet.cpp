#include "DataSet.h"

const Vector<float>& DataSet::GetData(size_t index) const
{
    assert(index < m_allData.size());
    return m_allData[index];
}

const Vector<float>& DataSet::GetTarget(size_t index) const
{
    return m_targets[index];
}