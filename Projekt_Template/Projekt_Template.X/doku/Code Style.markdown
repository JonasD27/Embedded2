@page em_code_style Code-Style


Hier wird der aktuelle Coding Style hinterlegt.

@subsection em_formatierung Einr?ckung, Klammern und Formatierung
Der Code sollte m?glichst einfach lesbar sein und gut strukturiert sein. 
Die geschwungenen Klammern von Bl?cken wie z.B. If-Bl?cken stehen immer in einer neuen, alleinstehenden Zeile.
Bl?cke werden einger?ckt.

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
Falls der Kommentar sich ?ber mehrere Zeilen erstreckt, wird dieser mit /*  */begrenzt. 
Andernfalls reichen die zwei doppelten Slashs //. Kommentare sind generell in Deutsch.

	int icounter_5; //Counter mit Startwert 5
	 
	int rgb_to_lumi(int r, int g, int b).... /*Funktion um RGB-Werte in   einen Luminanz-Wert zu konvertieren*/
 

@subsection em_bezeichner Bezeichner / Namen von Variablen, Funktionen & Konstanten
Die Namen von den verschiedenen Strukturen sollen schon beim Lesen einen Hinweis auf deren Funktion geben. 
Die Namen sind generell in Englisch verfasst und k?nnen auch Abk?rzungen enthalten.
Variablen- und Funktionen-Bezeichner werden durch Unterstriche getrennt und sind klein geschrieben. (snake_case)
Konstanten hingegen werden gro? geschrieben.

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
Nur die n?tigsten Tags verwenden.

	/**
	* @brief Verz?gerung (ms)
	* Verz?gerungsfunktion, blockierend
	* @param milliseconds Anzahl der zu verz?gernden Millisekunden
	* @return ...
	*/




