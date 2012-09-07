

function master_msg(msg)
	message_player(1,msg)
	print("Worker from master:"..msg)
end


function worker_msg(msg)
	print("Worker from worker:"..msg)
end

function player_msg(msg)
	print("Worker from player:"..msg)
	return msg
end