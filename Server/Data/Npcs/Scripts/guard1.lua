printf = function(s,...)
   return io.write(s:format(...))
end

function tableHasKey(table,key)
	return table[key] ~= nil
end

events = {}
resultEvents = {}

--[Npc is idle]--
events[18] = function()
	rand = math.random()
	if rand < 0.5 then
		resultEvents[1] = {
			maxDuration = 30
		}
	else 
		resultEvents[0] = {
			duration = math.random(10, 20)
		}
	end	
end

if (tableHasKey(events, event.id))
then
	events[event.id]()
end