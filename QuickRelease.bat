@rem AutoRelease.bat ����h��

CLS
rem �����[�X���܂��B
PAUSE

CALL newcsrr

CALL ff
cx **
CD /D %~dp0.

CALL qq
cx **

CALL Release.bat /-P

MOVE out\* S:\�����[�X��\.

START "" /B /WAIT /DC:\home\bat syncRev

rem **** QUICK RELEASE COMPLETED ****
