@echo off

echo copy from:
set /p from=
echo new name:
set /p to=

powershell -executionpolicy unrestricted ./CreateNewProject.ps1 %from% %to%