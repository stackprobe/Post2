@rem AutoRelease.bat から派生

CLS
rem リリースします。
PAUSE

CALL newcsrr

CALL ff
cx **
CD /D %~dp0.

CALL qq
cx **

CALL Release.bat /-P

MOVE out\* S:\リリース物\.

START "" /B /WAIT /DC:\home\bat syncRev

rem **** QUICK RELEASE COMPLETED ****
