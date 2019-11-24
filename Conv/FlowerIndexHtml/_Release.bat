CALL _Clean.bat
Converter\Converter\bin\Release\Converter.exe

COPY /B out\Hatena.html ..\..\ornithopter\home\.
COPY /B out\index.html  ..\..\ornithopter\home\.

CALL C:\home\bat\env\env.bat
C:\Factory\Tools\ncp.exe /S %mimiko% /F /UP out\GeTunnel.html GeTunnel.html
C:\Factory\SubTools\nrun.exe /S %mimiko% UpdateGeTunnelHtml

@echo ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
@echo ■ GeTunnel.html は BlueFish を再起動しないと更新されません ■
@echo ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
@echo ■ index.html も Post2 の _Release をしないと更新されません ■
@echo ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

PAUSE
