#ifndef SQPMODEL_H
#define SQPMODEL_H

#include "../tleLib/orbit/cSatellite.h"
#include "../tleLib/tleparser.h"
#include "qcustomplot.h"
#include <vector>
#include "reactionmodel.h"
#include <cmath>

class SqpModel : public Model
{
 cVector initpos, initvel;
 cSatellite * sat;
 QDateTime t0;
public:
     SqpModel(QString name)
     {
         TLEParser pars;
         QList<TLEUnit> units = pars.getTLEUnitsFromFile(name);
         cTle tleUnit(units[0].line1.toStdString().c_str(), units[0].line2.toStdString().c_str(), units[0].line3.toStdString().c_str());
         sat = new cSatellite(tleUnit);
         initpos = sat->PositionEci(0.0).Position();
         initvel = sat->PositionEci(0.0).Velocity();
         double epoch = sat->Orbit().Epoch().Date();

         QDate date = QDate::fromJulianDay(floor(epoch));
         double msecs = 24.0 * 3600.0 * 1000.0 * (epoch - floor(epoch));
         t0 = QDateTime(date, QTime::fromMSecsSinceStartOfDay(msecs));
     }
    cVector calculateNextState(double t);
    cVector get_initpos(){return initpos;}
    cVector get_initvel(){return initvel;}
    QDateTime get_t0(){return t0;}

};

#endif // SQPMODEL_H
