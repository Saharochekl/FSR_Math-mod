#include "gravitymodel.h"

GravityModel::GravityModel(double mu) : mu(mu) {}

std::vector<double> GravityModel::calculateNextState(const std::vector<double>& state, double t, double dt)
{
    // state = [r_x, r_y, r_z, v_x, v_y, v_z]
    std::vector<double> nextState(6);

    std::vector<double> r = {state[0], state[1], state[2]};

    std::vector<double> v = {state[3], state[4], state[5]};

    double rMagnitude = std::sqrt(r[0]*r[0] + r[1]*r[1] + r[2]*r[2]);
    double acceleration = -mu/(rMagnitude * rMagnitude * rMagnitude);
    std::vector<double> accelerationVec = {acceleration * r[0], acceleration * r[1], acceleration * r[2]};

    for (int i = 0; i < 3; ++i) {
        nextState[i] = state[i] + v[i] * dt;
        nextState[i+3] = state[i+3] + accelerationVec[i] * dt;  // Обновление скорости
    }
    return nextState;
}
