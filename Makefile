.PHONY: start
CPIO_FILE := rootfs.cpio

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

rebuild_cpio: clean
	@echo "rebuild cpio"
	cd rootfs && find . | cpio -o --format=newc > ../$(CPIO_FILE)
