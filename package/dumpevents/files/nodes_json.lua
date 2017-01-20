local interested_fields = {
	'type',
	'os_type',
	'http_clients',
	'detection_finished',
	'sigs:http:known',
	'sigs:http:unknown',
	'sigs:syn:known',
	'sigs:syn:unknown',
	'uidata'
}

local function load_from_redis(key)
	local datatype = redis.call('type', key)['ok']
	local ret
	if datatype == 'none' then
		ret = nil
	elseif datatype == 'string' then
		ret = redis.call('get', key)
	elseif datatype == 'set' then
		ret = redis.call('smembers', key)
	elseif datatype == 'hash' then
		ret = redis.call('hgetall', key)
	elseif datatype == 'list' then
		ret = redis.call('lrange', key, 0, -1)
	else
		ret = 'error reading ' .. key .. ' from redis'
	end
	return ret
end

local devices_table = {}
local json_root = {['devices']=devices_table}
local macaddrs = redis.call('smembers', 'nodedb:nodeset')
for i,mac in ipairs(macaddrs) do
	local device = {}
	devices_table[mac] = device
	for j, field in ipairs(interested_fields) do
		device[field]=load_from_redis('nodedb:mac:' .. mac .. ':' .. field)
	end
end
return cjson.encode(json_root)
