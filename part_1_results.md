.
orders-100K.txt.gz
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_rust_solution/trades && ls -l /home/vsts/work/1/part_1_rust_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_rust_solution')
[0.00365890600005514, 0.0022027029999662773]
output file does not exist: /home/vsts/work/1/part_1_rust_solution/trades
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_cpp_solution/trades && ls -l /home/vsts/work/1/part_1_cpp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_cpp_solution')
[0.0015433030002895975, 0.0015748020000501128]
output file does not exist: /home/vsts/work/1/part_1_cpp_solution/trades
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_fsharp_solution/trades && ls -l /home/vsts/work/1/part_1_fsharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_fsharp_solution')
[0.001521102999959112, 0.0015554020001218305]
output file does not exist: /home/vsts/work/1/part_1_fsharp_solution/trades
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_csharp_solution/trades && ls -l /home/vsts/work/1/part_1_csharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_csharp_solution')
[0.0016099029999168124, 0.001799202999791305]
output file does not exist: /home/vsts/work/1/part_1_csharp_solution/trades
{'orders-100K.txt': {'part_1_rust_solution': 0.0022027029999662773, 'part_1_cpp_solution': 0.0015433030002895975, 'part_1_fsharp_solution': 0.001521102999959112, 'part_1_csharp_solution': 0.0016099029999168124}}
