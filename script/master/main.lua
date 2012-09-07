
function gate_msg(msg)
	print("Master from gate:"..msg)
	message_all_worker(msg)
	return msg
end

function worker_msg(msg)
	print("Master from worker:"..msg)
	return msg
end