IF NOT EXIST Post2\Post2.exe START ?_No_Post2_exe

C:\Factory\Tools\Cluster.exe /OW /M out\ornithopter.clu ornithopter
C:\Factory\Tools\Cluster.exe /OW /M out\Post2.clu Post2

rem Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°
rem Å°Å°Å° syncRev ÇµÇƒÇÀ Å°Å°Å°
rem Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°Å°

IF NOT "%1" == "/-P" PAUSE
