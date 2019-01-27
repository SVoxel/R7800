#!/usr/bin/awk -f
# AWK script to obfuscate mac addresses

BEGIN {
	RS="\""
	ORS=""
	cmd = "getsalt"
	cmd | getline salt
}

/[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]/ {
	# Mac address.  Do obfuscation
	mac = $1
	# Try to look up internally first
	if (! (mac in mapping)) {
		# Compute the hash for the first time
		cmd = "hashmac "$1 salt
		cmd | getline result
		close(cmd)
		mapping[mac] = result
	}
	printf "%s%s", sep, mapping[mac]
	sep = "\""
}

!/[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]:[[:xdigit:]][[:xdigit:]]/ {
	printf "%s%s", sep, $0
	sep = "\""
}

END {
}
