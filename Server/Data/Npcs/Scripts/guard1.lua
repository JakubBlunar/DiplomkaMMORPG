
registerEvent(events.NPC_IS_IDLE, function()
	if(isInSpawnPosition()) 
	then
		makeNpcStay(999)
	else 
		goToSpawnPosition(999)
	end
end
)

registerEvent(events.NPC_COMBAT_DECIDE, function()
	castRandomSpell()
end
)
