@mainpage Programmdokumentation

Die Dokumentation erfolgt getrennt nach den jeweiligen Aufgaben

@page em_aufgabe1 Aufgabe I2C Lichtsensor

Es sollen weitere I2C Busteilnehmer an den I2C Bus angeschlossen und angesteuert werden. F?r diese Aufgabe zumindest ein I2C Lichtsensor vom Typ BH1750 (Modul GY-302). Sie k?nnen gerne auch weitere eigene oder von mir gestellte Sensoren verwenden.

Da der grundliegende I2C Kommunikationsablauf immer ?hnlich ist, soll eine universelle FSM entwickelt werden, welche im Interrupt aber auch wahlweise in der Super Loop verwendet werden kann.

Diese wird mittels der Funktion exchangeI2C()  getriggert, welche alls Schnittstelle zwischen Anwendungsprogramm und FSM fungiert.

Das Auslesen der Sensor Daten soll mit frei variierbaren Zeitintervallen erfolgen, im Bereich von 1 Sekunde bis 3600 Sekunden. (Makrodefine)
Nach dem erfolgreichen Lesen der Sensordaten sollen diese ?ber die UART ausgegeben werden

Die grundlegende Funktion ist wie folgt:

Im Interrupt des Timer1 _T1Interrupt() wird zyklisch eine Anfrage mit der Funktion exchangeI2C() get?tigt.
In der Superloop befindet sich die Funktion doI2C(), welche die FSM enth?lt.
Au?erdem befindet sich dort die Funktion print_sensor_values(), welche die Sensor-Wert nach erfolgreicher Abarbeitung der Anfrage ausgibt.


Die Funktionen in der I2C funktionieren wie folgt:

In der Funktion put_I2C_struct_FIFO() wird eine I2C-Anfrage im I2C-FIFO abgelegt.
Die Funktion get_I2C_struct_FIFO() entnimmt die I2C-Anfrage aus dem I2C-FIFO.
Mit der initI2C() wird die I2C-Kommunikation initialisiert.

Anschlie?end kommen die Funktionen der FSM:

Zu Beginn die Funktion FSM_Idle(). Diese kopiert die Anfrage aus dem FIFO und leitet die Start-Sequenz ein. Wenn es eine Anfrage gibt, werden die Startbedingungen weitergeleitet.
Mit der Funktion FSM_Start() wird das Trancieve-Register mit der Adresse beschrieben. Entweder wird geschrieben oder gelesen.
Wenn geschrieben wird, wird die Funktion FSM_Adresse_Write() aufgerufen und die zu ?bertragenden Daten werden in das Tranceive-Register geschrieben.
Wenn es nichts mehr zu senden gibt, dann kommt es zum Restart und die Funktion FSM_Repeated_Start() wird aufgerufen. Hier wird wieder das Tranceive-Register mit der Adresse beschrieben. 
Wenn der Restart erfolgreich war, wird geschaut ob noch gelesen werden kann.
Hierbei wird die Funktion FSM_Adresse_Read() aufgerufen. Hier wird das Lesen der Daten des Slaves initiiert. 
Gibt es noch etwas zum Lesen wird die Funktion FSM_RECV_EN() aufgerufen. Diese liest das Receive Register aus und best?tigt dies mit einem ACK oder NACK.
Treten Fehlerf?lle beim Lesen oder Schreiben auf, wird eine Stopp-Bedingung eingeleitet. Dies hat zur Folge das die Funktion FSM_Stop() aufgerufen wird. Diese verursacht die R?ckkehr in den Idle-State.

Der Status der Anfragen wird innerhalb der FSM gesetzt. Falls der Status auf Finished gesetzt wird, gibt print_sensor_values() die gemessenen Werte aus.


@page em_aufgabe3 Erweiterung I2C Lichtsensor Callback

In dieser Aufgabe wurde die I2C-Funktionalit?t um die M?glichkeit erweitert, eine Callback Funktion zu definieren. Diese Funktion wird aufgerufen, sobald die FSM die ?bertragung stoppt. 
Das kann sowohl im Normal-Fall als auch im Fehlerfall erfolgen.
Die zwei Callback-Funktionen I2C_TempSens_Callback() und I2C_LightSens_Callback() geben dann jweils die Temperatur oder den Lichtwert aus, falls der Status auf Finished gesetzt wurde.
Die Funktionen m?ssen dabei vom Typ I2C_Callback_t sein, welcher die Weitergabe der ben?tigten Zegier und Daten erlaubt.


@page em_aufgabe2 Aufgabe LCD Display GPIO

In dieser Aufgabe sollte das LCD Display in Betrieb genommen werden. Es wurde daf?r die Bibliothek lcd_gpio.h geschrieben.
Der Zugriff erfolgt dabei noch ?ber die GPIOs.
Um das LCD anzusteuern muss dieses vorher einmalig initialisiert werden. Dies geschieht in der Funktion lcd_init().

Ist das LCD initialisiert, k?nnen ?ber die Funktion lcd_write_data() Daten ?ber den Bus versendet werden.
Mit der Funktion lcd_get_status() wird das Busy-Flag und der Adress-Z?hler abgefragt.
Die Funktion waitForBusyLCD() liest das Busy-Flag und blockiert solange dies gesetzt ist.
In den anderen Funktionen, welche einen Schreib-Zugriff implementieren wird immer zuerst die Funktion waitForBusyLCD() ausgef?hrt um zu gew?hrleisten, dass die zu sendenden Daten auch vom LCD interpretiert werden k?nnen.

Es wurde eine Funktion writeStrLCD() geschrieben, welche es erlaubt komplette Strings auf dem LCD anzuzeigen.

Weiter Funktionen sind lcd_clear(), welche das LCD zur?cksetzt und den Inhalt l?scht sowie lcd_set_pos(), welche den Cursor auf die gew?nschte Position bewegt.

Um die geforderten Timings einzuhalten wurden zwei unterschiedliche Delay-Mechanismen verwendet:
__delay_cycles() verz?gert um die angegebenen Zyklen. In unserem Fall betr?gt FCY 50MHZ also ist ein Taktzyklus 20ns lang.
__delay_us() verz?gert um die angegebenen Mikrosekunden.

In der SuperLoop wird die Funktion display_temp_load() jede ms aufgerufen und gibt in einem Takt von 3s abwechselnd die akutelle Temperatur und Aulastung in der ersten Zeile des LCDs aus.
Des Weiteren wird alle 3s die per UART empfangenen Daten in der zweiten Zeile des LCDs ausgegeben.

@image html LCD_Timing.png

Hier zu sehen ist das Timing der LCD Signale. Als erstes wird hier die Funktion lcd_set_pos() ausgef?hrt. In dieser wird das Display auf die Home position gesetzt.
Danach wird zyklisch das Busy-Flag abgefragt. Es dauert ca 0.4ms bis es nicht mehr gesetzt ist. Danach erfolgen Schreib-Zugriffe, welche jeweils ca. 30us andauern.



@page em_aufgabe4 Aufgabe LCD Display PMP

In dieser Aufgabe sollte das LCD mit Hilfe des PMP-Moduls angesteuert werden. Es wurde hierf?r die Bibliothek PMP.h geschrieben. Der Parallel Master Port (PMP) ist ein paralleles Kommunikationsmodul, das speziell f?r
die Kommunikation mit einer Vielzahl von parallelen Ger?ten, wie zum Beispiel in unserem Fall ein LCD, verwendbar ist. Um die Ansteuerung des LCDs zu erm?glichen, wird vorab eine Initialisierung vorgenommen. Hierf?r 
wurde die Funktion initPMP() implementiert.
Anschlie?end wurde darauf geachtet, dass dieselben Funktionen verwendet wurden wie schon bei der Ansteuerung mittels GPIO. Diese wurden gr??tenteils angepasst, damit die Ansteuerung ?ber PMP funktioniert. Dadurch das wir dieselben
Funktionen verwendet haben, kann man jederzeit problemlos in der Hauptschleife zwischen der Ansteuerung ?ber PMP oder GPIO wechseln.
  
  
@page em_aufgabe4 Aufgabe EEPROM SPI
In dieser Aufgabe wurde ein EEPROM-Speicher mit Hilfe der SPI Schnittstelle angesteuert.
Daf?r wird die Schnittstelle in der Funktion initSPI() initialisiert.
Mit readSignatureEEPROM() wird die Signatur ausgelesen. Liefert diese Funktion 0x29 zur?ck, so war die Kommunikation erfolgreich.
Mit readStatusEEPROM() wird der akutelle Status ausgelesen. Dieser liefert unter Anderem Informationen ?ber das Write-Enable Latch und das Write in Progress Bit.
In der Funktion busyEEPROM() wird der Status auf das Write-In-Progress Bit hin untersucht. Diese Funktion kann dann zyklisch in einer While-Schleife aufgerufen werden um solange zu warten, bis geschrieben werden kann.
Mit Hilfe von setWriteEnableEEPROM() wird das Write-Enable Latch gesetzt.
Schreib- und Lese-Zugriff erfolgt ?ber writeDataEEPROM() und readDataEEPROM().

  

 

