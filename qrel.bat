@rem QuickRelease.bat から派生

rem ビルド済みかチェック
IF NOT EXIST Post2\Post2.exe GOTO END

CLS
rem リリースします。
PAUSE

rem CALL newcsrr

rem CALL ff
rem cx **
rem CD /D %~dp0.

rem CALL qq
rem cx **

CALL Release.bat /-P

MOVE out\* S:\リリース物\.

START "" /B /WAIT /DC:\home\bat syncRev

rem **** QUICK RELEASE COMPLETED ****

:END
