@mainpage Programmdokumentation

Die Dokumentation erfolgt gtrennt nach den jeweiligen Aufgaben

@subsection em_aufgabe1 Aufgabe I2C Lichtsensor

Es sollen weitere I2C Busteilnehmer an den I2C Bus angeschlossen und angesteuert werden. Für diese Aufgabe zumindest ein I2C Lichtsensor vom Typ BH1750 (Modul GY-302). Sie können gerne auch weitere eigene oder von mir gestellte Sensoren verwenden.

Da der grundliegende I2C Kommunikationsablauf immer ähnlich ist, soll eine universelle FSM entwickelt werden, welche im Interrupt aber auch wahlweise in der Super Loop verwendet werden kann.

Diese wird mittels der Funktion exchangeI2C()  getriggert, welche alls Schnittstelle zwischen Anwendungsprogramm und FSM fungiert.

Das Auslesen der Sensor Daten soll mit frei variierbaren Zeitintervallen erfolgen, im Bereich von 1 Sekunde bis 3600 Sekunden. (Makrodefine)
Nach dem erfolgreichen Lesen der Sensordaten sollen diese über die UART ausgegeben werden
Optional: Erweitern Sie den Kommandointerpreter von Embedded 1 um die Zeitintervalle über die UART verändern zu können.

