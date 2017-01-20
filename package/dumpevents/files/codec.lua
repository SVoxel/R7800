-- event IDs,  version 1 unless otherwise specified
local event_id_open = 1
local event_id_close = 2
local event_id_milestone = 3
local event_id_classification = 4
local event_id_feature = 5
local event_id_con_open = 6
local event_id_con_close = 7
-- 8 much?
local event_id_oversub_update = 9
local event_id_oversub_create = 10

-- open, v1
-- format: "1,time,uuid,flow_id,mac,policy_id"
local function open_decode_from_csv(event_csv)
	local s,e,pos
	local event = { event = "open" }

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_open then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- uuid
	s,e = string.find(event_csv,',',pos)
	event.uuid = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- flow_id
	s,e = string.find(event_csv,',',pos)
	event.flow_id = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- mac
	s,e = string.find(event_csv,',',pos)
	event.mac = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- begin details
	event.details = {}
	-- policy_id
	event.details.policy_id = string.sub(event_csv,pos)

	return event
end
-- close, v1
-- format: "2,time,uuid,flow_id,mac,tx_bytes,rx_bytes"
local function close_decode_from_csv(event_csv)
	local s,e,pos
	local event = { event = "close" }

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_close then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- uuid
	s,e = string.find(event_csv,',',pos)
	event.uuid = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- flow_id
	s,e = string.find(event_csv,',',pos)
	event.flow_id = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- mac
	s,e = string.find(event_csv,',',pos)
	event.mac = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- begin details
	event.details = {}
	-- tx_bytes
	s,e = string.find(event_csv,',',pos)
	event.details.tx_bytes = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_bytes
	event.details.rx_bytes = string.sub(event_csv,pos)

	return event
end
-- milestone, v1
-- format: "3,time,uuid,flow_id,tx_bytes,tx_packets,tx_qlen,tx_requeues,tx_overlimits,tx_drops,tx_backlog,rx_bytes,rx_packets,rx_qlen,rx_requeues,rx_overlimits,rx_drops,rx_backlog"
local function milestone_decode_from_csv(event_csv)
	local s,e,pos
	local event = { event = "milestone" }

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_milestone then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- uuid
	s,e = string.find(event_csv,',',pos)
	event.uuid = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- flow_id
	s,e = string.find(event_csv,',',pos)
	event.flow_id = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- begin details
	event.details = {}
	-- tx_bytes
	s,e = string.find(event_csv,',',pos)
	event.details.tx_bytes = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- tx_packets
	s,e = string.find(event_csv,',',pos)
	event.details.tx_packets = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- tx_qlen
	s,e = string.find(event_csv,',',pos)
	event.details.tx_qlen = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- tx_requeues
	s,e = string.find(event_csv,',',pos)
	event.details.tx_requeues = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- tx_overlimits
	s,e = string.find(event_csv,',',pos)
	event.details.tx_overlimits = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- tx_drops
	s,e = string.find(event_csv,',',pos)
	event.details.tx_drops = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- tx_backlog
	s,e = string.find(event_csv,',',pos)
	event.details.tx_backlog = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_bytes
	s,e = string.find(event_csv,',',pos)
	event.details.rx_bytes = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_packets
	s,e = string.find(event_csv,',',pos)
	event.details.rx_packets = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_qlen
	s,e = string.find(event_csv,',',pos)
	event.details.rx_qlen = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_requeues
	s,e = string.find(event_csv,',',pos)
	event.details.rx_requeues = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_overlimits
	s,e = string.find(event_csv,',',pos)
	event.details.rx_overlimits = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_drops
	s,e = string.find(event_csv,',',pos)
	event.details.rx_drops = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rx_backlog
	event.details.rx_backlog = string.sub(event_csv,pos)

	return event
end

-- decode a 5-tuple from the csv, where the first character of the
-- 5-tuple starts at pos
-- returns: a 2-tuple of (5-tuple-string, end_of_tuple_pos)
local function tuple_decode_from_csv(event_csv,pos)
	local s,e
	local tstart,tend
	-- tstart points to proto
	tstart = pos
	s,e = string.find(event_csv,',',tstart)
	-- src_ip is e + 1
	s,e = string.find(event_csv,',',e + 1)
	-- sport is e + 1
	s,e = string.find(event_csv,',',e + 1)
	-- dest_ip is e + 1
	s,e = string.find(event_csv,',',e + 1)
	-- dport is e + 1
	s,e = string.find(event_csv,',',e + 1)
	if e then
		tend = e - 1
	else
		tend = nil
	end
	return string.sub(event_csv,tstart,tend) , e
end

-- classification, v1
-- format: "4,time,5-tuple,class,classifier"
local function classification_decode_from_csv(event_csv)
	local s,e,pos
	local event = { event = "classification" }

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_classification then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- 5-tuple
	event['5-tuple'], e = tuple_decode_from_csv(event_csv,pos)
	pos = e + 1
	-- class
	s,e = string.find(event_csv,',',pos)
	event.class = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- classifier
	event.classifier = string.sub(event_csv,pos)

	return event
end
-- feature discovery, v1
-- format: "5,time,5-tuple,data"
local function feature_decode_from_csv(event_csv)
	local s,e,pos
	local event = { event = "feature" }

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_feature then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- 5-tuple
	event['5-tuple'], e = tuple_decode_from_csv(event_csv,pos)
	pos = e + 1
	-- data
	event.data = string.sub(event_csv,pos)

	return event
end
-- con-open, v1
-- format: "6,time,5-tuple"
local function con_open_decode_from_csv(event_csv)
	local s,e,pos
	local event = { event = "con-open" }

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_con_open then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- 5-tuple
	event['5-tuple'], e = tuple_decode_from_csv(event_csv,pos)

	return event
end
-- con-close, v1
-- format: "7,time,5-tuple,orig-bytes,orig-packets,reply-bytes,reply-packets"
local function con_close_decode_from_csv(event_csv)
	local s,e,pos
	local event = { event = "con-close" }

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_con_close then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- 5-tuple
	event['5-tuple'], e = tuple_decode_from_csv(event_csv,pos)
	pos = e + 1
	-- begin details
	event.details = {}
	-- orig-bytes
	s,e = string.find(event_csv,',',pos)
	event.details['orig-bytes'] = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- orig-packets
	s,e = string.find(event_csv,',',pos)
	event.details['orig-packets'] = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- reply-bytes
	s,e = string.find(event_csv,',',pos)
	event.details['reply-bytes'] = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- reply-packets
	event.details['reply-packets'] = string.sub(event_csv,pos)

	return event
end

-- oversub update and create, v1
-- format: "[9,10],time,uuid,flow_id,direction,policy_type,stratum,rate,delay,work"
local function oversub_decode_from_csv(event_csv)
	local s,e,pos
	local event = {}

	pos = 1
	-- event id
	s,e = string.find(event_csv,',',pos)
	local id = tonumber(string.sub(event_csv,pos,e - 1))
	if id ~= event_id_oversub_update and id ~= event_id_oversub_create then
		return nil
	end
	pos = e + 1
	-- time
	s,e = string.find(event_csv,',',pos)
	event.time = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- uuid
	s,e = string.find(event_csv,',',pos)
	event.uuid = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- flow_id
	s,e = string.find(event_csv,',',pos)
	event.flow_id = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- begin details
	event.details = {}
	-- direction
	s,e = string.find(event_csv,',',pos)
	event.details.direction = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- policy_type
	s,e = string.find(event_csv,',',pos)
	event.details.policy_type = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- stratum
	s,e = string.find(event_csv,',',pos)
	event.details.stratum = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- rate
	s,e = string.find(event_csv,',',pos)
	event.details.rate = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- delay
	s,e = string.find(event_csv,',',pos)
	event.details.delay = string.sub(event_csv,pos,e - 1)
	pos = e + 1
	-- work
	event.details.work = string.sub(event_csv,pos)

	return event
end
-- oversub update, v1
-- format: "9,time,uuid,flow_id,direction,policy_type,stratum,rate,delay,work"
local function update_decode_from_csv(event_csv)
	local event = oversub_decode_from_csv(event_csv)
	if not event then
		return nil
	end
	event.event = "update"
	return event
end
-- oversub create, v1
-- format: "10,time,uuid,flow_id,direction,policy_type,stratum,rate,delay,work"
local function create_decode_from_csv(event_csv)
	local event = oversub_decode_from_csv(event_csv)
	if not event then
		return nil
	end
	event.event = "create"
	return event
end

-- lookup table that maps decode function of event types
-- [<event_type_id>] = decode_event_type_fn
local event_decoder_fn = {
	[event_id_open] = open_decode_from_csv,
	[event_id_close] = close_decode_from_csv,
	[event_id_milestone] = milestone_decode_from_csv,
	[event_id_classification] = classification_decode_from_csv,
	[event_id_feature] = feature_decode_from_csv,
	[event_id_con_open] = con_open_decode_from_csv,
	[event_id_con_close] = con_close_decode_from_csv,
	[event_id_oversub_update] = update_decode_from_csv,
	[event_id_oversub_create] = create_decode_from_csv,
}
-- converts a single raw event entry in csv format into a lua table
local function csv_to_table(event_csv)
	if type(event_csv) ~= "string" then
		return nil
	end
	-- the first field is always the ID of the event type
	local s,e = string.find(event_csv,',',1)
	local etype = string.sub(event_csv,1,e - 1)
	local decode = event_decoder_fn[tonumber(etype)]
	if decode then
		return decode(event_csv)
	end
	return nil
end
-- converts a lua table containing a list of events into json
local function table_to_json(events)
	return cjson.encode(events)
end
