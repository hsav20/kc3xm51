@echo off
if not exist build md build
if not exist release md release
cd build

copy ..\src\main.h > error.h
copy ..\src\const.h > error.h
copy ..\src\main.lin > error.h
copy ..\src\main.c > error.h
copy ..\src\key.c > error.h
copy ..\src\display.c > error.h
copy ..\src\Kc3xType.h > error.h
copy ..\src\KcmI2c.h > error.h
copy ..\src\KcmI2c.c > error.h
copy ..\src\audio.c > error.h
copy ..\src\function.c > error.h

copy ..\Tools\stdarg.h > error.h
copy ..\Tools\c51tiny.h > error.h
copy ..\Tools\c51tiny.c > error.h

..\Tools\C51 KcmI2c.c SMALL WL(1) > error.h
if errorlevel 1 goto ERR_COMP
..\Tools\C51 audio.c SMALL WL(1) > error.h
if errorlevel 1 goto ERR_COMP
..\Tools\C51 function.c SMALL WL(1) > error.h
if errorlevel 1 goto ERR_COMP
..\Tools\C51 main.c SMALL WL(1) > error.h
if errorlevel 1 goto ERR_COMP
..\Tools\C51 key.c SMALL WL(1) > error.h
if errorlevel 1 goto ERR_COMP
..\Tools\C51 display.c SMALL WL(1) > error.h
if errorlevel 1 goto ERR_COMP
..\Tools\C51 c51tiny.c SMALL WL(1) > error.h
if errorlevel 1 goto ERR_COMP

..\Tools\BL51 @main.lin > error.h
if errorlevel 2 goto ERR_COMP
..\Tools\oh51 kc3xm51.tmp HEXFILE (kc3xm51.hex) > error.h
if errorlevel 1 goto ERR_COMP

copy kc3xm51.hex ..\release > error.h

dir ..\Release\kc3xm51.hex
goto ALL_DONE

:ERR_COMP
type error.h
ECHO.
ECHO.
ECHO error !!!
ECHO.

:ALL_DONE
cd ..


