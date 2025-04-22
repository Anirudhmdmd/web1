@echo off
echo Building BLN Node Package...

rem Clean and create package dir
if exist node_package rmdir /s /q node_package
mkdir node_package

rem Copy required files
copy app_bln.c node_package\
copy cfg.xml node_package\
copy label node_package\

echo Build complete.
