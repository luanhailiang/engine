#!/bin/bash


start()
{
	gnome-terminal -t MASTER -e './bin/masterd -c./bin/config.lua -b0'
	sleep 0.5
	gnome-terminal -t GATE -e   './bin/gated   -c./bin/config.lua -b0'
	sleep 0.5
	gnome-terminal -t WORKER1 -e './bin/workerd -c./bin/config.lua -b1 -w1'
	gnome-terminal -t WORKER2 -e './bin/workerd -c./bin/config.lua -b1 -w2'
}

stop()
{
	(ps aux | grep '/bin/workerd' | awk '{print $2}' | xargs kill -10 &>/dev/null) || echo 'kill workerd'
	(ps aux | grep '/bin/gated'   | awk '{print $2}' | xargs kill -10 &>/dev/null) || echo 'kill gated'
	(ps aux | grep '/bin/masterd' | awk '{print $2}' | xargs kill -10 &>/dev/null) || echo 'kill masterd'
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