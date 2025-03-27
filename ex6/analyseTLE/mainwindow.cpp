#include "mainwindow.h"
#include "gravitymodel.h"
#include "sqpmodel.h"
#include <QDateTime>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->_satKeys = QStringList({"x", "y", "z"});

    QCustomPlot* customPlot = new QCustomPlot(this);
        ui->plotLayout->addWidget(customPlot);

        int itemRow = 0;

    for(auto key : this->_satKeys)
    {
        customPlot->addGraph();
        this->plots[key] = customPlot->graph(customPlot->graphCount() - 1);
        this->plots[key]->setPen(QPen(Qt::red));

        auto label = new QLabel(this);
        label->setText(key);

        auto btn = new QPushButton(this);
        btn->setText(tr("Color"));


        QString styleS = QString("QPushButton { border: 2px solid #8f8f91; \n"
                                                "border-radius: 10px; \n"
                                                "padding: 5px; \n"
                                                "background-color:" + QColor(Qt::red).name() + ";})");

        btn->setStyleSheet(styleS);

        this->labels[key] = label;
        this->selectColorButtons[key] = btn;
        QObject::connect(btn, SIGNAL(pressed()), this, SLOT(buttonPressedSlot()));

        ui->viewSetLayout->addWidget(label, itemRow, 0);
        ui->viewSetLayout->addWidget(btn, itemRow, 1);
        itemRow++;

    }

    for(auto key : this->_satKeys)
    {
        /*
        QVector<double> t, x, y;


        TLEParser pars;

       GravityModel mymodel(4.0e14);
       QList<TLEUnit> units = pars.getTLEUnitsFromFile(this->_satFiles[key]);

       //cVector vel = sat.PositionEci(0.0).Velocity();
       //vector<double> nextState = {pos.m_x, pos.m_y, pos.m_z, vel.m_x, vel.m_y, vel.m_z};
      for(int i = 0; i < units.size(); i++)
       {
          cTle tleUnit(units[0].line1.toStdString().c_str(), units[0].line2.toStdString().c_str(), units[0].line3.toStdString().c_str());
          cSatellite sat(tleUnit);
          cVector pos = sat.PositionEci(0.0).Position();
           double len1 = sqrt(pos.m_x*pos.m_x + pos.m_y*pos.m_y + pos.m_z*pos.m_z) - 6371.0;
           double epoch = sat.Orbit().Epoch().Date();

            QDate date = QDate::fromJulianDay(floor(epoch));
            double msecs = 24.0 * 3600.0 * 1000.0 * (epoch - floor(epoch));


            QDateTime dt(date, QTime::fromMSecsSinceStartOfDay(msecs));
            //nextState = mymodel.calculateNextState(nextState, 0.0, (dt.toTime_t() - t.last())/1000.0);
            //double len2 = sqrt(nextState[0]*nextState[0] + nextState[1]*nextState[1] + nextState[2]*nextState[2])*1000 - 6371.0;
           t.push_back(dt.toTime_t());
           x.push_back(len1);

           //cVector pos = sat.PositionEci(0.0).Position();
       }

       this->plots[key]->setData(t, x);
       this->plots[key]->rescaleAxes();

    }*/
        GravityModel mymodel(3.98e+14);
        SqpModel sqpmodel("stratosat-tk.txt");
        cVector pos = sqpmodel.get_initpos();
        cVector vel = sqpmodel.get_initvel();
        QDateTime t0 = sqpmodel.get_t0();
        QVector<double> T, x, y;



               vector<double> nextState = {pos.m_x*1000.0, pos.m_y*1000.0, pos.m_z*1000.0, vel.m_x*1000.0, vel.m_y*1000.0, vel.m_z*1000.0};
               //vector<double> nextState = {0, 3*6371000, 0, 1200, 0, 0};

               qDebug() << pos.m_x << " " << pos.m_y << " " << pos.m_z;
               QDateTime t1 = t0.addSecs(360000);
               double dt = 1.0;

               //double len2 = sqrt(nextState[0]*nextState[0] + nextState[1]*nextState[1] + nextState[2]*nextState[2])/1000;
               for(double t = t0.toTime_t(); t < t1.toTime_t(); t += dt)
               {
                    pos = sqpmodel.calculateNextState(t);

                    if(key == "x"){
                        double x1 = pos.m_x;
                        double x2 = nextState[0]/1000.0;
                        x.push_back(x1-x2);
                    } else if(key == "y"){
                        double y1 = pos.m_y;
                        double y2 = nextState[1]/1000.0;
                        x.push_back(y1-y2);
                    } else if(key == "z"){
                        double z1 = pos.m_z;
                        double z2 = nextState[2]/1000.0;
                        x.push_back(z1-z2);
                    }
                    T.push_back(t);
                    nextState = mymodel.calculateNextState(nextState, 0.0, dt);

               }

               this->plots[key]->setData(T, x);
               this->plots[key]->rescaleAxes();
    }


    QCPItemStraightLine *infLine = new QCPItemStraightLine(customPlot);
    infLine->point1->setCoords(0, 180);  // location of point 1 in plot coordinate
    infLine->point2->setCoords(1000, 180);  // location of point 2 in plot coordinate

    infLine->setPen(QPen(Qt::red));
    infLine->pen().setWidth(3);
        // configure right and top axis to show ticks but no labels:
        // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
        customPlot->xAxis2->setVisible(true);
        customPlot->xAxis2->setTickLabels(false);
        customPlot->yAxis2->setVisible(true);
        customPlot->yAxis2->setTickLabels(false);
        // make left and bottom axes always transfer their ranges to right and top axes:
        connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
        connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

        // configure bottom axis to show date instead of number:
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setTickOrigin(QDateTime(QDate(2000, 1, 1), QTime(12, 0)));

        dateTicker->setDateTimeFormat("dd.MM.yyyy\n hh:mm:ss");
        customPlot->xAxis->setTicker(dateTicker);

        // pass data points to graphs:
        //customPlot->graph(1)->setData(t, y);
        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:

        // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
        customPlot->graph(1)->rescaleAxes(true);
        // Note: we could have also just called customPlot->rescaleAxes(); instead
        // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
        customPlot->setInteractions(QCP::iRangeDrag |
                                    QCP::iRangeZoom |
                                    QCP::iSelectPlottables);


        customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);

        customPlot->replot();
        customPlot->update();

}

void MainWindow::clearAllWidgets()
{
//    while ( QWidget* w = ui->pointsScrollAreaContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }

//    while ( QWidget* w = ui->vectorsScrollAreaContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }

//    while ( QWidget* w = ui->framesScrollAreaContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }

//    while ( QWidget* w = ui->anglesScrollAreaContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }

//    while ( QWidget* w = ui->planesScrollAreaContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }

//    while ( QWidget* w = ui->FOVscrollAreaWidgetContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }
//    while ( QWidget* w = ui->tracksScrollAreaContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }

//    while ( QWidget* w = ui->stylesScrollAreaContents->findChild<QWidget*>() )
//    {
//        delete w;
//    }



//    this->checkBoxes.erase(this->checkBoxes.begin(), this->checkBoxes.end());
//    this->checkBoxes.clear();

//    this->checkBoxesToObj.erase(this->checkBoxesToObj.begin(), this->checkBoxesToObj.end());
//    this->checkBoxesToObj.clear();

//    this->labels.erase(this->labels.begin(), this->labels.end());
//    this->labels.clear();

//    this->labelsToObj.erase(this->labelsToObj.begin(), this->labelsToObj.end());
//    this->labelsToObj.clear();

//    this->sizeSpinBox.erase(this->sizeSpinBox.begin(), this->sizeSpinBox.end());
//    this->sizeSpinBox.clear();

//    this->opacitySpinBox.erase(this->opacitySpinBox.begin(), this->opacitySpinBox.end());
//    this->opacitySpinBox.clear();

//    this->selectColorButtons.erase(this->selectColorButtons.begin(), this->selectColorButtons.end());
//    this->selectColorButtons.clear();


}

void MainWindow::buttonPressedSlot()
{
    QPushButton* obj = dynamic_cast<QPushButton*>(sender());

    for(auto id = this->selectColorButtons.begin(); id != this->selectColorButtons.end(); id++)
    {
        if(id->second == obj)
        {

            QColor newCol = QColorDialog::getColor();

            QString styleS = QString("QPushButton { border: 2px solid #8f8f91; \n"
                                                    "border-radius: 10px; \n"
                                                    "padding: 5px; \n"
                                                    "background-color:" + newCol.name() + ";})");


            id->second->setStyleSheet(styleS);

            QString key = id->first;
            key = key.remove(0, key.indexOf('/')+1);
            this->plots[key]->setPen(newCol);
            this->plots[key]->parentPlot()->replot();
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

