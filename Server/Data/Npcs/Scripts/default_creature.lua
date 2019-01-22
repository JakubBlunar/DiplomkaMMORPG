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


registerEvent(18, function()
	rand = math.random()
	if rand < 0.7 then
		sendNpcToRandomPositionInLocation(30)
	else
		makeNpcStay(math.random(10, 20))
	end	
end
)
