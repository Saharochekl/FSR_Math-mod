#ifndef REACTIONMODEL_H
#define REACTIONMODEL_H

#include "odemodel.h"

class ReactionModel : public ODEModel
{
    std::vector <double> params;
    int model_type;
public:
    ReactionModel(std::vector<double> a);
    ReactionModel();
    std::vector<double> rightPart(double t, std::vector<double> x);
    void set_params(std::vector<double> a);
    void set_model_type(int mod);
};

#endif // REACTIONMODEL_H
