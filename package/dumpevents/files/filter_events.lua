local t = redis.call('lrange', KEYS[1], 0, -1)
local types = {}
for index,value in ipairs(ARGV) do
	types[value] = true
end
local ret = {}
for index,value in ipairs(t) do
	local json = cjson.decode(value)
	local etype = json['event']
	if types[etype] then
		ret[#ret+1] = value
	end
end
return ret
