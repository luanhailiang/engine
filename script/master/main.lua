
function gate_msg(id,msg)
	print("Master from gate:"..id..":"..msg)
	message_all_worker(msg)
	return msg
end

function worker_msg(id,msg)
	print("Master from worker:"..id..":"..msg)
	return msg
end