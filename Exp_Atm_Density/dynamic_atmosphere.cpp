#include "dynamic_atmosphere.h"
#include "atmosphereconst.h"
#include "msise90_sub.h"


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
    double sod = utc.time_h * 3600 + utc.time_m * 60 + utc.time_s;
    float xsod = static_cast<float>(sod);

    // В модели MSIS высота должна быть в километрах.
    float xalt = static_cast<float>(geo.attitude);
    // Используем геодезические координаты, полученные из CalculateGeodetics.
    float xlat = static_cast<float>(geo.latitude);
    float xlon = static_cast<float>(geo.longitude);
    // Вычисляем локальное солнечное время (LST) – примитивно: LST = (sod/3600 + долгота/15) (в часах)
    float xlst = fmod(xsod / 3600.0f + xlon / 15.0f, 24.0f);

    // Создаем комбинированное значение "год и день" (формат YYDDD или год*1000+день)
    int xyd = utc.year * 1000 + utc.day;

    // Подготавливаем параметры для вызова gtd6_ (функция из MSIS90)
    float xf107  = static_cast<float>(mF107);
    float xf107a = static_cast<float>(mF107A);
    // Здесь, как типичный пациент, мы задаем номинальные значения геомагнитных индексов (например, 15)
    float xap[7] = {15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f, 15.0f};
    int xmass = 48;  // масса = 48 для "всей" атмосферы

    // Массивы для результатов от gtd6_
    float xden[8] = {0.0f};
    float xtemp[2] = {0.0f};

// Вызываем MSIS90-функцию gtd6_
#ifdef USE_64_BIT_LONGS
    long int xydLong = static_cast<long int>(xyd);
    long int xmassLong = static_cast<long int>(xmass);
    gtd6_(&xydLong, &xsod, &xalt, &xlat, &xlon, &xlst, &xf107a, &xf107,
          &xap[0], &xmassLong, &xden[0], &xtemp[0]);
#else
    gtd6_(&xyd, &xsod, &xalt, &xlat, &xlon, &xlst, &xf107a, &xf107,
          &xap[0], &xmass, &xden[0], &xtemp[0]);
#endif

    // Согласно стандарту MSIS90 общая масса плотности хранится в xden[5].
    // Обычно эта величина дана в г/см³, поэтому для перевода в кг/м³ умножаем на 1000.
    long double density = xden[5] * 1000.0;
    return density;

}


// Функция, рассчитывающая геодезические координаты по заданной позиции (в км)
// Используются параметры WGS84 для Земли.
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
    double z_ecef = z_j2000;  // Ось Z не меняется


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
    double newLat = lat_rad; // Объявляем newLat заранее

    while (iteration < maxIterations && fabs(newLat - lat_rad) > delta)
    {
        lat_rad = newLat;
        N = a / sqrt(1 - ecc2 * sin(lat_rad) * sin(lat_rad));
        newLat = atan2(z_ecef + ecc2 * N * sin(lat_rad), rxy);
        ++iteration;
    }
    coordinates cord_on_Earth;
    // Вычисляем высоту над эллипсоидом
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

    // Применяем gmtime, так как мы уже сместили время вручную
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

    // Если месяц январь или февраль, считаем их как 13 и 14 месяцы предыдущего года.
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
