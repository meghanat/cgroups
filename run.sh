gcc job.c -o emulate_job
rm wl_resp.txt
touch wl_resp.txt
gcc controller.c -o controller
sudo ./controller 0 &
