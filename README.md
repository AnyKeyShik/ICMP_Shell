# ICMP Shell Linux kernel module

<a href="https://github.com/AnyKeyShik/ICMP_Shell/blob/master/LICENSE">
<img src ="https://img.shields.io/github/license/AnyKeyShik/ICMP_Shell.svg" />
</a>

I have wanted to enter kernel development for a very long time. And now at work I need to understand Linux kernel very deep. And this is just practice for better understand the deepest mechanics of it.

## Getting started

#### Requirements

To compile and run this project, you will need:
* The GNU make
* The GNU GCC
* A Linux x64 operating system
* Installed headers of linux kernel

#### Running the code

For build this module:
```
git clone https://github.com/AnyKeyShik/ICMP_Shell.git
cd ICMP_Shell
make
```

For turn it on:
```
sudo insmod icmp_shell.ko
```

For turn it off:
```
sudo rmmod icmp_shell.ko
```
