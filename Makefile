ASSEMBLER_DIR = Assembler
MACHINE_DIR = Machine

default: build
.PHONY: clean

build:
	$(MAKE) -C $(ASSEMBLER_DIR)
	$(MAKE) -C $(MACHINE_DIR)

clean:
	$(MAKE) -C $(ASSEMBLER_DIR) clean
	$(MAKE) -C $(MACHINE_DIR) clean
