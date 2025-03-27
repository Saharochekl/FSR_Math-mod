#include "tleparser.h"

using namespace Zeptomoby::OrbitTools;

TLEParser::TLEParser()
{
}



QList<TLEUnit> TLEParser::getTLEUnitsFromFile(QString pathToFile)
{
    QList<TLEUnit> units;
    units.clear();



    QFile* tle = new QFile(pathToFile);
    QTextStream in(tle);


    bool isOk = false;

    if(tle->open(QIODevice::ReadOnly))
    {

        isOk = true;

        QString NAME; NAME.clear();
        QString OLE1; OLE1.clear();
        QString OLE2; OLE2.clear();

        while(!in.atEnd())
        {

            NAME = in.readLine();

            //Check whether current set contains line_zero


            if(!cTle::IsValidLine(NAME.toStdString(), cTle::eTleLine::LINE_ONE))
            {

                //Удаляем номер строки и пробел перед именем
                if(NAME.startsWith("0 "))
                {
                        NAME.remove(0, 2);
                }

                OLE1 = in.readLine();

            }
            else
            {
                OLE1 = NAME;
            }



            isOk = isOk && (!OLE1.isEmpty());

            if(!isOk)
            {
                break;
            }

            isOk = isOk && cTle::IsValidLine(OLE1.toStdString(), cTle::eTleLine::LINE_ONE);


            OLE1.insert(OLE1.size() - 1, "\0");

            OLE2= in.readLine();

            isOk = isOk && cTle::IsValidLine(OLE2.toStdString(), cTle::eTleLine::LINE_TWO);


            if(!isOk)
            {
                break;
            }

            OLE2.insert(OLE2.size() - 1, "\0");

            TLEUnit unit;

            unit.line1 = NAME;
            unit.line2 = OLE1;
            unit.line3 = OLE2;

            units.push_back(unit);

        }
        tle->close();
    }



    return units;
}


