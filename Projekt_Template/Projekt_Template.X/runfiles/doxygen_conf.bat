cd ..\doxygen

@echo Starte Doxywizard mit der Doxyfile Configuration

rem @echo @todo Leider blockiert hier noch Doxywizard die Batch Beendigung 
rem @echo obwohl "start" verwendet wurde
rem start doxywizard.exe Doxyfile

@echo Bei Fehler: Dateisystem Verknuepfung der Endung .doxy mit doxywizard angelegt??
start Doxyfile.doxy
