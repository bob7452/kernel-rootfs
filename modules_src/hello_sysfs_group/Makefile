obj-m += hello_sysfs_group.o

all: clean
	make -C ../../linux M=$(PWD) modules

clean:
	@echo "Clean Make"
	make -C ../../linux M=$(PWD) clean

