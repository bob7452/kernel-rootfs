#!/bin/bash

CUR_FILE=$(readlink -f "$0")
CUR_DIR=$(dirname $CUR_FILE)
ROOTFS_DIR=$CUR_DIR/rootfs

cd $CUR_DIR
echo "step 0. create rootfs dirs at $CUR_DIR"

mkdir rootfs
cd rootfs
mkdir -p bin sbin etc proc sys usr/bin usr/sbin dev tmp


echo "step 1. linux kernel lastest version"
cd $CUR_DIR
git clone --depth=1 https://github.com/torvalds/linux.git
cd linux
make defconfig
make -j$(nproc)

echo "step 2. build busy box"
cd $CUR_DIR
git clone https://github.com/mirror/busybox.git
cd busybox
make distclean
make defconfig
make menuconfig
make -j$(nproc)
make CONFIG_PREFIX=../rootfs install

echo "step 3. Add init script to rootfs"

cat <<EOF > "$ROOTFS_DIR/init"
#!/bin/bash
mount -t proc none /proc
mount -t sysfs none /sys
echo "Welcome to minimal rootfs!"
exec /bin/sh
EOF

# Make the init script executable
chmod +x "$ROOTFS_DIR/init"

echo "init script created and made executable at $ROOTFS_DIR/init"
