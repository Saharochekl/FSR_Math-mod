#include "sqpmodel.h"


cVector SqpModel::calculateNextState(double t){
    return sat->PositionEci((t - t0.toTime_t())/60.0).Position();
}
