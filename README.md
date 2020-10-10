# linux-kernel-ata-sniffer
## TL;DR
ATA command sniffer for Linux.

Tested on linux kernel versions: 4.19.91, 5.4.0-48-generic.

The compiled kernel options must include: CONFIG_KALLSYMS, CONFIG_HAVE_DYNAMIC_FTRACE, CONFIG_FTRACE, CONFIG_FUNCTION_TRACER. On most distros these options are default included.

## How it works

I started to explore the kernel sources in order to understand which part of the Linux ATA driver is responsible for sending the ATA command.

I found that the unexported function ata_qc_issue is the one that actually sends the commands.

The kernel module hooks that function using ftrace and every call to the function would be intercepted by our hook function which prints the ATA command id.

Based on https://www.apriorit.com/dev-blog/546-hooking-linux-functions-2:

Ftrace is an internal tracer designed to help out developers and designers of systems to find what is going on inside the kernel.
Ftrace is based on the compiler options -pg and -mfentry. These kernel options insert the call of a special tracing function — mcount() or __fentry__() at the beginning of every function. 
Calling ftrace from every function is, of course, pretty costly. This is why there’s an optimization available for popular architectures — dynamic ftrace. If ftrace isn’t in use, it nearly doesn’t affect the system because the kernel knows where the calls mcount() or __fentry__() are located and replaces the machine code with nop (a specific instruction that does nothing) at an early stage. And when Linux kernel trace is on, ftrace calls are added back to the necessary functions.

### Note:
You must compile the kernel with the needed options for ftrace. This will tell the compiler to insert at the beginning of every function a call to mcount() (or __fentry()__ when a newer compiler is in use).

## Limitation
If someone inside the kernel will send the command directly without using libATA the kernel module won't intercept it.

## Usage
cd linux-kernel-ata-sniffer

make

insmod ata_sniffer.ko

DONE!!!
