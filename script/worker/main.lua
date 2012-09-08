

function master_msg(msg)
	message_player(1,msg)
	message_all_worker(msg)
	print("Worker from master:"..msg)
end


function worker_msg(id, msg)
	print("Worker from worker:"..id..":"..msg)
end

function player_msg(id, msg)
	print("Worker from player:"..id..":"..msg)
	return msg
end