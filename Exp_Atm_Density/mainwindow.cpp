#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "atmosphere.h"
#include "dynamic_atmosphere.h"
#include "exponentialatmosphere.h"


#include <QString>
#include <QMessageBox>
#include <sstream>
#include <iomanip>
#include <ctime>

// Конструктор главного окна
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Инициализируем профиль температуры, который используется обеими моделями
    mTempProfile = new TemperatureProfile();

    // Создаём статическую модель атмосферы заранее – она всегда нужна, даже если динамика в моде.
    mStaticModel = new ExponentialAtmosphere(1.225, 101325, 9.81, mTempProfile);
}

// Деструктор главного окна
MainWindow::~MainWindow()
{
    delete mStaticModel;
    // Если динамическая модель создаётся на лету, её не нужно хранить как член класса.
    delete mTempProfile;
    delete ui;
}

// Обработчик нажатия кнопки "Calculate"
void MainWindow::on_CalcButton_clicked()
{
    // Если выбран режим статической модели:
    if (ui->Static_button->isChecked()) {
        // Получаем высоту из текстового поля Static_Alt (в метрах)
        double altitude = ui->Static_Alt->toPlainText().toDouble();
        // Вызываем статическую модель (ExponentialAtmosphere)
        double density = mStaticModel->getDensity(altitude, 0.0);
        double temperature = mTempProfile->getTemperature(altitude);

        QString output = QString("Static Model:\n"
                                 "Height = %1 meters\n"
                                 "Density = %2 kg/m³\n"
                                 "Temperature = %3 K")
                             .arg(altitude)
                             .arg(density, 0, 'e', 6)
                             .arg(temperature, 0, 'f', 2);
        ui->textBrowser->setText(output);
    }
    // Если выбран режим динамической модели:
    else if (ui->Dynamic_button->isChecked()) {
        // Получаем координаты в J2000 из полей Dyn_X, Dyn_Y, Dyn_Z
        double j2000[3];
        j2000[0] = ui->Dyn_X->toPlainText().toDouble();
        j2000[1] = ui->Dyn_Y->toPlainText().toDouble();
        j2000[2] = ui->Dyn_Z->toPlainText().toDouble();

        // Получаем время в формате строки из поля Dyn_time
        QString timeStr = ui->Dyn_time->toPlainText();
        std::tm tm = {};
        std::istringstream ss(timeStr.toStdString());
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (ss.fail()) {
            QMessageBox::critical(this, "Error", "Time format error. Expected format: YYYY-MM-DD HH:MM:SS");
            return;
        }
        // Для получения времени в UTC используем timegm (на MacOS он обычно есть)
        std::time_t utcTime = timegm(&tm);
        // Преобразуем в нашу структуру UTC_time
        UTC_time utc = convertUtcToLocalSolarTime(utcTime, 0.0);

        // Получаем значения F10.7 и F10.7A из соответствующих полей
        double mF107 = ui->mF107_data->toPlainText().toDouble();
        double mF107A = ui->mF107A_data->toPlainText().toDouble();

        // Создаём динамическую модель атмосферы "на лету"
        DynamicAtmosphere dynamicModel(1.225, 101325, 9.81, mTempProfile, mF107, mF107A);

        // Вычисляем плотность, передавая координаты в системе J2000 и время UTC
        long double density = dynamicModel.getDensity(j2000, utc);

        // Получаем геодезические координаты (преобразование из J2000 в геодезические происходит внутри функции)
        coordinates geo = dynamicModel.CalculateGeodetics(j2000, utc);

        // Формируем строку вывода с результатами
        QString output = QString("Dynamic Model:\n"
                                 "Density = %1 kg/m³\n\n"
                                 "Geodetic Coordinates:\n"
                                 "Latitude  = %2°\n"
                                 "Longitude = %3°\n"
                                 "Altitude  = %4 km\n\n"
                                 "UTC Time: %5-%6-%7 %8:%9:%10")
                             .arg(density, 0, 'e', 6)
                             .arg(geo.latitude, 0, 'f', 2)
                             .arg(geo.longitude, 0, 'f', 2)
                             .arg(geo.attitude, 0, 'f', 2)
                             .arg(utc.year)
                             .arg(utc.month)
                             .arg(utc.day)
                             .arg(static_cast<int>(utc.time_h), 2, 10, QChar('0'))
                             .arg(static_cast<int>(utc.time_m), 2, 10, QChar('0'))
                             .arg(static_cast<int>(utc.time_s), 2, 10, QChar('0'));
        ui->textBrowser->setText(output);
    }
    else {
        QMessageBox::warning(this, "Warning", "Please select a model: Static or Dynamic.");
    }
}
