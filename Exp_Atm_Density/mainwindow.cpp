#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаем объект температурного профиля
    mTempProfile = new TemperatureProfile();

    // Создаем модель атмосферы с примерными параметрами:
    // базовая плотность 1.225 кг/м³, базовое давление 101325 Па, g = 9.81 м/с².
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
    // Читаем введенную высоту (в метрах) из QTextEdit
    double height = ui->textEdit->toPlainText().toDouble();

    // Вычисляем плотность (time не используется) и температуру
    double density = mAtmModel->getDensity(height, 0.0);
    double temperature = mTempProfile->getTemperature(height);

    // Формируем строку с результатами
    QString output = QString("Height = %1 meters\nDensity = %2 kg/m³\nTemperature = %3 K")
                     .arg(height)
                     .arg(density, 0, 'e', 6)
                     .arg(temperature, 0, 'f', 2);

    // Выводим строку в QTextBrowser
    ui->textBrowser->setText(output);
}
