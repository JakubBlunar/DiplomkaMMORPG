events = {}

function errorHandler (message)
	return "ERROR: " .. message
end

printf = function(s,...)
   return io.write(s:format(...))
end

function tableHasKey(table,key)
	return table[key] ~= nil
end

function registerEvent(id, handler)
	events[id] = handler
end

function handleEvent(eventId)
	if (tableHasKey(events, eventId))
	then
		events[eventId]()
	end
end