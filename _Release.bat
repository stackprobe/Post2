IF NOT EXIST Post2\Post2.exe START ?_No_Post2_exe

C:\Factory\Tools\Cluster.exe /OW /M S:\リリース物\ornithopter.clu ornithopter
C:\Factory\Tools\Cluster.exe /OW /M S:\リリース物\Post2.clu Post2

rem ■■■■■■■■■■■■■■
rem ■■■ syncRev してね ■■■
rem ■■■■■■■■■■■■■■

IF NOT "%1" == "/-P" PAUSE
