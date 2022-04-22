@mainpage Programmdokumentation

Die Dokumentation erfolgt getrennt nach den jeweiligen Aufgaben

@subsection em_aufgabe1 Aufgabe I2C Lichtsensor

Es sollen weitere I2C Busteilnehmer an den I2C Bus angeschlossen und angesteuert werden. Für diese Aufgabe zumindest ein I2C Lichtsensor vom Typ BH1750 (Modul GY-302). Sie können gerne auch weitere eigene oder von mir gestellte Sensoren verwenden.

Da der grundliegende I2C Kommunikationsablauf immer ähnlich ist, soll eine universelle FSM entwickelt werden, welche im Interrupt aber auch wahlweise in der Super Loop verwendet werden kann.

Diese wird mittels der Funktion exchangeI2C()  getriggert, welche alls Schnittstelle zwischen Anwendungsprogramm und FSM fungiert.

Das Auslesen der Sensor Daten soll mit frei variierbaren Zeitintervallen erfolgen, im Bereich von 1 Sekunde bis 3600 Sekunden. (Makrodefine)
Nach dem erfolgreichen Lesen der Sensordaten sollen diese über die UART ausgegeben werden

Die grundlegende Funktion ist wie folgt:

Im Interrupt des Timer1 _T1Interrupt() wird zyklisch eine Anfrage mit der Funktion exchangeI2C() getätigt.
In der Superloop befindet sich die Funktion doI2C(), welche die FSM enthält.
Außerdem befindet sich dort die Funktion print_sensor_values(), welche die Sensor-Wert nach erfolgreicher Abarbeitung der Anfrage ausgibt.

