@echo off
if not exist build md build
if not exist release md release
cd build

copy ..\src\main.lin > error.h
copy ..\Tools\stdarg.h > error.h
copy ..\Tools\c51tiny.h > error.h
copy ..\Tools\c51tiny.c > error.h

for %%a in (..\src\*.c) do (
	copy %%a > error.h
)
for %%a in (..\src\*.h) do (
	copy %%a > error.h
)

for %%a in (*.c) do (
	..\Tools\C51 %%a SMALL "WL(1)" > error.h 
)


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
del *.c
del *.h
del *.lst
cd ..


