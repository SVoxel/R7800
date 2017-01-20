#!/usr/local/bin/gawk -f 
#
# either put in your path and call as	./checktimer.awk <input
#                              or as	gawk -f checktimer.awk <input
# checktimer.awk
#

#
# Copyright (c) 2013 Qualcomm Atheros, Inc.
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary.
#

match($0, / [\-\+][A-Za-z]:/) { 
		name = substr($0,RSTART+1,RLENGTH-1);
		counts[name]++			#count of matches of this type
		hits[name,substr($NF,6)]++	#counts of hits on this type,pointer
		places[substr($NF,6)]++		#count of hits on a pointer
		print $0, " matched:", name, counts[name], hits[name,substr($NF,6)]
	}

END {
	n = asorti(places, temp)
	print "Displaying:",n,"sorted by address"
	for (j=1; j<n; j++) {
		for (i in counts) if ( i SUBSEP temp[j] in hits) {
			printf("%s,%s = %d ", i, temp[j], hits[i SUBSEP temp[j]])
			if (i ~ /+.:/) {
				mate = "-" substr(i, 2)
				if (mate in counts) {
					#print " mates",i,mate
					if (hits[i SUBSEP temp[j]] != hits[mate SUBSEP temp[j]])
						printf(" ***** mismatched create/delete ")
				}
			}
		}
		print "."
	}
	print "plus counts should match minus counts, or there may be a leak"
	for (i in counts) print i,counts[i]

}
