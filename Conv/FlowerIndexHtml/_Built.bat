CALL _Clean.bat
Converter\Converter\bin\Release\Converter.exe

COPY /B out\index.html  ..\..\ornithopter\home\.
COPY /B out\Hatena.html ..\..\ornithopter\home\.

CALL C:\home\bat\env\env.bat
C:\Factory\Tools\ncp.exe /S %mimiko% /F /UP out\GeTunnel.html GeTunnel.html
C:\Factory\SubTools\nrun.exe /S %mimiko% UpdateGeTunnelHtml

@echo ��������������������������������������������������������������
@echo �� GeTunnel.html �� BlueFish ���ċN�����Ȃ��ƍX�V����܂��� ��
@echo ��������������������������������������������������������������
@echo �� index.html �� Post2 �� _Release �����Ȃ��ƍX�V����܂��� ��
@echo ��������������������������������������������������������������
