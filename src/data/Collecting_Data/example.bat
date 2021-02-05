@echo off
setlocal enableextensions
setlocal enabledelayedexpansion
%WINDIR%\System32\chcp.com 437 > NUL 2>&1

rem //-------------------------------------------------------------------------
rem // Configurations.
rem //-------------------------------------------------------------------------

rem //-------------------------------------------------------------------------
rem // Debug & execution settings.
rem //-------------------------------------------------------------------------
set DEBUG=YES
set EXECUTE=YES
set /A COMMANDS_COUNT=0
set TIME_OUT_IN_S=1

rem //-------------------------------------------------------------------------
rem // Folders settings.
rem //-------------------------------------------------------------------------
set ROOT_FOLDER=%cd%
set BIN_FOLDER=%ROOT_FOLDER%\_xlsdk\run\windows\Release\64
set OUTPUTS_FOLDER=%ROOT_FOLDER%\_xlsdk\run\windows\outputs

rem //-------------------------------------------------------------------------
rem // Key settings.
rem //-------------------------------------------------------------------------
set PAUSE_TIME_IN_MS=30000

rem //-------------------------------------------------------------------------
rem // Other settings.
rem //-------------------------------------------------------------------------
set RESUME_DELAY_IN_MS=30000
set DELAYED_DCTL_IN_MS=90000
set TIME_DATA=no
set KEY_DATA=no
set PERFORMANCE_DATA=no
set IN_CYCLE_PERFORMANCE_DATA=no
set ADDRESS_IPV4=127.0.0.1
set PORT=49260
set XLOADER_LOCK=yes
set OUTPUT_FILE_NAME=battery_use

rem //-------------------------------------------------------------------------
rem // Common logger library's settings.
rem //-------------------------------------------------------------------------
set LOGGER_MODELER_OPTIONS=deferred_logger_stop=yes
set LOGGER_LIBRARY_001=ll='!BIN_FOLDER!\sql_logger.dll','db_differential_elaspsed_time=yes db_wal=yes db_wal_autocheckpoint=0 db_cache=yes db_cache_size=auto db_synchronous=off db_journal_mode=off db_locking_mode=exclusive delayed_dctl=summarize dctl_process_delay=%DELAYED_DCTL_IN_MS% do_not_signal_delayed_dctl=yes'

rem //-------------------------------------------------------------------------
rem // Payloads' settings.
rem //-------------------------------------------------------------------------
set INPUT_LIBRARY_001=il='!BIN_FOLDER!\intel_acpi_battery_input.dll'


rem //-------------------------------------------------------------------------
rem // By-pass the interactive menu if an argument has been provided.
rem //-------------------------------------------------------------------------
:ENTRY
if "%1" == "" goto MENU
rem //-------------------------------------------------------------------------
rem       0000
rem       1234
choice /C SODQ /N /D %1 /T 0
goto GOT_ARGUMENT_FROM_CLI

rem //-------------------------------------------------------------------------
rem // Display options menu.
rem //-------------------------------------------------------------------------
:MENU

cls
@echo " [%DATE% - %TIME%]
@echo " [%CD%]
@echo "+----------------------------------------------------------------------+
@echo "| S: Start the collector.                                              |
@echo "| O: Stop the collector.                                               |
@echo "| D: Display the settings used during the operations.                  |
@echo "|                                                                      |
@echo "| Q: Quit.                                                             |
@echo "+----------------------------------------------------------------------+
@echo "[WARNING]: MUST be run as Administrator.

rem //-------------------------------------------------------------------------

rem //-------------------------------------------------------------------------
rem // Process user input.
rem //-------------------------------------------------------------------------
rem       0000
rem       1234
choice /C SODQ /N /M "Press the key of your choice:"

:GOT_ARGUMENT_FROM_CLI

if %ERRORLEVEL% == 1 goto START
if %ERRORLEVEL% == 2 goto STOP
if %ERRORLEVEL% == 3 goto DISPLAY
if %ERRORLEVEL% == 4 goto END
if %ERRORLEVEL% GTR 4 goto END

goto END

rem  @@@@   @@@@@   @@   @@@@@   @@@@@
rem @    @    @    @  @  @    @    @
rem @         @   @    @ @    @    @
rem  @@@@     @   @    @ @@@@@     @
rem      @    @   @@@@@@ @  @      @
rem      @    @   @    @ @   @     @
rem @@@@@     @   @    @ @    @    @
rem //-------------------------------------------------------------------------
rem // Start the collector.
rem //-------------------------------------------------------------------------
:START

set COMMAND="%BIN_FOLDER%\esrv.exe"^
 --start^
 --output_folder "!OUTPUTS_FOLDER!"^
 --time_in_ms^
 --pause %PAUSE_TIME_IN_MS%^
 --no_pl^
 --end_on_error^
 --priority_boost^
 --kernel_priority_boost^
 --shutdown_priority_boost^
 --do_not_use_system_error_logs^
 --resume_delay %RESUME_DELAY_IN_MS%^
 --library "%BIN_FOLDER%\intel_modeler.dll"^
 --device_options ^"^
 time=%TIME_DATA%^
 generate_key_file=%KEY_DATA%^
 performance=%PERFORMANCE_DATA%^
 in_cycle_performance=%IN_CYCLE_PERFORMANCE_DATA%^
 output=%OUTPUT_FILE_NAME%^
 output_folder='!OUTPUTS_FOLDER!'^
 lock_xls=%XLOADER_LOCK%^
 deferred_logger_stop=yes^
 do_not_signal_flush=yes^
 !LOGGER_LIBRARY_001!^
 !INPUT_LIBRARY_001!^
 ^"^
 --address %ADDRESS_IPV4%^
 --port %PORT%
set /A COMMANDS_COUNT=COMMANDS_COUNT+1
echo COMMAND[%COMMANDS_COUNT%][%DATE% - %TIME%]=[%COMMAND%]
if %DEBUG% == YES echo COMMAND[%COMMANDS_COUNT%]=[%COMMAND%]
if %EXECUTE% == YES (
	%COMMAND%
)

goto END

rem  @@@@   @@@@@  @@@@  @@@@@
rem @    @    @   @    @ @    @
rem @         @   @    @ @    @
rem  @@@@     @   @    @ @@@@@
rem      @    @   @    @ @
rem      @    @   @    @ @
rem @@@@@     @    @@@@  @
rem //-------------------------------------------------------------------------
rem // Stop the collector.
rem //-------------------------------------------------------------------------
:STOP

set COMMAND="%BIN_FOLDER%\esrv.exe"^
 --stop^
 --address %ADDRESS_IPV4%^
 --port %PORT%
set /A COMMANDS_COUNT=COMMANDS_COUNT+1
echo COMMAND[%COMMANDS_COUNT%][%DATE% - %TIME%]=[%COMMAND%]
if %DEBUG% == YES echo COMMAND[%COMMANDS_COUNT%]=[%COMMAND%]
if %EXECUTE% == YES (
	%COMMAND%
)

goto END

rem @@@@@  @@@@@   @@@@  @@@@@  @        @@    @  @
rem @    @   @    @    @ @    @ @       @  @   @  @
rem @    @   @    @      @    @ @      @    @  @  @
rem @    @   @     @@@@  @@@@@  @      @    @   @
rem @    @   @         @ @      @      @@@@@@   @
rem @    @   @         @ @      @      @    @   @
rem @@@@@  @@@@@  @@@@@  @      @@@@@@ @    @   @
rem //-------------------------------------------------------------------------
rem // Echo the settings used during the operations.
rem //-------------------------------------------------------------------------
:DISPLAY
cls

rem //-------------------------------------------------------------------------
rem // Display configurations.
rem //-------------------------------------------------------------------------
echo //-------------------------------------------------------------------------
echo // Debug and execution settings.
echo //-------------------------------------------------------------------------
echo DEBUG:.....................................................................[!DEBUG!]
echo EXECUTE:...................................................................[!EXECUTE!]
echo.
echo //-------------------------------------------------------------------------
echo // Folders settings.
echo //-------------------------------------------------------------------------
echo ROOT_FOLDER:...............................................................[!ROOT_FOLDER!]
echo BIN_FOLDER:................................................................[!BIN_FOLDER!]
echo OUTPUTS_FOLDER:............................................................[!OUTPUTS_FOLDER!]
echo.
echo //-------------------------------------------------------------------------
echo // Key settings.
echo //-------------------------------------------------------------------------
echo PAUSE_TIME_IN_MS:..........................................................[!PAUSE_TIME_IN_MS!]
echo OS_CLOCK_IN_MS:............................................................[!OS_CLOCK_IN_MS!]
echo HW_CLOCK_IN_MS:............................................................[!HW_CLOCK_IN_MS!]
echo PROCESS_PAUSE_TIME_IN_MS:..................................................[!PROCESS_PAUSE_TIME_IN_MS!]
echo SAMPLER_PAUSE_TIME_IN_MS:..................................................[!SAMPLER_PAUSE_TIME_IN_MS!]
echo.
echo //-------------------------------------------------------------------------
echo // Other settings.
echo //-------------------------------------------------------------------------
echo RESUME_DELAY_IN_MS:........................................................[!RESUME_DELAY_IN_MS!]
echo DELAYED_DCTL_IN_MS:........................................................[!DELAYED_DCTL_IN_MS!]
echo TIME_DATA:.................................................................[!TIME_DATA!]
echo KEY_DATA:..................................................................[!KEY_DATA!]
echo PERFORMANCE_DATA:..........................................................[!PERFORMANCE_DATA!]
echo IN_CYCLE_PERFORMANCE_DATA:.................................................[!IN_CYCLE_PERFORMANCE_DATA!]
echo ADDRESS_IPV4:..............................................................[!ADDRESS_IPV4!]
echo PORT:......................................................................[!PORT!]
echo XLOADER_LOCK:..............................................................[!XLOADER_LOCK!]
echo.
echo //-------------------------------------------------------------------------
echo // Payloads settings.
echo //-------------------------------------------------------------------------
echo INPUT_LIBRARY_001:.........................................................[!INPUT_LIBRARY_001!]

echo.
echo //-------------------------------------------------------------------------
echo // Logger library settings.
echo //-------------------------------------------------------------------------
echo LOGGER_MODELER_OPTIONS:....................................................[!LOGGER_MODELER_OPTIONS!]
echo LOGGER_LIBRARY_001:........................................................[!LOGGER_LIBRARY_001!]
echo.
goto END

rem //-------------------------------------------------------------------------
rem // Branch / end hub.
rem //-------------------------------------------------------------------------
:END

if %ERRORLEVEL% == 4 goto EXIT
if not "%1"=="" goto EXIT
pause
goto MENU

:EXIT

@echo on
