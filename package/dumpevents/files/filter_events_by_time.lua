local t = redis.call('lrange', KEYS[1], 0, -1)
local begintime = tonumber(ARGV[1])
local endtime = tonumber(ARGV[2])
local ret = {}
for index,value in ipairs(t) do
	local json = cjson.decode(value)
	local timestamp = tonumber(json['time'])
	if timestamp >= begintime and timestamp <= endtime then
		ret[#ret+1] = value
	end
end
return ret
