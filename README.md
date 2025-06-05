# FSR_Math-mod

> Qt 6 desktop‑приложение (C++17) для моделирования плотности атмосферы  
> на основе **MSISE‑90** + экспоненциальной модели. Тестировалось на macOS 14
(Apple Silicon и Intel), но при желании собирается и под Linux/Windows.

---

## Возможности

* **MSISE‑90** (gtd6) перекомпилирована из Fortran → C и подключается через
  `libf2c`.
* GUI на **Qt 6**: ввод высоты/эпохи, лог‑окно, экспорт плотности.
* Быстрое сравнение «динамической» плотности (солнечно‑геомагнитный фон)
  с простым экспоненциальным приближением.
* Скрипт **Mac‑install_libf2c.sh** автоматически собирает и ставит
  `libf2c.a` и `f2c.h`.

## Структура репозитория

FSR_Math-mod/ 
├── Exp_Atm_Density/ # Qt‑проект (.pro, .ui, .cpp/h) 
│ ├── msise90_sub.{c,h} # портированный gtd6 + вспом. процедуры 
│ ├── atmosphere.{h,cpp} 
│ └── ... 
├── Mac-install_libf2c.sh # bash‑установка libf2c 
└── ...

---

## Быстрый старт (macOS)

```bash
git clone https://github.com/Saharochekl/FSR_Math-mod.git
cd FSR_Math-mod

# 1. Qt 6
brew install qt@6            # или официальный installer

# 2. libf2c
chmod +x Mac-install_libf2c.sh
./Mac-install_libf2c.sh      # добавь sudo, если /usr/local не‑записываем

#Скрипт скачивает архив libf2c.zip, собирает его c makefile.u и кладёт libf2c.a / f2c.h в /usr/local (Intel) или /opt/homebrew (Apple Silicon)

#Сборка

cd Exp_Atm_Density
qmake Exp_Atm_Density.pro    # или открой проект в Qt Creator
make -j$(sysctl -n hw.ncpu)
open build/Exp_Atm_Density.app   # macOS‑Bundle
```

## Быстрый старт Windows 10/11


Установите Qt 6 Desktop (комплект MinGW 64‑bit или MSVC 2022 64‑bit).

Откройте PowerShell 7+ и выполните:

```bash
./Install-libf2c.ps1 -Prefix "C:\dev\thirdparty\f2c"
```
Скрипт скачает исходники, соберёт libf2c и скопирует файлы в <Prefix>\include, lib.

В Qt Creator откройте FSR_Math-mod.pro, выберите комплект MinGW 64‑bit или MSVC 64‑bit, нажмите Build.

Сборка из CLI:
```bash
REM MinGW
qmake -r FSR_Math-mod.pro
mingw32-make -j8

REM MSVC (Developer Prompt)
qmake -r -spec win32-msvc FSR_Math-mod.pro
nmake
```

Использование
Запусти Exp Atm Density.

Задай дату/высоту (или импортируй TLE — в планах).

Нажми Compute — в статус‑баре появится ρ, в логе — полный вектор плотностей + температуры. Лог сохраняется в Lololologer.log.

Для пост‑обработки (GMAT, MATLAB и т.д.) используй Density[5] (массовая плотность, кг·м⁻³ после умножения на 1000).

TIP: F10.7 / Ap сейчас забиты константами; подставь реальные данные ― точность ощутимо вырастет.

Roadmap
 Автоматическая подкачка космической погоды (NOAA API)

 Графики ρ(h) и T(h) прямо в GUI

 CI‑сборки (GitHub Actions) под Linux

PR‑ы и issue‑репорты приветствуются! Перед коммитом прогоняйте clang-format.