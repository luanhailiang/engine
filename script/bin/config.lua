--client link port bind start 
gate_client_port 	= 6000

--heart beat time
heart_beat_time		= 1000000

--Network card name 
eth_name			= "eth0"

--worker bind port start
worker_start_pub 	= "tcp://127.0.0.1:7%03d"

--gate worker router address
gate_work_router	= "tcp://127.0.0.1:5005"

--master gate push pull address
master_gate_rep 	= "tcp://127.0.0.1:5001"
--master gate publish sub address
master_gate_pub 	= "tcp://127.0.0.1:5002"

--master worker push pull address
master_work_rep 	= "tcp://127.0.0.1:5003"
--master worker publish sub address
master_work_pub 	= "tcp://127.0.0.1:5004"

--master lua configure
master_lua_file		= "./master/main.lua"
master_lua_path		= "./master/?.lua"
master_lua_cpath	= "./bin/?.so"

master_lua_beat		= "heart_beat"
master_lua_gate		= "gate_msg"
master_lua_worker	= "worker_msg"

--worker lua configure
worker_lua_file		= "./worker/main.lua"
worker_lua_path		= "./worker/?.lua"
worker_lua_cpath	= "./bin/?.so"

worker_lua_beat		= "heart_beat"
worker_lua_master	= "master_msg"
worker_lua_player	= "player_msg"
worker_lua_worker	= "worker_msg"

log_directory		= "./log/"