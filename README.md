# DashboardPaper

## Podział plików

### lib/GUI/GUI_Paint

Tworzenie canvasu, rysowanie punktów, lini, tekstu liczb i czasu oraz obroty i odbicia obrazu.
Brak interakcji z hardware. 

### lib/GUI/GUI_BMPfile

Wczytanie bitmapy z pliku (funkcje dla różnych skal kolorów) oraz wstawienie obrazu do canvasu przy wykorzystując GUI_Paint

### lib/Fonts

Tablice zawierające mapowanie znaków. Podawane jako argument funkcji z GUI_Paint

### lib/e-Paper

Komunikacja z wyświetlaczem: inicjalizacja, clear, wyświetlanie, sleep

### lib/Config/Debug.h

Wyświetlanie w terminalu wiadomości Debug("")

### lib/Config/DEV_Config

Konfiguracja SPI i GPIO w zależności od wykorzystania RPi lub Jetson Nano oraz wybranej biblioteki sprzętowej

### lib/Config/dev_hardware_SPI

Alternatywna biblioteka sprzętowa do standardowych?