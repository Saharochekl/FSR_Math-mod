#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H


class atmosphere
{
public:
    atmosphere();
    virtual bool getDensity(double* pos, double t, double* d);
};

#endif // ATMOSPHERE_H
