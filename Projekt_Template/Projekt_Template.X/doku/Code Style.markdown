@mainpage Dokumentation

@section em_code_style Code Style

Hier wird der aktuelle Coding Style hinterlegt.

@subsection em_formatierung Einrückung, Klammern und Formatierung
Der Code sollte möglichst einfach lesbar sein und gut strukturiert sein. 
Die geschwungenen Klammern von Blöcken wie z.B. If-Blöcken stehen immer in einer neuen, alleinstehenden Zeile.
Blöcke werden eingerückt.

	if (x < foo (y, z))
	{
	  haha = bar[4] + 5;
	}
	else
	{
	  while (z)
	  {
		haha += foo (z, z);
		z--;
	  }
	return ++x + bar ();
	}

Auch bei einzeiligen If-Anweisungen werden geschwungene Klammern verwendet.


@subsection em_kommentare Kommentare
Generell sollte jede Variable, Funktion oder andere Logik deren Aufgabe oder Bedeutung nicht 
direkt erkennbar ist mit einem kurzen Kommentar beschrieben werden. 
Falls der Kommentar sich über mehrere Zeilen erstreckt, wird dieser mit „/* …. */“ begrenzt. 
Andernfalls reichen die zwei doppelten Slashs „//“. Kommentare sind generell in Deutsch.

	int icounter_5; //Counter mit Startwert 5
	 
	int rgb_to_lumi(int r, int g, int b).... /*Funktion um RGB-Werte in   einen Luminanz-Wert zu konvertieren*/
 

@subsection em_bezeichner Bezeichner / Namen von Variablen, Funktionen & Konstanten
Die Namen von den verschiedenen Strukturen sollen schon beim Lesen einen Hinweis auf deren Funktion geben. 
Die Namen sind generell in Englisch verfasst und können auch Abkürzungen enthalten.
Variablen- und Funktionen-Bezeichner werden durch Unterstriche getrennt und sind klein geschrieben. (snake_case)
Konstanten hingegen werden groß geschrieben.

	//Konstanten
	int DELAY_MS=4;
	 
	//Variablen und Funktionen
	int current_memory_left;
	void set_all_leds_high();


@subsection em_bib Bibliotheken
Bibliotheken werden generell immer am Anfang der Datei eingebunden.

	#include "test_3.h"

@subsection em_doxy Doxygen
Einleitung mit /** 
Nur die nötigsten Tags verwenden.

	/**
	* @brief Verzögerung (ms)
	* Verzögerungsfunktion, blockierend
	* @param milliseconds Anzahl der zu verzögernden Millisekunden
	* @return …
	*/




@section em_doku Programmdokumentation

Die Dokumentation erfolgt gtrennt nach den jeweiligen Aufgaben

@subsection em_aufgabe1 Aufgabe I2C Lichtsensor

Es sollen weitere I2C Busteilnehmer an den I2C Bus angeschlossen und angesteuert werden. F�r diese Aufgabe zumindest ein I2C Lichtsensor vom Typ BH1750 (Modul GY-302). Sie k�nnen gerne auch weitere eigene oder von mir gestellte Sensoren verwenden.

Da der grundliegende I2C Kommunikationsablauf immer �hnlich ist, soll eine universelle FSM entwickelt werden, welche im Interrupt aber auch wahlweise in der Super Loop verwendet werden kann.

Diese wird mittels der Funktion exchangeI2C()  getriggert, welche alls Schnittstelle zwischen Anwendungsprogramm und FSM fungiert.

Das Auslesen der Sensor Daten soll mit frei variierbaren Zeitintervallen erfolgen, im Bereich von 1 Sekunde bis 3600 Sekunden. (Makrodefine)
Nach dem erfolgreichen Lesen der Sensordaten sollen diese �ber die UART ausgegeben werden
Optional: Erweitern Sie den Kommandointerpreter von Embedded 1 um die Zeitintervalle �ber die UART ver�ndern zu k�nnen.