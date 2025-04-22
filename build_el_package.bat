@echo off
echo Building BLN Node Package...

rem Clean and create package dir
if exist node_package rmdir /s /q node_package
mkdir node_package

rem Copy required files
copy app_bln.c node_package\
copy cfg.xml node_package\
copy label node_package\

rem Zip the files (optional if Jenkins will archive the folder directly)
powershell -Command "Compress-Archive -Path node_package\* -DestinationPath bln_node_package.zip"

echo Build complete.
