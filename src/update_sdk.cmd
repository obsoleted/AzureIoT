REM This script replaces the SDK folder with the latest bits from the Azure SDK repository.
REM It removes some files we do not need.  It currently targets the develop branch until fixes are
REM merged into master.

if exist "%~dp0\temp" rd /s /q "%~dp0\temp"
if exist "%~dp0\sdk" rd /s /q "%~dp0\sdk"
if exist "%~dp0\azure_c_shared_utility" rd /s /q "%~dp0\azure_c_shared_utility"

mkdir "%~dp0\temp"
mkdir "%~dp0\sdk"
mkdir "%~dp0\azure_c_shared_utility"

cd /D "%~dp0\temp"

git clone https://github.com/Azure/azure-iot-sdks.git --recursive

cd azure-iot-sdks
echo Upstream HEAD @ > metadata.txt
git rev-parse HEAD >> metadata.txt

cd "%~dp0\temp"

copy "%~dp0\temp\azure-iot-sdks\metadata.txt" "%~dp0\sdk"

copy "%~dp0\temp\azure-iot-sdks\c\iothub_client\src\" "%~dp0\sdk"
copy "%~dp0\temp\azure-iot-sdks\c\iothub_client\inc\" "%~dp0\sdk"
copy "%~dp0\temp\azure-iot-sdks\c\serializer\src\" "%~dp0\sdk"
copy "%~dp0\temp\azure-iot-sdks\c\serializer\inc\" "%~dp0\sdk"
copy "%~dp0\temp\azure-iot-sdks\c\azure-c-shared-utility\src\" "%~dp0\sdk"
copy "%~dp0\temp\azure-iot-sdks\c\azure-c-shared-utility\inc\azure_c_shared_utility\" "%~dp0\azure_c_shared_utility"
copy "%~dp0\temp\azure-iot-sdks\c\azure-c-shared-utility\adapters\agenttime.c" "%~dp0\sdk"
copy "%~dp0\temp\azure-iot-sdks\c\azure-c-shared-utility\adapters\tickcounter_tirtos.c" "%~dp0\sdk"

del "%~dp0\azure_c_shared_utility\iot_logging.h"
copy "%~dp0\iot_logging.h" "%~dp0\azure_c_shared_utility"

del "%~dp0\sdk\iothubtransportamqp.*"
del "%~dp0\sdk\iothubtransportamqp_websockets.*"
del "%~dp0\sdk\iothubtransportmqtt.*"
del "%~dp0\sdk\tickcounter.*"
del "%~dp0\sdk\tlsio*.*"
del "%~dp0\sdk\constbuffer.*"


cd ..

rmdir /S "%~dp0\temp"
