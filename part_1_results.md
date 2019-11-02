-rw-r--r-- 1 vsts docker 92412 Nov  2 05:57 /home/vsts/work/1/s/part_1_cpp_solution/trades
-rw-r--r-- 1 vsts docker 92412 Nov  2 05:58 /home/vsts/work/1/s/part_1_cpp_solution/trades
.
orders-100K.txt.gz
subprocess.run(['/home/vsts/work/1/s/part_1_cpp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/s/part_1_cpp_solution/trades && ls -l /home/vsts/work/1/s/part_1_cpp_solution/trades'], shell=True, cwd='/home/vsts/work/1/s/part_1_cpp_solution')
[26.653930434999893, 26.149473474999468]
output file does not exist: /home/vsts/work/1/s/part_1_cpp_solution/trades
{'orders-100K.txt': {'part_1_cpp_solution': 26.149473474999468}}
