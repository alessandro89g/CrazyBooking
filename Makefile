SUBDIRS := $(wildcard CrazyBookingServer CrazyBookingClient)

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
