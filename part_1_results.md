.
orders-100K.txt.gz
subprocess.run(['/home/vsts/work/1/part_1_rust_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_rust_solution/trades && ls -l /home/vsts/work/1/part_1_rust_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_rust_solution')
[0.002357194999603962, 0.0020813950004594517]
output file does not exist: /home/vsts/work/1/part_1_rust_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_cpp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_cpp_solution/trades && ls -l /home/vsts/work/1/part_1_cpp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_cpp_solution')
[0.0023470950000046287, 0.0025142940003206604]
output file does not exist: /home/vsts/work/1/part_1_cpp_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_fsharp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_fsharp_solution/trades && ls -l /home/vsts/work/1/part_1_fsharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_fsharp_solution')
[0.002245194999886735, 0.0023759949999657692]
output file does not exist: /home/vsts/work/1/part_1_fsharp_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_csharp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_csharp_solution/trades && ls -l /home/vsts/work/1/part_1_csharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_csharp_solution')
[0.0023801940005796496, 0.0024013939992073574]
output file does not exist: /home/vsts/work/1/part_1_csharp_solution/trades
{'orders-100K.txt': {'part_1_rust_solution': 0.0020813950004594517, 'part_1_cpp_solution': 0.0023470950000046287, 'part_1_fsharp_solution': 0.002245194999886735, 'part_1_csharp_solution': 0.0023801940005796496}}
