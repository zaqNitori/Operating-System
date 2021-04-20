所有的程式範例都需要在linux系統下才可以運行

419prime.c
為利用pthread 計算質數的code file
gcc -pthread -o prime.out 419prime.c

415-multithread-pid.c
hw1的pid藉由生成多個pthread來達到自動
配置及釋放PID的功能

630-Mutex-pid.c
由於多個thread同時access shared date會
出現問題，因此加上了mutex lock，確保一次
只會有一個thread能夠access shared data.

