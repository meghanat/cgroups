
util=0.2

counter=1


# fg_util=`echo "1 - $util" | bc`

# echo $fg_util



while [ $counter -le 4 ]
do
	echo Util is $util

	
	bg_period=` python -c "print ( 1 - $util ) / $util * 400.0 " `
	echo Period is $bg_period

	sudo gcc -o bg_job	bg_job.c
	sudo gcc -o loader bg_controller.c
	sudo ./loader $util $bg_period &
	sudo ./loader $util $bg_period &
	sudo ./loader $util $bg_period &
	sudo ./loader $util $bg_period &
	gcc job.c -o emulate_job
	rm wl_resp.txt
	touch wl_resp.txt
	gcc controller.c -o controller
	sudo ./controller 0 &

	echo $!

	wait $!

	sudo killall controller
	sudo killall emulate_job

	sudo killall loader
	sudo killall bg_job
	mv wl_resp.txt $util_$bg_period.txt
	
	
	util=`echo " $util + 0.2 " | bc`

	counter=$(( $counter + 1 ))
done