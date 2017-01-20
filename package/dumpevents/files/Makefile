M4 ?= m4

.PHONY: all clean

all:
	$(M4) -P filter_events.lu4 >filter_events.lua
	$(M4) -P filter_events_by_time.lu4 >filter_events_by_time.lua

clean:
	rm -f filter_events.lua
	rm -f filter_events_by_time.lua

install: all
	cp filter_events.lua /usr/bin/
	cp filter_events_by_time.lua /usr/bin/
