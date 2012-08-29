#!/bin/bash


start()
{
	./bin/masterd -c./bin/config.lua -b0 > log/MASTER 2>&1 &
	
	./bin/gated -c./bin/config.lua -b0 -i1 > log/GATE1 2>&1 &
	./bin/gated -c./bin/config.lua -b0 -i2 > log/GATE2 2>&1 &
	
	./bin/workerd -c./bin/config.lua -b1 -i1 > log/WORKER1 2>&1 &
	./bin/workerd -c./bin/config.lua -b1 -i2 > log/WORKER2 2>&1 &
	./bin/workerd -c./bin/config.lua -b1 -i3 > log/WORKER3 2>&1 &
	./bin/workerd -c./bin/config.lua -b1 -i4 > log/WORKER4 2>&1 &
	./bin/workerd -c./bin/config.lua -b1 -i5 > log/WORKER5 2>&1 &
	./bin/workerd -c./bin/config.lua -b1 -i6 > log/WORKER6 2>&1 &
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