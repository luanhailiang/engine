#!/bin/bash


start()
{
	./bin/gated ./bin/config.lua > log/gated 2>&1 &
	echo 'start gated'
	./bin/workd ./bin/config.lua > log/workd 2>&1 &
	echo 'start workd'
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