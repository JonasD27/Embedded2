cd ..\doxygen

@echo Make PDF File

call latex\make.bat

@echo Open PDF Dokumentation
start latex\refman.pdf 

