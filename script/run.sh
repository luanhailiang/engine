#!/bin/bash


start()
{
	./bin/masterd -c./bin/config.lua -b0 > log/MASTER 2>&1 &
	
	./bin/gated -c./bin/config.lua -b0 > log/GATE 2>&1 &
	
	./bin/workerd -c./bin/config.lua -b1 > log/WORKER1 2>&1 &
	./bin/workerd -c./bin/config.lua -b1 > log/WORKER2 2>&1 &
	./bin/workerd -c./bin/config.lua -b1 > log/WORKER3 2>&1 &
}

stop()
{
	(ps aux | grep '/bin/gated' | awk '{print $2}' | xargs kill -10 &>/dev/null) || echo 'kill gated'
	(ps aux | grep '/bin/workd' | awk '{print $2}' | xargs kill -10 &>/dev/null) || echo 'kill workd'
}

case "$1" in		
	start)		
		start;;						
	restart)
		stop						
		start;;										
	stop)										
		stop;;												
	*)		
		echo '["start"|"restart"|"stop"]'															    		
		exit 1															
esac