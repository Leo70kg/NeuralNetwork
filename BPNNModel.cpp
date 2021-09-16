#include "BPNNModel.h"

BPNNModel::BPNNModel(const BPNNConfiguration& config):Model(config), m_config(config)
{
    int L = m_config.LayerNumber;
    biases.resize(L);
    weights.resize(L);
    for (int i = 0; i < L - 1; i++)
    {
        int hidden = m_config.hiddenLayerSizes[i];
        biases[i] = Vector<float>(hidden);
        weights[i] = Matrix<float>(hidden,
            i == 0 ? m_config.feature_number : m_config.hiddenLayerSizes[i - 1]);
    }

    biases[L - 1] = Vector<float>(m_config.category_number);
    weights[L - 1] = Matrix<float>(m_config.category_number, m_config.hiddenLayerSizes.back());

    std::srand(123456789);
    for (int i = 0; i < L; i++)
    {
        weights[i].FanInFanOutRandomize();
    }

    Setup();
}

void BPNNModel::Setup()
{
    int L = m_config.LayerNumber;
    d_biases.resize(L);
    d_weights.resize(L);
    layers.resize(L);
    activate_layers.resize(L);
    gradient_layers.resize(L);
    for (int i = 0; i < L; i++)
    {
        size_t dim = biases[i].Size();
        d_biases[i].Resize(dim);
        layers[i].Resize(dim);
        activate_layers[i].Resize(dim);
        gradient_layers[i].Resize(dim);
        d_weights[i].Resize(weights[i].Row(), weights[i].Col());
    }
}
void BPNNModel::ComputeGradient(const Vector<float>& x, const Vector<float>& y)
{
    int L = m_config.LayerNumber;
    Vector<float>& y_hat = activate_layers[L - 1];
    (gradient_layers[L - 1] = y_hat).Sub(y);
    for (int i = L - 2; i >= 0; i--)
    {
        gradient_layers[i].AssignMulTMat(weights[i + 1],
            gradient_layers[i + 1]);
        gradient_layers[i].Mul(activate_layers[i])
            .MulScalarVecSub(1.0f, activate_layers[i]);
    }

    for (int i = 0; i < L; i++)
    {
        d_biases[i].Add(gradient_layers[i]);
        d_weights[i].AddMul(gradient_layers[i],
            i > 0 ? activate_layers[i - 1] : x);
    }
}

void BPNNModel::Update()
{
    int L = m_config.LayerNumber;
    for (int i = 0; i < L; i++)
    {
        d_biases[i].Div((float)m_config.batchSize);
        d_weights[i].Div((float)m_config.batchSize);
        biases[i].Sub(m_config.learning_rate, d_biases[i]);
        weights[i].Sub(m_config.learning_rate, d_weights[i]);
    }
}

size_t BPNNModel::Eval(const Vector<float>& data)
{
    int L = m_config.LayerNumber;
    for (int i = 0; i < L; i++)
    {
        layers[i].AssignMul(weights[i],
            i == 0 ? data : activate_layers[i - 1]).Add(biases[i]);

        activate_layers[i] = layers[i];
        if (i != L - 1) activate_layers[i].Sigmod();
        else activate_layers[i];
    }

    y_hat = activate_layers[L - 1];
    return y_hat.Max().second;
   
}

void BPNNModel::ClearGradient()
{
    for (int i = 0; i < m_config.LayerNumber; i++)
    {
        d_biases[i] = 0;
        d_weights[i] = 0;
    }
}

bool BPNNModel::Load(const std::string& filePath)
{
    std::ifstream fin(filePath.c_str());
    std::string str;
    fin >> str;
    for (int i = 0; i < m_config.LayerNumber; i++)
    {
        fin >> biases[i] >> weights[i];
    }

    Setup();
    return true;
}

bool BPNNModel::Save()
{
    std::ofstream fout(m_config.modelSavePath.c_str());
    fout << "BPNN" << std::endl;
    for (int i = 0; i < m_config.LayerNumber; i++)
    {
        fout << biases[i] << weights[i];
    }
    return true;
}