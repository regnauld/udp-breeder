The udp-breeder utility listens on a specified UDP port and forward all
packets to one or more targets.  The source address and port of forwarded
packets remains unchanged.

Typically udp-breeder has been used to redistribute UDP traffic
such as Netflow flows (IOS has limitations on the number of export
destinations), SNMP traps, or even DNS traffic.

Note that udp-breeder doesn't support IPv6 yet.

# Compiling

## FreeBSD

Use the provided port - assumes the sources are under src/ in the port
directory

## Linux (Ubuntu)

Currently there is no Makefile for Linux. Contributions and packages
welcome :)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ sudo apt-get install libnet1 libnet1-dev
$ cc -c *.c
$ cc -o udp-breeder -lnet *.o
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Invoking

Let's say you wanted to receive Netflow flows from one probe, on local
port 9000, and resend that traffic to multiple colectors (10.0.0.1, 10.0.0.2,
and 10.0.0.3) on ports 9001, 9002 and 9003 respectively.

You'd configure the probe (router, switch, or other) to send the traffic
to the host you're running udp-breeder on to port 9000, then run:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
$ udp-breeder -p 9000 10.0.0.1:9001 10.0.0.2:9002 10.0.0.3:9003
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Options

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-f  don't become a daemon, remain in the foreground
-p  port to listen on
-q  quiet mode
-l  log to the following file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
