eventHandlers = {}
events = {
	ERR = 1,
	WRONGPACKETTYPE = 2,
	POSITION = 3,
	MOVEMENT = 4,
	LATENCY = 5,
	LOGINREQUEST = 6,
	LOGINRESPONSE = 7,
	CHARACTER_CHOOSE = 8,
	CHARACTER_CHOOSE_RESPONSE = 9,
	CHARACTER_MAP_JOIN = 10,
	CHARACTER_MAP_LEAVE = 11,
	CHARACTER_LOGOUT = 12,
	NPC_MOVEMENT_CHANGE = 13,
	NPCS_MOVEMENT_CHANGE = 14,
	NPC_CHANGES = 15,
	NPC_STATUS_CHANGED = 16,
	ATTRIBUTES_CHANGED = 17,
	NPC_IS_IDLE = 18,
	ATTRIBUTES_BONUSES_CHANGES = 19,
	PLAYER_START_CAST_SPELL = 20,
	SPELL_CAST_RESULT= 21,
	SEND_MESSAGE = 22,
	INCREASE_CHARACTER_ATTRIBUTE = 23,
	NPC_COMBAT_DECIDE = 24
}

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
	eventHandlers[id] = handler
end

function handleEvent(eventId)
	if (tableHasKey(eventHandlers, eventId))
	then
		eventHandlers[eventId]()
	end
end

