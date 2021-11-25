#ifndef __BSDEMODEL_H__
#define __BSDEMODEL_H__

#include <memory>
#include <time.h>
#include <string>
#include <mpi.h>
#include "Vector.h"
#include "Equation.h"
#include "BSDEConfiguration.h"


class BSDEModel
{
public:
    BSDEModel() = default;
	BSDEModel(const BSDEConfiguration& _config);
	
    void ComputeGradient(const Equation& equation, const float y, size_t index);
    void Update();
	float Eval(const Equation& equation, size_t index);
    void ClearGradient();
    float Loss(float y, float y_pred);
	void Setup();
	bool SaveInit();
	bool Save(int epoch);
	virtual void Fit(const Equation& equation) = 0;

protected:
    const BSDEConfiguration& m_config;
	std::vector<Matrix<float>> weights;
    std::vector<Matrix<float>> d_weights;
    std::vector<Vector<float>> layers;
    std::vector<Vector<float>> activate_layers;
    std::vector<Vector<float>> gradient_layers;
    std::vector<Vector<float>> y_layers;
	std::vector<Vector<float>> z_layers;
	
	int root_rank;
	// Used in MPI_Reduce: to sum up the updated weights in each process.
	std::vector<Matrix<float>> sum_weights;

	float dy;
	Vector<float> dz;

    float y_init;
    Vector<float> z_init;

    float delta_t;
    float y_hat;
	float total_loss;

	std::string save_file_path;
	double start, end;
};



#endif