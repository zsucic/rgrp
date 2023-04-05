# Log Search

Log Search is a simple command-line tool written in ANSI C that searches for regular expressions within log files. It can also break the log file into groups using a specified regular expression and print out any group that matches the search regex. The idea here is to first separate log file into log entries even when they are multiline before searching for specific regex. That way any returned results will contain the whole log entry rather than just the line where the pattern appears.

## Prerequisites

To compile the code, you need to have the PCRE library installed on your system.
e.g. on Debian based systems:
```
sudo apt-get install libpcre3-dev
```

## Build Process

You can build the project using the provided Makefile.

### Build with Make

Run the following command in the terminal:

```sh
make
```
### Manual Compile with GCC
To manually compile the code, run the following command in the terminal:
```
sh

gcc -Wall -Wextra -o rgrp rgrp.c -lpcre
```
## Usage
```sh
./rgrp [-i] <search_regex> <log_file> [group_separator_regex]
```
+ -i: (Optional) Enable case-insensitive search.

+ <search_regex>: The regex pattern you want to search for.

+ <log_file>: The path to the log file.

+ [group_separator_regex]: (Optional) The regex pattern to use as the group separator. Defaults to "^(\d{4}-\d{2}-\d{2}T\d{2}\:\d{2}\:\d{2}\.\d{6}\+\d{4},)".

## Examples

Search for the word "error" in the log file my_log.log:
```
sh
./rgrp "error" my_log.log
```

Search for the word "error" in the log file my_log.log, with case-insensitive search:

```sh
./rgrp -i "error" my_log.log
```

Search for the word "error" in the log file my_log.log, using a custom group separator regex:

```
sh
./rgrp "error" my_log.log "^(\\d{2}/\\d{2}/\\d{4} \\d{2}:\\d{2}:\\d{2})"
```

Real world example: Search for "Clock" in dmesage log
- case sensitive:
```
$ ./rgrp "Clock" /var/log/dmesg "\["
[    0.338187] kernel: acpi PNP0A08:00: _OSC: OS supports [ExtendedConfig ASPM ClockPM Segments MSI EDR HPX-Type3]
$ 
```
- case insensitive:
```
$ ./rgrp -i "Clock" /var/log/dmesg "\["
[    0.037037] kernel: clocksource: refined-jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645519600211568 ns
[    0.102665] kernel: clocksource: hpet: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 133484873504 ns
[    0.122688] kernel: clocksource: tsc-early: mask: 0xffffffffffffffff max_cycles: 0x29ba31d3661, max_idle_ns: 440795206745 ns
[    0.261053] kernel: Measured 4640 cycles TSC warp between CPUs, turning off TSC clock.
[    0.267410] kernel: clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645041785100000 ns
[    0.338187] kernel: acpi PNP0A08:00: _OSC: OS supports [ExtendedConfig ASPM ClockPM Segments MSI EDR HPX-Type3]
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
