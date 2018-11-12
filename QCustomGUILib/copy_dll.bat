@echo copy customguilib header files
copy .\src\pixmapBar\*.h .\3PartLib\include\
copy .\src\QGraphViewUI\*.h .\3PartLib\include\
copy .\src\QLightBoxUI\*.h .\3PartLib\include\
copy .\src\splashScreen\*.h .\3PartLib\include\

@echo copy customguilib library
copy .\win32\Debug\*.lib .\3PartLib\lib\
copy .\win32\Release\*.lib .\3PartLib\lib\
