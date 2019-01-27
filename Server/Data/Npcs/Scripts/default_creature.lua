registerEvent(events.NPC_IS_IDLE, function()
	rand = math.random()
	if rand < 0.7 then
		sendNpcToRandomPositionInLocation(30)
	else
		makeNpcStay(math.random(10, 20))
	end
end
)

registerEvent(events.NPC_COMBAT_DECIDE, function()
	castRandomSpell()
end
)


