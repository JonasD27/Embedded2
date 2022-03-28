@mainpage Embedded1 und Embedded2 Aufgabendokumentation

@ref p_formatierung 

@section Programmdokumentation

Die Dokumentation erfolgt gtrennt nach den jeweiligen Aufgaben

@subsection em_aufgabe1 Aufgabe 1

 * die Main Funktion - main()
 * Systemfrequenz
   * Die Oscillatoreinstellungen sind in ConfigureOscillator() programmiert
   * Das Makrodefine #SYS_FREQ wird dabei ausgewertet
 * Für die GPIOs wie Taster und LEDs sind in der Datei user.h diverse Zugriffs Makros definiert sowie eine Initialisierungsfunktion initApp()
 * Softwaredelay Funktion delay_ms()

@subsection em_aufgabe23 Aufgabe 2 und 3

 * Delayfunktion mit dem Timer 1 delay_ms_t1() und zugehörige Initialisierung init_ms_t1()
 * Die LED und Taster Tasks: TaskLED0() und TaskTaster0()

@subsection em_aufgaben Restliche Aufgaben

 * Für die Prozessorauslastung: Auslastung() und init_T23_gated(void) 
 * Betriebszeit: initBetriebszeit() setBetriebszeit() getBetriebszeit() _T1Interrupt() 
 * UART putcUART() putsUART() getcUART()
   * FIFO: getcTX_FIFO() putcTX_FIFO() _U1TXInterrupt() _U1RXInterrupt
 * I2C und Temperatursensor: readTemperatureSimpleBlocking() readTemperatureFSM()
 * State Machines: diverse Umsetzungen in aFSM.h
 * Funktionen für das LC Display in LCD.h

 * Dokumentation @ref doku_page_timer_h in uTimer.h

