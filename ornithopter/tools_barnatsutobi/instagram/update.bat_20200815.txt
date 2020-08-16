MD C:\appdata
MD C:\appdata\barnatsutobi
MD C:\appdata\barnatsutobi\instagram

DEL C:\appdata\barnatsutobi\instagram\successful.flg

FOR /F "DELIMS=" %%A IN ('C:\Factory\Tools\mkpw.exe /9 /-V 10') DO SET @DsUserId=%%A

C:\app\Kit\HGet\HGet.exe ^
	/H cookie "ds_user_id=%@DsUserId%;" ^
	/U "https://www.instagram.com/barnatsutobi/" ^
	/RSF C:\appdata\barnatsutobi\instagram\successful.flg ^
	/RHF C:\appdata\barnatsutobi\instagram\res-header.out ^
	/RBF C:\appdata\barnatsutobi\instagram\res-body.out

SET @DsUserId=

IF NOT EXIST C:\appdata\barnatsutobi\instagram\successful.flg GOTO END
mkIndexHtml.exe C:\appdata\barnatsutobi\instagram\res-body.out ..\..\home_barnatsutobi\instagram\index.html
:END
