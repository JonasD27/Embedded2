@mainpage Embedded 2 Aufgabendokumentation

@section em_doku Programmdokumentation

Die Dokumentation erfolgt gtrennt nach den jeweiligen Aufgaben

@subsection em_aufgabe1 Aufgabe 1

 * die Main Funktion - main()
 * Systemfrequenz
   * Die Oscillatoreinstellungen sind in ConfigureOscillator() programmiert
   * Das Makrodefine #SYS_FREQ wird dabei ausgewertet
 * Für die GPIOs wie Taster und LEDs sind in der Datei user.h diverse Zugriffs Makros definiert sowie eine Initialisierungsfunktion initApp()
 * Softwaredelay Funktion delay_ms()

