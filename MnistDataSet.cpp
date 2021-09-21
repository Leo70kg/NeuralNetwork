#include "MnistDataSet.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool MnistDataSet::Load(const std::string& dataFile, const std::string& labelFile)
{
    LoadData(dataFile);
    LoadLabels(labelFile);
    return true;
}
bool MnistDataSet::Dump(const std::string& filePath)
{
    std::ofstream fout(filePath.c_str());
    fout << "samples:" << m_allData.size() << std::endl;
    fout << "categories:" << m_categoryNumber << std::endl;
    for (int i = 0; i < m_allData.size(); i++)
    {
        if (i % 1000 == 0)
        {
            std::cout << i << " samples dumped" << std::endl;
        }
        fout << "sample " << i << ": label: " << m_targets[i].HotIndex() << std::endl;
        m_allData[i].Print(fout, col);
        fout << "====================================" << std::endl;
    }
    return true;
}

bool MnistDataSet::LoadData(const std::string& dataFile)
{
    std::ifstream fin(dataFile.c_str(), std::ifstream::binary);
    if (!fin)
        return false;

    std::string line;
    std::vector<float> fields;

    while (getline(fin, line))
    {
        std::istringstream sin(line);
        fields.clear();
        std::string field;
        while (getline(sin, field, ',')) {
            fields.push_back(std::stof(field));
        }
        Vector<float> v(fields);
        m_allData.push_back(v);
    }
    fin.close();
    std::cout << "Input data loaded successfully!" << std::endl;

    return true;
}

bool MnistDataSet::LoadLabels(const std::string& labelFile)
{
    std::ifstream fin(labelFile.c_str(), std::ifstream::binary);
    if (!fin)
        return false;

    while (!fin.eof())
    {
        float num;
        std::string sTmp;
        fin >> num;
        getline(fin, sTmp);
        m_targets.push_back(OneHotVector((size_t)num, m_categoryNumber));
    }
    fin.close();
    std::cout << "Label data loaded successfully!" << std::endl;

    return true;
}