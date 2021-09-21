#include "BaseDataSet.h"
#include "OneHotVector.h"

BaseDataSet::BaseDataSet(int categoryNumber) :
    m_categoryNumber(categoryNumber)
{
}

const Vector<float>& BaseDataSet::GetData(size_t index) const
{
    assert(index < m_allData.size());
    return m_allData[index];
}

const OneHotVector& BaseDataSet::GetTarget(size_t index) const
{
    return m_targets[index];
}