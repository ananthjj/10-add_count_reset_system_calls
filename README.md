# final-project-eizner-josyula

Challenges while installing and building kernel
-setting inadequate number of processors
-allocating inadequate amount of memory

1. Installing prerequisites
>> sudo sed -i "s/# deb-src/deb-src/g" /etc/apt/sources.list
>> sudo apt update -y
>> sudo apt install -y git fakeroot build-essential libncurses-dev xz-utils libssl-dev libelf-dev bc
>> sudo apt install -y bison flex zstd

2. Check your version with uname -r, and then pick a download at kernel.org that is as close to your kernel version as possible. Within your virtual machine, download Linux source code, using curl, 
# -O -J will set the output filename based on the URL
>> curl -O -J https://www.kernel.org/pub/linux/kernel/v4.x/linux-6.0.11.tar.xz
OR apt source linux

3. Decompress your tarball, e.g.:
>> tar -xvf linux-6.0.11.tar.xz —directory /home/ananth
>> cd linux-6.0.11

4. Change permissions and rename the folder
>> sudo chown -R ananth linux-6.0.11/
>> mv linux-6.0.11 linux-6.0.11-orig

5. Configure the kernel building process by copying the existing current boot configuration file so that we don’t have to edit the configuration
cd linux-6.0.11
cp /boot/config-$(uname-r) .config
make menuconfig 

6. Edit .config as follows:
set CONFIG_SYSTEM_REVOCATION_KEYS and CONFIG_SYSTEM_TRUSTED_KEYS from .config to null string

7. Compile the kernel
>> sudo make -j 8
	If compilation issue:
		First, sudo make clean

8. Install kernel modules and image
>> sudo make modules_install -j 4
>> sudo make install -j 4

9. Configure GRUB  (Did initially, but not needed since make install does that too)

10. To solve initrd too big issue, strip version
	cd /lib/modules/<kernel_version>
	sudo find . -name "*.ko" -exec strip --strip-	unneeded {} +
	cd /<path>/<to>/<linux-kernel_src>
	sudo make install -j 4

11. Navigate to ~/linux-6.0.11/arch/x86/entry/syscalls/ and open syscall_64.tbl, Scroll to the bottom of the first section and reserve syscalls numbers.  It is a tab between each column (not a space)

12. We have set everything up, the only thing missing is the implementation of these new syscalls.
	Navigate to ~/linux-6.0.11/kernel and edit sys.c .
	Go into Makefile under the same directory and add your new file to the building process
# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the linux kernel.
# 
obj-y = fork.o exec_domain.o panic.o \
cpu.o exit.o softirq.o resource.o \ 
sysctl.o sysctl_binary.o capability.o ptrace.o user.o \ 
signal.o sys.o umh.o workqueue.o pid.o task_work.o \ 
extable.o params.o \ 
kthread.o sys_ni.o nsproxy.o \ 
notifier.o ksysfs.o cred.o reboot.o \ 
async.o range.o smpboot.o ucount.o hello_syscall.o syscalls_counts.o

13. In kernel/sys.c write the SYSCALL_DEFINE#() functions. SYSCALL_DEFINEN is a family of macros that make it easy to define a system call with N arguments. The first argument to the macro is the name of the system call (without sys_ prepended to it). The remaining arguments are pairs of type and name for the parameters. When the system call has one argument, we use SYSCALL_DEFINE1, 
14. Modify fs/exec.c and kernel/fork.c to increment counts
15. In init/main.c declare a struct variable and initialize all fields to 0
16. For the struct to be accessible from fork, exec and sys, define struct in include/linux/syscalls.h and declare the global variable as extern
17. Test the Syscall by creating a program test.c that uses syscall(SYS_retrieveCount_SC, count_struct) and syscall(SYS_resetCount_SC) 
18. Compile it with gcc -o test test.c
19. Execute it as ./test

References:
https://levelup.gitconnected.com/create-your-own-system-calls-part-2-process-weights-97d5e66f56f3

https://www.youtube.com/watch?v=vdOcuawlelI
https://brennan.io/2016/11/14/kernel-dev-ep3/
https://www.cs.swarthmore.edu/~kwebb/cs45/s20/labs/lab2.html

After running sudo make -j 8:<img width="553" alt="J-usb-vartax ko" src="https://user-images.githubusercontent.com/53486880/206886763-d9767b1c-4af9-4af0-a778-1eb1d107af1a.png">

After running sudo make modules_install -j 4:<img width="555" alt="bnodules6" src="https://user-images.githubusercontent.com/53486880/206886773-7f2557f1-448e-442d-b574-fe2ce5a9c400.png">

After running sudo make install -j 4:<img width="689" alt="INSTALL" src="https://user-images.githubusercontent.com/53486880/206886778-666c785e-2d91-426b-be1e-19d829585b4e.png">

If when restarting the new kernel:<img width="482" alt="Ubuntu Clone (Kernal Project)  Running" src="https://user-images.githubusercontent.com/53486880/206886791-24f786d3-67a2-474a-9f5b-54c701367cc5.png"><img width="540" alt="Ubuntu Clone (Kernal Project)  Running)" src="https://user-images.githubusercontent.com/53486880/206886792-999d6da4-c6c5-48c8-9ff4-c7711143cca0.png">

Go back to the original kernel and do the following:<img width="555" alt="(sudo  password for ananth" src="https://user-images.githubusercontent.com/53486880/206886798-c6630cd6-9289-4a90-9aaf-5ada27d70f48.png">

Now upon starting the new kernel one sees a successful boot:<img width="541" alt="Ubuntu Clone (Kernal Project)  Running" src="https://user-images.githubusercontent.com/53486880/206886800-e3cf22e6-17cb-4416-ab8f-799e67a1b318.png">
