CALL _Clean.bat
Converter\Converter\bin\Release\Converter.exe

COPY /B out\index.html ..\..\ornithopter\home\.

CALL C:\home\bat\env\env.bat
C:\Factory\Tools\ncp.exe /S %mimiko% /F /UP out\GeTunnel.html GeTunnel.html
C:\Factory\SubTools\nrun.exe /S %mimiko% UpdateGeTunnelHtml

PAUSE
