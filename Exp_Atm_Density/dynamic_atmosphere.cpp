#include "dynamic_atmosphere.h"
#include "atmosphereconst.h"
#include "msise90_sub.h"
#include <fstream>
#include <iostream>

#include <err.h>

DynamicAtmosphere::DynamicAtmosphere(double baseDensity, double basePressure, double gravity,
                                     TemperatureProfile* tempProfile,
                                     double F107, double F107A)
    : Atmosphere(baseDensity, basePressure, gravity),
    mTempProfile(tempProfile),
    mF107(F107),
    mF107A(F107A),
    mAltitudeBands(28),
    mRefHeight(nullptr),
    mRefDensity(nullptr),
    mScaleHeight(nullptr)
{
    AtmosphereConstants::SetConstants(mAltitudeBands, mRefHeight, mRefDensity, mScaleHeight);
}

DynamicAtmosphere::~DynamicAtmosphere()
{
    delete[] mScaleHeight;
    delete[] mRefHeight;
    delete[] mRefDensity;
}

long double DynamicAtmosphere::getDensity(double altitude, double time) const
{

    if (altitude < 0.0)
        throw std::runtime_error("Altitude below zero in getDensity(alt,time)");

    double alt_km = altitude / 1000.0;
    int band = FindBand(alt_km);
    double baseDensityValue = mRefDensity[band] *
                              std::exp( -(alt_km - mRefHeight[band]) / mScaleHeight[band] );


    return baseDensityValue; // или любой другой return
}

int DynamicAtmosphere::FindBand(double height) const
{
    int index = mAltitudeBands - 1;
    for (int i = 0; i < mAltitudeBands - 1; ++i)
    {
        if (height < mRefHeight[i+1])
        {
            index = i;
            break;
        }
    }
    return index;
}

long double DynamicAtmosphere::getDensity(const double* j2000, const UTC_time utc) const
{
    coordinates geo = CalculateGeodetics(j2000, utc);

    long double JD = utcToJulianDate(utc);

    double GMST_deg = fmod(280.46061837 + 360.98564736629 * (JD - 2451545.0), 360.0);
    if (GMST_deg < 0)
        GMST_deg += 360.0;
    float GMST_rad = static_cast<float>(GMST_deg * M_PI / 180.0);

    // Вычисляем количество секунд с начала суток
    sod = static_cast<float>(utc.time_h * 3600 + utc.time_m * 60 + utc.time_s);
    float xsod = static_cast<float>(sod);

    // В модели MSIS высота должна быть в километрах.
    float xalt = static_cast<float>(geo.attitude);
    // Используем геодезические координаты, полученные из CalculateGeodetics.
    float xlat = static_cast<float>(geo.latitude);
    float xlon = static_cast<float>(geo.longitude);
    // Вычисляем локальное солнечное время (LST) – примитивно: LST = (sod/3600 + долгота/15) (в часах)
    float xlst = fmod(xsod / 3600.0f + xlon / 15.0f, 24.0f);

    // Создаем комбинированное значение "год и день" (формат YYDDD или год*1000+день)
    std::tm tm_local = {};
    tm_local.tm_year = utc.year - 1900;
    tm_local.tm_mon  = utc.month - 1;
    tm_local.tm_mday = utc.day;
    tm_local.tm_hour = static_cast<int>(utc.time_h);
    tm_local.tm_min  = static_cast<int>(utc.time_m);
    tm_local.tm_sec  = static_cast<int>(utc.time_s);
    tm_local.tm_isdst = 0;
    std::time_t local_time = mktime(&tm_local);
    std::tm* local_tm = localtime(&local_time);
    int day_of_year = local_tm->tm_yday + 1;  // Преобразуем в 1-based

    // Формируем комбинированное значение "год и день" в формате YYDDD.
    int xyd = utc.year * 1000 + day_of_year;

    // Подготавливаем параметры для вызова gtd6_ (функция из MSIS90)
    float xf107  = static_cast<float>(mF107);
    float xf107a = static_cast<float>(mF107A);
    // Здесь, как типичный пациент, мы задаем номинальные значения геомагнитных индексов (например, 15)
    float xap[7] = {15.0f, 15.2f, 15.7f, 15.5f, 15.3f, 15.0f, 15.1f};
    integer xmass = 48;  // масса = 48 для "всей" атмосферы

    // Массивы для результатов от gtd6_
    float xden[8];
    for (int j = 0; j < 8; j++)
    {
        xden[j] = (float)0;
    }
    float xtemp[2];
    for(int j = 0; j<2; ++j){
        xtemp[j] = (float) 0;
    }

    extern float sod;    // количество секунд от начала суток
    extern int yd;       // год и день в формате YYDDD
    extern float f107;   // F10.7
    extern float f107a;  // F10.7A
    extern float ap[7];  // массив геомагнитных индексов

    // Устанавливаем глобальные переменные в соответствии с нашими вычислениями
    sod = xsod;
    yd = xyd;
    f107 = xf107;
    f107a = xf107a;
    for (int i = 0; i < 7; i++)
    {
        ap[i] = xap[i];
    }
    std::ofstream logFile ("../../../../logs_of_gtd6_.log");
    logFile << "Before-GTDS6()\n";
    logFile << "===========\n";
    logFile << "Epoch                  = " << JD << "\n";
    logFile << "Year & Days            = " << xyd << "\n";
    logFile << "Seconds                = " << xsod << "\n";
    logFile << "Altitude               = " << xalt << "\n";
    logFile << "Latitude               = " << xlat << "\n";
    logFile << "Longitude              = " << xlon << "\n";
    logFile << "Solar Time             = " << xlst << "\n";
    logFile << "F107 Average           = " << xf107a << "\n";
    logFile << "F107                   = " << xf107 << "\n";
    for (int i = 0; i < 7; i++) {
        logFile << "Geomagnetic index[" << i << "]   = " << xap[i] << "\n";
    }
    logFile << "Mass                   = " << xmass << "\n";
    for (int i = 0; i < 8; i++) {
        logFile << "Density[" << i << "]      = " << xden[i] << "\n";
    }
    logFile << "EXOSPHERIC Temperature = " << xtemp[0] << "\n";
    logFile << "Temperature at Alt     = " << xtemp[1] << "\n\n";

    int res = gtd6_(&xyd,&xsod,&xalt,&xlat,&xlon,&xlst,&xf107a,&xf107,xap,&xmass,
          xden,xtemp);




    //FILE* logFile = fopen("logfile.log"<< r+);
    //std::ofstream logFile ("/Users/zuha/Desktop/FKI/3 курс 2023-2025/6 семестр 2025/Матмод/Lololologer.log");
    if (!logFile.is_open()) {
        std::cerr << "Ошибка открытия файла logs.log!" << std::endl;
    }

    logFile << "Post-GTDS6()\n";
    logFile << "===========\n";
    logFile << "Epoch                  = " << JD << "\n";
    logFile << "Year & Days            = " << xyd << "\n";
    logFile << "Seconds                = " << xsod << "\n";
    logFile << "Altitude               = " << xalt << "\n";
    logFile << "Latitude               = " << xlat << "\n";
    logFile << "Longitude              = " << xlon << "\n";
    logFile << "Solar Time             = " << xlst << "\n";
    logFile << "F107 Average           = " << xf107a << "\n";
    logFile << "F107                   = " << xf107 << "\n";
    for (int i = 0; i < 7; i++) {
        logFile << "Geomagnetic index[" << i << "]   = " << xap[i] << "\n";
    }
    logFile << "Mass                   = " << xmass << "\n";
    for (int i = 0; i < 8; i++) {
        logFile << "Density[" << i << "]      = " << xden[i] << "\n";
    }
    logFile << "EXOSPHERIC Temperature = " << xtemp[0] << "\n";
    logFile << "result of gtd6_ func_work = " << res << "\n";
    logFile << "Temperature at Alt     = " << xtemp[1] << "\n\n";
    logFile.flush();


    // Согласно стандарту MSIS90 общая масса плотности хранится в xden[5].
    long double density = xden[5] * 1000.0;
    return density;

}


// Функция, рассчитывающая геодезические координаты по заданной позиции (в км)
coordinates DynamicAtmosphere::CalculateGeodetics(const double* j2000, const UTC_time time) const
{

    // Преобразуем время: вычисляем GMST в градусах по формуле:
    // GMST = 280.46061837 + 360.98564736629 * (JD - 2451545.0)  (mod 360)
    long double epoch = utcToJulianDate(time);
    double GMST_deg = fmod(280.46061837 + 360.98564736629 * (epoch - 2451545.0), 360.0);
    if (GMST_deg < 0)
        GMST_deg += 360.0;
    double GMST_rad = GMST_deg * M_PI / 180.0;

    // Преобразуем координаты из J2000 (ECI) в ECEF через поворот вокруг оси Z:
    // ECEF = Rz(GMST_rad) * J2000
    double x_j2000 = j2000[0];
    double y_j2000 = j2000[1];
    double z_j2000 = j2000[2];

    double x_ecef = cos(GMST_rad) * x_j2000 + sin(GMST_rad) * y_j2000;
    double y_ecef = -sin(GMST_rad) * x_j2000 + cos(GMST_rad) * y_j2000;
    double z_ecef = z_j2000;


    const double a = 6378.137;                    // экваториальный радиус в км
    const double f = 1.0 / 298.257223563;           // сжатие
    const double ecc2 = 2 * f - f * f;              // квадрат эксцентриситета


    // Расчет расстояния проекции на экваториальную плоскость
    double rxy = sqrt(x_ecef * x_ecef + y_ecef * y_ecef);
    double N; // радиус кривизны в направлении нормали

    double lat_rad = atan2(z_ecef, rxy);
    const double delta = 1e-10;
    const int maxIterations = 100;
    int iteration = 0;
    double newLat = lat_rad;

    while (iteration < maxIterations && fabs(newLat - lat_rad) > delta)
    {
        lat_rad = newLat;
        N = a / sqrt(1 - ecc2 * sin(lat_rad) * sin(lat_rad));
        newLat = atan2(z_ecef + ecc2 * N * sin(lat_rad), rxy);
        ++iteration;
    }
    coordinates cord_on_Earth;
    // Вычисляем высоту над эллипсоидом
    N = a / sqrt(1 - ecc2 * sin(lat_rad) * sin(lat_rad));
    cord_on_Earth.attitude = rxy / cos(lat_rad) - N;

    // Переводим широту и долготу в градусы
    cord_on_Earth.latitude = lat_rad * (180.0 / M_PI);
    cord_on_Earth.longitude = atan2(y_ecef, x_ecef) * (180.0 / M_PI);

    return cord_on_Earth;
}

UTC_time convertUtcToLocalSolarTime(const std::time_t utcTime, double longitude)
{
    double offsetHours = longitude / 15.0;
    std::time_t localTimeT = utcTime + static_cast<std::time_t>(offsetHours * 3600);

    // Применяем gmtim<< так как мы уже сместили время вручную
    std::tm* tmLocal = std::gmtime(&localTimeT);
    UTC_time lt;
    lt.year   = tmLocal->tm_year + 1900;  // tm_year — годы с 1900
    lt.month  = tmLocal->tm_mon + 1;       // tm_mon от 0 до 11
    lt.day    = tmLocal->tm_mday;
    lt.time_h   = tmLocal->tm_hour;
    lt.time_m = tmLocal->tm_min;
    lt.time_s = tmLocal->tm_sec;
    return lt;
}


long double utcToJulianDate(const UTC_time& utc) {
    int Y = utc.year;
    int M = utc.month;
    int D = utc.day;
    if (M <= 2) {
        Y -= 1;
        M += 12;
    }

    int A = Y / 100;
    int B = 2 - A + (A / 4);

    // Вычисляем дробную часть дня
    double dayFraction = (utc.time_h + utc.time_m / 60.0 + utc.time_s / 3600.0) / 24.0;

    // Формула для Юлианской даты:
    double JD = std::floor(365.25 * (Y + 4716)) +
                std::floor(30.6001 * (M + 1)) +
                D + dayFraction + B - 1524.5;

    return JD;
}
