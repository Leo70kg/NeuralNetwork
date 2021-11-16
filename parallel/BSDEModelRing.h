#ifndef __BSDEMODELRING_H__
#define __BSDEMODELRING_H__

#include <memory>
#include <time.h>
#include <string>
#include <mpi.h>
#include "BSDEModel.h"
#include "Vector.h"
#include "Equation.h"
#include "BSDEConfiguration.h"


class BSDEModelRing : public BSDEModel
{
public:
    BSDEModelRing() = default;
    BSDEModelRing(const BSDEConfiguration& _config) : BSDEModel(_config) {}
    /*
	void Setup();
    void ComputeGradient(const Equation& equation, const float y, size_t index);
    void Update();
	float Eval(const Equation& equation, size_t index);
    void ClearGradient();
    float Loss(float y, float y_pred);
	bool SaveInit() override;
	bool Save(int epoch) override;
	*/
	void Fit(const Equation& equation) override;
  
    /*
	const BSDEConfiguration& m_config;
    const int m_rank;
	const int m_nprocs;
	int root_rank;
	std::vector<Matrix<float>> weights;
    std::vector<Matrix<float>> d_weights;
    std::vector<Vector<float>> layers;
    std::vector<Vector<float>> activate_layers;
    std::vector<Vector<float>> gradient_layers;
    std::vector<Vector<float>> y_layers;
	std::vector<Vector<float>> z_layers;

	// Used in MPI_Reduce: to sum up the updated weights in each process.
	std::vector<Matrix<float>> sum_weights;

	float dy;
	Vector<float> dz;

    float y_init;
    Vector<float> z_init;

    float delta_t;
    float y_hat;
    //float loss;
	float total_loss;

	std::string save_file_path;
	
	int root_rank;  
	// Used in MPI_Reduce: to sum up the updated weights in each process.
	std::vector<Matrix<float>> sum_weights;
	float total_loss;
	double start, end;
	*/
};



#endif
