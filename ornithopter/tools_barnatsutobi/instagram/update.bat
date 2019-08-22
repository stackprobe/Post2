MD C:\appdata
MD C:\appdata\barnatsutobi
MD C:\appdata\barnatsutobi\instagram

DEL C:\appdata\barnatsutobi\instagram\successful.flg

C:\app\Kit\HGet\HGet.exe ^
	/U "https://www.instagram.com/barnatsutobi/" ^
	/RSF C:\appdata\barnatsutobi\instagram\successful.flg ^
	/RHF C:\appdata\barnatsutobi\instagram\res-header.out ^
	/RBF C:\appdata\barnatsutobi\instagram\res-body.out

IF NOT EXIST C:\appdata\barnatsutobi\instagram\successful.flg GOTO END
mkIndexHtml.exe C:\appdata\barnatsutobi\instagram\res-body.out ..\..\home_barnatsutobi\instagram\index.html
:END
