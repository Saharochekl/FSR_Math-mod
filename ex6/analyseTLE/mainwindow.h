#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../tleLib/orbit/cSatellite.h"
#include "../tleLib/tleparser.h"

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void clearAllWidgets();
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::map<QString, QLabel*> labels;
    std::map<QString, QPushButton*> selectColorButtons;

    QStringList _satKeys;
    std::map<QString, QString> _satFiles;

    std::map<QString, QCPGraph*> plots;
private slots:

    void buttonPressedSlot();


};
#endif // MAINWINDOW_H
