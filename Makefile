obj-m +=mem_cdev1.o
KDIR=/embedded/SUNXI_MAINLINE/rootfs/lib/modules/4.0.4-dirty/build
all:
	$(MAKE) -C $(KDIR) ARCH=arm CROSS_COMPILE=arm-linux- SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) ARCH=arm CROSS_COMPILE=arm-linux- SUBDIRS=$(PWD) clean
