#include "reactionmodel.h"

ReactionModel::ReactionModel(std::vector<double> a)
{
    params=a;
}

ReactionModel::ReactionModel()
{
    params.push_back(1.0);
    params.push_back(2.5);
}

std::vector<double> ReactionModel::rightPart(double t, std::vector<double> x)
{
    std::vector<double> result;
    if (model_type==1){//my model
    }
    if (model_type==2){//sgp
    }
    return result;
}

void ReactionModel::set_params(std::vector<double> a){
    params.clear();
    params=a;
    return;
}

void ReactionModel::set_model_type(int mod){
    model_type=mod;
    return;
}
