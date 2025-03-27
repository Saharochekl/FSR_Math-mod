#ifndef GRAVITYMODEL_H
#define GRAVITYMODEL_H

#include <vector>
#include "reactionmodel.h"
#include <cmath>

class GravityModel : public ODEModel
{
public:
    GravityModel(double mu);
    std::vector<double> calculateNextState(const std::vector<double>& state, double t1, double t2);

private:
    double mu;
};

#endif // GRAVITYMODEL_H
