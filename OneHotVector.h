#ifndef __ONEHOTVECTOR_H__
#define __ONEHOTVECTOR_H__
class OneHotVector
{
public:
    OneHotVector() = default;
    OneHotVector(size_t index, size_t len) : m_Len(len), m_index(index) {}
    virtual ~OneHotVector() {}
    size_t Size() const
    {
        return this->m_Len;
    }
    size_t HotIndex() const
    {
        return m_index;
    }

    template<class T>
    friend class Vector;

    template<class T>
    friend class Matrix;

private:
    size_t m_index;
    size_t m_Len;
};

#endif