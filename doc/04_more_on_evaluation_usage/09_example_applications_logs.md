# Example applications logs

Logs are output to the serial terminal.

The format is as follow:

• Log level: INF, ERR, DBG

• Time

• Function name

• Log

## TSN application logs

The TSN application has various counters and statistics which help to validate:

- application scheduling and processing timing statistics

- network traffic correctness and latency statistics

## Main TSN task logs

INF 1588004334 : tsn_task_stats_print : tsn task(20033600)

INF 1588004334 : tsn_task_stats_print : sched : 60000

INF 1588004334 : tsn_task_stats_print : sched early : 0

INF 1588004334 : tsn_task_stats_print : sched missed : 0

INF 1588004334 : tsn_task_stats_print : sched timeout : 1

INF 1588004334 : tsn_task_stats_print : clock discont : 1

- Scheduling counters (“sched” should increment of 10000 per second)

stats(20034418) sched err min 1698 mean 1870 max 6730 rms^2 3692509 stddev^2 192358 absmin 1698 absmax 8826

- Scheduling error statistics (difference between actual task scheduling time and programmed time)

n_slot 101 slot_size XXX

0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 3 54510 439 178 316 307 192 205 345 198 157 120 159 140 136 69 64 80 92 91 633 329 326 437 60 46 46 36 43 45 41 50 46 22 1 0 1 0 0 0 0 0 0 0 1 2 1 1 2 1 15 5 3 1 2 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0

- Scheduling error histogram (XXX ns bucket)

stats(20034870) processing time min 3264 mean 5319 max 15136 rms^2 34697464 stddev^2 6397299 absmin 3264 absmax 28192

- Processing time statistics (main task duration)

n_slot 101 slot_size XXX

0 0 0 28417 89 4913 6035 14506 1146 2854 846 339 626 11 200 16 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

- Processing time histogram (XXX ns bucket)

stats(20034CC8) total time min 4970 mean 7190 max 16882 rms^2 58531757 stddev^2 6826178 absmin 4970 absmax 29930

- Total time statistics (scheduling error + processing time)

n_slot 101 slot_size XXX

0 0 0 0 2 27530 900 10465 469 13284 1069 3947 923 586 484 69 267 2 2 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

- Total time histogram (XXX ns bucket)

## Network socket

INF 1588004334 : net_socket_stats_print : net rx socket(20033618) 0

INF 1588004334 : net_socket_stats_print : frames : 57333

INF 1588004334 : net_socket_stats_print : err : 0

INF 1588004334 : net_socket_stats_print : net tx socket(20033688) 0

INF 1588004334 : net_socket_stats_print : frames : 60001

INF 1588004334 : net_socket_stats_print : err : 1

- Low-level network socket. Only frames relevant to the network socket (Layer 2) are counted here.

## Application socket

INF 1588004334 : socket_stats_print : cyclic rx socket(20002300) net_sock(20033618) peer id: 0

INF 1588004334 : socket_stats_print : valid frames : 49154

INF 1588004334 : socket_stats_print : err id : 0

INF 1588004334 : socket_stats_print : err ts : 8179

INF 1588004334 : socket_stats_print : err underflow : 2698

INF 1588004334 : socket_stats_print : link up

- Application header is checked at this level. Also, the timestamps from the remote peers are verified as well which guarantee that only expected and in sequence data is processed.

  stats(200027A0) traffic latency min 38080 mean 38202 max 38320 rms^2 1866461389 stddev^2 1720 absmin 38080 absmax 38080

- Traffic latency statistics (the difference between the theoretical scheduling time of the peer that sent the frame and the frame receive time (measured by the MAC))

  n_slot 101 slot_size 1000

  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 49153 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

- Traffic latency histogram (1μs bucket)
