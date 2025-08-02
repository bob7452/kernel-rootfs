.PHONY: start
CPIO_FILE := rootfs.cpio
SUB_MODULE_DIR := modules_src/*

start:
	bash start_qemu.sh
init:
	bash init.sh
clean:
	@echo "Clean"
	@if [ -f $(CPIO_FILE) ]; then \
		echo "$(CPIO_FILE) exists"; \
		rm $(CPIO_FILE); \
	else \
		echo "Not find $(CPIO_FILE)"; \
	fi
rebuild_module:
	@echo "rebuild sub module"
	@for dir in $(SUB_MODULE_DIR); do \
		echo "Building $$dir"; \
		cd $$dir && pwd && $(MAKE) clean && $(MAKE); cd -; \
	done
rebuild_cpio: clean rebuild_module
	@echo "rebuild cpio"
	cd rootfs && find . | cpio -o --format=newc > ../$(CPIO_FILE)
	find . \( -path "./busybox" -o -path "./linux" -o -path "./rootfs" \) -prune -o -type f -name "*.ko" -exec cp {} rootfs/modules/ \;
	cd rootfs && find . | cpio -o --format=newc > ../$(CPIO_FILE)