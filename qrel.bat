@rem QuickRelease.bat ����h��

rem �r���h�ς݂��`�F�b�N
IF NOT EXIST Post2\Post2.exe GOTO END

CLS
rem �����[�X���܂��B
PAUSE

rem CALL newcsrr

rem CALL ff
rem cx **
rem CD /D %~dp0.

rem CALL qq
rem cx **

	CALL AllRemake.bat

CALL Release.bat /-P

MOVE out\* S:\�����[�X��\.

START "" /B /WAIT /DC:\home\bat syncRev

rem **** QUICK RELEASE COMPLETED ****

:END
