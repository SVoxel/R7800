#!/usr/bin/env python
#
# Test loading router GUI inside web browser.

ROUTER_IP = "127.0.0.1"
#ROUTER_IP = "192.168.1.1"

import sys

# Selenium (web-page automation and interaction)
from selenium import webdriver
#from selenium.common.exceptions import NoSuchElementException # unused for now
#from selenium.webdriver.common.keys import Keys               # unused for now

# Open browser & navigate to router
browser = webdriver.Firefox()
browser.get("http://%s" % ROUTER_IP)
browser.implicitly_wait(30) # Duration (sec) to poll when looking for objects
found_gui = "Killer 9100" in browser.title
browser.close()
if found_gui:
    print "GUI loads correctly"
    sys.exit(0)
else:
    print "Web browser does not see router GUI at %s" % ROUTER_IP
    sys.exit(1)

