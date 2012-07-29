TimeMonitor
===========

The aim of this project is to identify time bottlenecks. It provides checkpoints where you can track time spent on small parts of the code in addition to the total time.

## Basic usage

Create a new TimeMonitor and start profiling. Add some timepoints to get more precisely where the most time is spent. Finally stop profiling and print the results.

```C++

TimeMonitor *monitor = tm_init();

...

tm_start_profiling(monitor);

...

tm_add_timepoint(monitor, "some custom timepoint description");

...

tm_stop_profiling(monitor);

...

tm_print_info(monitor);
````

The results are presented as follows.

````
| timepoint  1 |      1 ms (  0.00%) | input-case generation completed |
| timepoint  2 |  14276 ms ( 99.00%) | selection sort completed |
| timepoint  3 |      0 ms (  0.00%) | check 100000 results from selection sort algorithm completed |
------------------------------------------------------
[TOTAL] - elapsed time: 14278 (ms)
```