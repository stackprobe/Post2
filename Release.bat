IF NOT EXIST Post2\Post2.exe START ?_No_Post2_exe

C:\Factory\Tools\Cluster.exe /OW /M out\ornithopter.clu ornithopter
C:\Factory\Tools\Cluster.exe /OW /M out\Post2.clu Post2

IF NOT "%1" == "/-P" PAUSE
