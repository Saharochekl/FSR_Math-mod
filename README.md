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

Использование
Запусти Exp Atm Density.

Задай дату/высоту (или импортируй TLE — в планах).

Нажми Compute — в статус‑баре появится ρ, в логе — полный вектор плотностей + температуры. Лог сохраняется в Lololologer.log.

Для пост‑обработки (GMAT, MATLAB и т.д.) используй Density[5] (массовая плотность, кг·м⁻³ после умножения на 1000).

TIP: F10.7 / Ap сейчас забиты константами; подставь реальные данные ― точность ощутимо вырастет.

Roadmap
 Автоматическая подкачка космической погоды (NOAA API)

 Графики ρ(h) и T(h) прямо в GUI

 CI‑сборки (GitHub Actions) под Windows и Linux

PR‑ы и issue‑репорты приветствуются! Перед коммитом прогоняйте clang-format.