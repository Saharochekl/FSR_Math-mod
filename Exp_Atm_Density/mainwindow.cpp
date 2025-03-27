#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mTempProfile = new TemperatureProfile();

    // Создаем модель атмосферы с примерными параметрами:
    mAtmModel = new ExponentialAtmosphere(1.225, 101325, 9.81, mTempProfile);
}

MainWindow::~MainWindow()
{
    delete mAtmModel;
    delete mTempProfile;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    double height = ui->textEdit->toPlainText().toDouble();

    double density = mAtmModel->getDensity(height, 0.0);
    double temperature = mTempProfile->getTemperature(height);

    QString output = QString("Height = %1 meters\nDensity = %2 kg/m³\nTemperature = %3 K")
                     .arg(height)
                     .arg(density, 0, 'e', 6)
                     .arg(temperature, 0, 'f', 2);

    ui->textBrowser->setText(output);
}
