#ifndef ODEMODEL_H
#define ODEMODEL_H

#include "model.h"

class ODEModel : public Model
{
private:
    std::vector<double> _state;
public:
    ODEModel();

    std::vector<double> rightPart(double t, std::vector<double> x);
};

#endif // ODEMODEL_H
