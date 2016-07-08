.section .data
.section .text
.globl _start
_start:
movl $1, %eax # this is the linux kernel command
movl $4, %ebx # this is the status number we will
int  $0x80 # this wakes up the kernel to run
