KERNELDIR:=/lib/modules/$(shell uname -r)/build
NAME:=icmp_shell
SRC:=main

obj-m = $(NAME).o
$(NAME)-objs = $(SRC).o

all: $(NAME).ko

$(NAME).ko: $(SRC).c
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
