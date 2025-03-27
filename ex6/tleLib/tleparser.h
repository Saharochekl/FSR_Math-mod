#ifndef TLEPARSER_H
#define TLEPARSER_H

#include "tleLib.h"

#include <QList>
#include <QTextStream>

#include "core/cTle.h"


struct TLESHARED_EXPORT TLEUnit{

    QString line1;
    QString line2;
    QString line3;

    TLEUnit()
    {

    }

    TLEUnit(const TLEUnit& unit)
    {
        this->line1 = unit.line1;
        this->line2 = unit.line2;
        this->line3 = unit.line3;
    }
};

class TLESHARED_EXPORT TLEParser
{
public:
    TLEParser();
    QList<TLEUnit> getTLEUnitsFromFile(QString pathToFile);
};

#endif // TLEPARSER_H
