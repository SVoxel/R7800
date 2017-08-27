# Pad the json files to create jsonp files suitable for cross-site access
# from a browser.
#
# "make all" will take and .json files, pad them for jsonp and write
# them as *.js files.

JSON := $(wildcard *.json)
JSONP := $(JSON:%.json=%.js)

.PHONY: all
all: $(JSONP)

# Name of the callback function for jsonp
CALLBACK := jsonpCallback

%.js: %.json
	echo "$(CALLBACK)(" > $@
	cat $< >> $@
	echo ");" >> $@

.PHONY: clean
clean:
	rm -f $(JSONP)
