# Makefile.template
# Просто передает управление локальным Makefile
# 
HW_DIRS := $(wildcard HW*)

.PHONY: all clean $(HW_DIRS)

all: $(HW_DIRS)

$(HW_DIRS):
	$(MAKE) -C $@

clean:
	for dir in $(HW_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
