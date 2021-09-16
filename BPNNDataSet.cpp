#include "BPNNDataSet.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool BPNNDataSet::Load(const std::string& dataFile, const std::string& labelFile)
{
    LoadData(dataFile);
    LoadLabels(labelFile);
    return true;
}
bool BPNNDataSet::Dump(const std::string& filePath)
{
    std::ofstream fout(filePath.c_str());
    fout << "samples:" << m_allData.size() << std::endl;
    //fout << "categories:" << m_categoryNumber << endl;
    for (int i = 0; i < m_allData.size(); i++)
    {
        if (i % 1000 == 0)
        {
            std::cout << i << " samples dumped" << std::endl;
        }
        //fout << "sample " << i << ": label: " << m_targets[i].HotIndex() << std::endl;
        m_allData[i].Print(fout, col);
        fout << "====================================" << std::endl;
    }
    return true;
}

bool BPNNDataSet::LoadData(const std::string& dataFile)
{
    std::ifstream fin(dataFile.c_str(), std::ifstream::binary);
    if (!fin)
        return false;

    std::string feature; //�洢��ȡ��ÿ������
    float feat_onePoint;  //�洢ÿ�а��ո�ֿ���ÿһ��float����
    std::vector<float> lines; //�洢ÿ������
    
    while (!fin.eof())
    {
        getline(fin, feature); //һ�ζ�ȡһ������
        std::stringstream stringin(feature); //ʹ�ô���ʵ�ֶ�string�������������
        lines.clear();
        while (stringin >> feat_onePoint) {      //���ո�һ�ζ�ȡһ�����ݴ���feat_onePoint 
            lines.push_back(feat_onePoint); //�洢ÿ�а��ո�ֿ������� 
        }
        Vector<float> v(lines);
        m_allData.push_back(v); //�洢��������
    }
    fin.close();
    std::cout << "Input data loaded successfully!" << std::endl;

    return true;
}

bool BPNNDataSet::LoadLabels(const std::string& labelFile)
{
    std::ifstream fin(labelFile.c_str(), std::ifstream::binary);
    if (!fin)
        return false;

    std::vector<float> lines;
    while (!fin.eof())
    {
        float num;
        std::string sTmp;
        fin >> num;
        lines.clear();
        getline(fin, sTmp);//�Թ���һ��
        lines.push_back(num);
        Vector<float> v(lines);
        m_targets.push_back(v);
    }
    fin.close();
    std::cout << "Label data loaded successfully!" << std::endl;
    
    return true;
}