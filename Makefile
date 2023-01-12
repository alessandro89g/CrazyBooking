SUBDIRS := $(wildcard CrazyBookingServer CrazyBookingClient CrazyBookingClient2)

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
