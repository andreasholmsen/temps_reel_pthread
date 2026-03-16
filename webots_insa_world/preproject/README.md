
## Result from running program with one or multiple processors

        root@insa-21098:~/Téléchargements/temps_reel_pthread/webots_insa_world/preprojec
        t# taskset -c 1 ./main
        Process with load 20 finished!
        Process with load 35 finished!
        Time taken: 3.99 s
        Time taken: 5.475 s
        root@insa-21098:~/Téléchargements/temps_reel_pthread/webots_insa_world/preprojec
        t# ./main
        Process with load 20 finished!
        Process with load 35 finished!
        Time taken: 3.141 s
        Time taken: 5.508 s
        root@insa-21098:~/Téléchargements/temps_reel_pthread/webots_insa_world/preprojec
        t# 
