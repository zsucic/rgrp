# rgrp
## Multiline Log Grep tool
### Building Sources
to Build, first make sure you have libpcre3-dev installed, e.g.
sudo apt-get install libpcre3-dev

then compile with: make
e.g.
```
$ git clone https://github.com/zsucic/rgrp.git
Cloning into 'rgrp'...
remote: Enumerating objects: 15, done.
remote: Counting objects: 100% (15/15), done.
remote: Compressing objects: 100% (12/12), done.
remote: Total 15 (delta 1), reused 0 (delta 0), pack-reused 0
Receiving objects: 100% (15/15), 4.11 KiB | 4.11 MiB/s, done.
Resolving deltas: 100% (1/1), done.
$ cd rgrp
$ make
gcc -Wall -Wextra -c rgrp.c -o rgrp.o
gcc -Wall -Wextra rgrp.o -o rgrp -lpcre
$ ./rgrp
Usage: ./rgrp <search_regex> <log_file> [group_separator_regex]
$ 

```



if that isn't working for you for whatever reason, you can just do the compile without make with:
```
$ gcc -o rgrp rgrp.c -lpcre
$ ./rgrp
Usage: ./rgrp <search_regex> <log_file> [group_separator_regex]
$ 
```

### Usage
To use rgrp provide the following information (M) -> mandatory (O) -> optional:
- search_regex (M) -> regular expression that will be used to match agains string blocks (i.e. not necessarily lines)
- log_file (M) -> file path of the log file you want to search through
- group_separator_regex (O) -> regular expression that will be used to find beginnings of string blocks and separate log file using those. The default regex in use is: __"^(\d{4}-\d{2}-\d{2}T\d{2}\:\d{2}\:\d{2}\.\d{6}\+\d{4},)"__ which will match datetime followed by a comma in the following format, e.g. __2023-03-31T15:55:02.034000+0000,__

Below is a simple example on how to adjust the group separator to match e.g. /var/log/dmesg (which has entries like this
```
[    1.192276] kernel: Freeing unused decrypted memory: 2036K
[    1.192724] kernel: Freeing unused kernel image (initmem) memory: 3240K
[    1.236461] kernel: Write protecting the kernel read-only data: 30720k
[    1.237178] kernel: Freeing unused kernel image (text/rodata gap) memory: 2036K

```

e.g. without adjusting the group separator, our search returns empty:
```
$ ./rgrp "trace_clock=local" /var/log/dmesg
$
```
however, when adjusted to just look for "[" as the beginning of a log entry, it returns results...
```
$ ./rgrp "trace_clock=local" /var/log/dmesg "\["
[    1.191029] kernel: Unstable clock detected, switching default tracing clock to "global"
                       If you want to keep using the local clock, then add:
                         "trace_clock=local"
                       on the kernel command line
$ 

```
just to be clear, it also works with single line entries, e.g.

```
$ ./rgrp "clock" /var/log/dmesg "\["
[    0.037037] kernel: clocksource: refined-jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645519600211568 ns
[    0.102665] kernel: clocksource: hpet: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 133484873504 ns
[    0.122688] kernel: clocksource: tsc-early: mask: 0xffffffffffffffff max_cycles: 0x29ba31d3661, max_idle_ns: 440795206745 ns
[    0.261053] kernel: Measured 4640 cycles TSC warp between CPUs, turning off TSC clock.
[    0.267410] kernel: clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645041785100000 ns
[    0.351603] kernel: PTP clock support registered
[    0.356803] kernel: clocksource: Switched to clocksource hpet
[    0.370482] kernel: clocksource: acpi_pm: mask: 0xffffff max_cycles: 0xffffff, max_idle_ns: 2085701024 ns
[    0.607556] kernel: rtc_cmos 00:01: setting system clock to 2023-03-26T16:13:55 UTC (1679847235)
[    1.191029] kernel: Unstable clock detected, switching default tracing clock to "global"
                       If you want to keep using the local clock, then add:
                         "trace_clock=local"
                       on the kernel command line
$ 

```

