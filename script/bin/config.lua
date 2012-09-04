--client link port bind start 
gate_client_port 	= 6000

--worker bind port start
worker_start_pub 	= "tcp://127.0.0.1:7%03d"

--gate worker router address
gate_work_router	= "tcp://127.0.0.1:5005"

--master gate publish sub address
master_gate_pub 	= "tcp://127.0.0.1:5001"
--master gate push pull address
master_gate_rep 	= "tcp://127.0.0.1:5002"

--master worker publish sub address
master_work_pub 	= "tcp://127.0.0.1:5003"
--master worker push pull address
master_work_rep 	= "tcp://127.0.0.1:5004"

		   
