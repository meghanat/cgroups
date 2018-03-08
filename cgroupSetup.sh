cgcreate -g cpu:group1

cgset -r cpu.shares=1195 group1

cgcreate -g cpuset:job0
cgcreate -g cpuset:job1
cgcreate -g cpuset:job2
cgcreate -g cpuset:job3

cgset -r cpuset.mems=0 job0
cgset -r cpuset.mems=0 job1
cgset -r cpuset.mems=0 job2
cgset -r cpuset.mems=0 job3

cgset -r cpuset.cpus=0 job0
cgset -r cpuset.cpus=1 job1
cgset -r cpuset.cpus=2 job2
cgset -r cpuset.cpus=3 job3

