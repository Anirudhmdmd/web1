@echo off
echo Compiling BLN EL Node...

REM Set path to elcc (EL compiler)
set EL_HOME=C:\Nokia\Mediation\elcompiler
set PATH=%EL_HOME%\bin;%PATH%

REM Clean old output if any
if exist bln.el del bln.el

REM Compile the node
elcc -n bln -s app_bln.c

REM Check for success
if %ERRORLEVEL% NEQ 0 (
    echo EL compilation failed!
    exit /b 1
)

REM Copy result to Jenkins workspace root (optional)
copy bln.el %WORKSPACE%

echo Node build complete.
