#!/bin/bash
qemu-system-x86_64 \
  -kernel linux/arch/x86/boot/bzImage \
  -initrd rootfs.cpio \
  -nographic \
  -append "console=ttyS0 init=/init"
