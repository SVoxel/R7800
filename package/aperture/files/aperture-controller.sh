#!/bin/sh

# Copyright (c) 2014 Qualcomm Atheros, Inc.
#
# All Rights Reserved.
# Qualcomm Atheros Confidential and Proprietary

# output current bandwidth usage of the pipe - second word on the iptables output line, first occurrence only
current_pipe_bw() {
	iptables -v -x -L FORWARD | sed '/ACCEPT .* RELATED,ESTABLISHED/ !d;s/^ *[^ ]* *\([^ ]*\) .*/\1/;q'
}

# check if anyone else is using the pipe
# tests true (return 0) if we may proceed with our test
pipe_is_free() {
	BW1=$(current_pipe_bw)
	sleep 1
	BW2=$(current_pipe_bw)
	if [ $((${BW2}-${BW1})) -gt $THRESHOLD ]; then
		return 1
	fi
	return 0
}

# check whether automatic measurement is enabled
# tests true (return 0) if we are allowed to run
# integration change: always run, we get killed when auto is turned off
auto_is_enabled() {
	#grep enable_auto $CONFIG_FILE | grep 1 >/dev/null && return 0
	[ "$ENABLE_APERTURE" = "no" ] && return 1
	return 0
}

# call aperture with variable arguments
call_binary() {
        $BINDIR/$BINARY_ONERUN $@                                 \
                               $LOGDEST                           \
                               --log-level "$LOGLEVEL"            \
                               --run-dir "$RUNDIR"                \
                               --pid-file "$PIDFILE_ONERUN"       \
                               --lock-file "$LOCKFILE"            \
                               --bwcd-socket "$BWCD_SOCKET"       \
                               --local-port "$SRC_PORT"           \
                               --history-file "$HISTORY_FILE"     \
                               --results-file "$RESULTS_FILE"     \
                               --stcache-file "$STCACHE_FILE"     \
                               --serverlist-url "$SERVERLIST_URL" \
                               --geolocate-url "$GEOLOCATE_URL"
}

# perform a single test and abort if the pipe becomes busy
run_test() {
	check_self_running manual || {
		TEST_ABORTED=1
		return
	}
	TEST_ABORTED=0
	[ -z "$APERTURE_STATE" ] && APERTURE_STATE=$(echo -e "0 0\n0\n\n")
	$LOGGER Starting test
	rm -f $RESULTS_FILE
	APERTURE_OUTPUT=ABORTED
	APERTURE_OUTPUT=$( (
		echo "$APERTURE_STATE"
		head -c 4096 /dev/urandom 2>/dev/null || {
			local bytes=0
			while [ $bytes -lt 4096 ]; do
				echo 123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF
				let bytes=bytes+64
			done
		}
		while test_is_running; do
			pipe_is_free && auto_is_enabled || {
				$LOGGER Pipe in use, aborting test
				abort_test
			}
		done
	) | call_binary --test-once)
	# we assume that, iff the results file is stdout, the bwcd pipe and all state files are stdin/out
	if [ "$RESULTS_FILE" != "--" ]; then
		[ -r $RESULTS_FILE ] && RESCODE=$(grep ^Retcode $RESULTS_FILE | cut -d ' ' -f 2) || RESCODE=-99
		return
	fi
	# when using stdin/out, the output will either be "ABORTED" or a multi-line content, as follows:
	# line 1   - new bandwidth values, in JSON format (values may be zero, don't apply)
	# line 2-6 - last test results (return code, up, down, ping, time stamp)
	# line 7-N - aperture state (history and speedtest cache)
	[ -z "$APERTURE_OUTPUT" ] && APERTURE_OUTPUT=ABORTED
	echo "$APERTURE_OUTPUT" | grep -q "ABORTED" && {
		TEST_ABORTED=1
		return
	}
	APERTURE_VALUES=$(echo "$APERTURE_OUTPUT" | head -1)
	APERTURE_STATE=$(echo "$APERTURE_OUTPUT" | tail -n +7)
	RESCODE=$(echo "$APERTURE_OUTPUT" | head -2 | tail -1 | cut -d ' ' -f 2)
	# JSON format: '{"bandwidth":{"put":{"up":"%lu","down":"%lu"}}}'
	BW_UP=$(echo "$APERTURE_VALUES" | cut -d \" -f 8)
	BW_DN=$(echo "$APERTURE_VALUES" | cut -d \" -f 12)
	[ -z "$BW_UP" -o -z "$BW_DN" ] && {
		$LOGGER "Bad parsing for global up/down values: UP=\"$BW_UP\", DN=\"$BW_DN\""
		[ -n "$DEBUGLOG_FILE" ] && echo "$APERTURE_OUTPUT" >"$DEBUGLOG_FILE"
		BW_UP=0
		BW_DN=0
	}
	[ "$RESULTS_FILE" != "$RESULTS_FILE_OUT" -a -n "$RESULTS_FILE_OUT" ] && {
		echo "Retcode $RESCODE" >"$RESULTS_FILE_OUT"
		echo "Up $BW_UP" >>"$RESULTS_FILE_OUT"
		echo "Down $BW_DN" >>"$RESULTS_FILE_OUT"
	}
	$LOGGER "Test returned $RESCODE, bw up $BW_UP / down $BW_DN"
}

# abort a running test
abort_test() {
	killall $BINARY_ONERUN
	rm -f $PIDFILE_ONERUN
}

# check whether a test is ongoing
# tests true (return 0) if aperture is currently running
test_is_running() {
	ps | grep -v grep | grep -v aperture-c | grep $BINDIR/$BINARY_ONERUN >/dev/null && return 0
	return 1
}

# force aperture to re-converge measurements before it applies any more values
reset_aperture_history() {
	call_binary --reset-history
}

# call without parameters to create a self lock, or with params to just check
check_self_running() {
	local pidfile=${PIDFILE}${1:+.$1}
	mkdir -p $pidfile.lock || exit 1
	[ -r $pidfile ] && [ -e /proc/$(cat $pidfile) ] && {
		$LOGGER Already running "$1" with PID $(cat $pidfile)
		rmdir $pidfile.lock
		return 1
	}
	[ -z "$1" ] && echo $$ >$PIDFILE
	rmdir $pidfile.lock
}

# main loop: perform a test, then wait
run_loop() {
	check_self_running || exit 1
	PIPE_FREE_MSG=1
	CURRENTRETRY=$RETRYINTERVAL
	INTERVAL_INITIAL=$INTERVAL
	reset_aperture_history
	[ "${STCACHE_FILE}" != "--" ] && rm -f "${STCACHE_FILE}"
	while auto_is_enabled; do
		if pipe_is_free; then
			PIPE_FREE_MSG=1
			run_test
			if [ $TEST_ABORTED = 0 ]; then
				[ $RESCODE -le -99 ] && CURRENTRETRY=$CONVERGEDELAY || CURRENTRETRY=$RETRYINTERVAL
				if [ $RESCODE = 0 ]; then
					applybw
					DELAY=$INTERVAL
					if [ $INTERVAL -gt $INTERVAL_CUTOFF ]; then
						$LOGGER "Sleep interval has increased past cutoff limit, job done"
						break
					fi
					INTERVAL="$(($INTERVAL * $INTERVAL_DECAYPERCENT / 100))"
				else
					DELAY=$CURRENTRETRY
					INTERVAL=$INTERVAL_INITIAL
				fi
			else
				DELAY=$CURRENTRETRY
			fi
		else
			[ $PIPE_FREE_MSG = 1 ] && $LOGGER Delaying test until pipe is free
			PIPE_FREE_MSG=0
			DELAY=$CURRENTRETRY
		fi
		if auto_is_enabled; then
			[ $DELAY -gt 1 ] && $LOGGER Sleeping $DELAY seconds...
			sleep $DELAY
		fi
	done
	$LOGGER Exiting aperture controller PID $$, auto is off
}

# run a manual test
run_manual() {
	check_self_running || exit 1
	test_is_running && abort_test
	rm -f $RESULTS_FILE
	call_binary --test-once --no-history "$@"
	[ -f $RESULTS_FILE ] && {
		grep -q "Retcode -99" $RESULTS_FILE || grep -q "Retcode 0" $RESULTS_FILE && return 0
	}
	return 1
}

applybw() {
	local results_file="${1:-$RESULTS_FILE}"
	local result_fail="[ FAIL ]"
	[ "$results_file" = "--" ] && results_file="${RESULTS_FILE_OUT}"
	if [ -f "$results_file" ]; then
		if grep -q "Retcode -99" "$results_file" || grep -q "Retcode 0" "$results_file"; then
			BW_UP=$(grep "^Up " "$results_file" | cut -d ' ' -f 2)
			BW_DN=$(grep "^Down " "$results_file" | cut -d ' ' -f 2)
			if [ "$BW_UP" != "0" -a "$BW_DN" != "0" -a -n "$BW_UP" -a -n "$BW_DN" ]; then
				local result="[  OK  ]"
				redis-cli >/dev/null -h $REDIS_HOST -p $REDIS_PORT set settings:bw:up "$BW_UP" || result="$result_fail"
				redis-cli >/dev/null -h $REDIS_HOST -p $REDIS_PORT set settings:bw:down "$BW_DN" || result="$result_fail"
				redis-cli >/dev/null -h $REDIS_HOST -p $REDIS_PORT publish bandwidth.events "$BW_UP;$BW_DN" || result="$result_fail"
				echo "$result apply bandwidth measurement, up=$BW_UP, dn=$BW_DN"
				[ "$result" = "$result_fail" ] && return 2 || return 0
			else
				echo "$result_fail applybw: up=$BW_UP, dn=$BW_DN"
				return 3
			fi
		else
			echo '$result_fail applybw: Last measurement failed. Please measure again.'
			return 4
		fi
	else
		echo '$result_fail applybw: Could not find previous measurement result. Please measure first.'
		return 5
	fi
}

cleanup_aperture() {
	$LOGGER Shutting down aperture controller
	rm -f "$PIDFILE"
	exit ${1:-0}
}

# immediate execution section
trap cleanup_aperture INT TERM
trap "$LOGGER ignoring HUP" HUP
case "$1" in
	measure)
		$LOGGER Running aperture manual measurement, PID $$
		PIDFILE=$PIDFILE.manual
		shift
		run_manual "$@"
		;;
	applybw)
		shift
		applybw "$@"
		exit $?
		;;
	*)
		$LOGGER Starting aperture controller, PID $$
		run_loop
		;;
esac
cleanup_aperture $?
