#!/bin/sh

# this script pulls a list of keys from a redis database and stores the
# values into a save file in redis-cli command format.  the save file can
# then be restored simply by running it.

# path to save file
PERSIST_FILE="${2:-/usr/lib/sbsaved/sb.persist.redis}"

# space-delimited list of keys to be saved into PERSIST_FILE
PERSIST_KEYS="settings:*"

# $1: key
persist_string() {
	local key=$1
	echo "redis-cli SET ${key} $(redis-cli GET ${key})"
}

# $1: key
persist_hash() {
	local hash=$1
	for key in $(redis-cli hkeys ${hash}); do
		local value=$(redis-cli HGET ${hash} ${key})
		echo "redis-cli HSET ${hash} ${key} ${value}"
	done
}

# $1: key
persist_list() {
	local key=$1
	for item in $(redis-cli lrange ${key} 0 -1); do
		echo "redis-cli RPUSH ${key} ${item}"
	done
}

# $1: key
persist_set() {
	local key=$1
	for item in $(redis-cli smembers ${key}); do
		echo "redis-cli SADD ${key} ${item}"
	done
}

# $1: key
persist_zset() {
	local key=$1
	for member in $(redis-cli zrange ${key} 0 -1); do
		local score=$(redis-cli zscore ${key} ${member})
		echo "redis-cli ZADD ${key} ${score} ${member}"
	done
}

persist() {
	redis-cli info >/dev/null 2>&1
	if [ "$?" != "0" ]; then
		echo "refusing to persist: redis is not running"
		return
	fi

	# note: this function is implemented this way because the output
	# of redis-cli DUMP results in error when given to redis-cli RESTORE.
	# for example, the following command:
	# $ redis-cli DUMP "mykey" | redis-cli -x RESTORE "mynewkey" 0
	# results in this error:
	# (error) ERR DUMP payload version or checksum are wrong
	tmp=$(mktemp -p /tmp)
	for key in ${PERSIST_KEYS}; do
		for subkey in $(redis-cli keys ${key}); do
			echo "persisting key ${subkey}"
			local t=$(redis-cli type ${subkey})
			case $t in
				hash)
					persist_hash ${subkey} >> ${tmp}
					;;
				string)
					persist_string ${subkey} >> ${tmp}
					;;
				list)
					persist_list ${subkey} >> ${tmp}
					;;
				set)
					persist_set ${subkey} >> ${tmp}
					;;
				zset)
					persist_zset ${subkey} >> ${tmp}
					;;
			esac
		done
	done
	mv ${tmp} ${PERSIST_FILE}
	rm -f ${tmp}
}

restore() {
	sh ${PERSIST_FILE}
}

case "${1}" in
	persist)
		persist
		;;
	restore)
		restore
		;;
	*)
		echo "usage: $(basename $0) <persist|restore> [persistfile]"
		;;
esac
