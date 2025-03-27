//
// cSatellite.h
//
// Copyright (c) 2014 Michael F. Henry
// Version 07/2014
//
#pragma once

#include <string>
#include "cTle.h"
#include "cEci.h"
#include "cOrbit.h"

using namespace Zeptomoby::OrbitTools;

namespace Zeptomoby 
{
namespace OrbitTools
{

class TLESHARED_EXPORT cSatellite
{
public:
    cSatellite() {
        m_pOrbit = 0x0;
        m_pName = 0x0;
    }
   cSatellite(const cTle& tle, const std::string* pName = NULL);
   cSatellite(const cSatellite& src);
   cSatellite& operator=(const cSatellite& rhs);
   ~cSatellite();

   string   Name() const;
   cEciTime PositionEci(const cJulian& time) const;
   cEciTime PositionEci(double mpe) const;

   void updateTLE(const cTle& newTle);

   const cOrbit& Orbit() const { return *m_pOrbit; }
   const string& PName() const { return *m_pName; }

    void updateIncl(double incl)
    {
        this->m_pOrbit->updateIncl(incl);
    }

    void updateRAAN(double incl)
    {
        this->m_pOrbit->updateRAAN(incl);
    }

    void updateEccentricity(double ecc)
    {
        this->m_pOrbit->updateEccentricity(ecc);
    }

    void updateArgPerigee(double arg)
    {
        this->m_pOrbit->updateArgPerigee(arg);
    }

    void updateMeanMotion(double a1)
    {
        this->m_pOrbit->updateMeanMotion(a1);
    }

    void updateMeanAnomaly(double ma)
    {
        this->m_pOrbit->updateMeanAnomaly(ma);
    }

    void updateData()
    {
        this->m_pOrbit->updateData();
    }

protected:
   cOrbit* m_pOrbit;
   string* m_pName;
};

}
}
