.
orders-100K.txt.gz
subprocess.run(['/home/vsts/work/1/part_1_rust_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_rust_solution/trades && ls -l /home/vsts/work/1/part_1_rust_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_rust_solution')
[0.002004683999984991, 0.002082783999867388]
output file does not exist: /home/vsts/work/1/part_1_rust_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_cpp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_cpp_solution/trades && ls -l /home/vsts/work/1/part_1_cpp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_cpp_solution')
[0.001972583999759081, 0.0015987880001375743]
output file does not exist: /home/vsts/work/1/part_1_cpp_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_fsharp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_fsharp_solution/trades && ls -l /home/vsts/work/1/part_1_fsharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_fsharp_solution')
[0.0019855839996125724, 0.0017037870002241107]
output file does not exist: /home/vsts/work/1/part_1_fsharp_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_csharp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_csharp_solution/trades && ls -l /home/vsts/work/1/part_1_csharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_csharp_solution')
[0.00184618599996611, 0.0014404890002879256]
output file does not exist: /home/vsts/work/1/part_1_csharp_solution/trades
{'orders-100K.txt': {'part_1_rust_solution': 0.002004683999984991, 'part_1_cpp_solution': 0.0015987880001375743, 'part_1_fsharp_solution': 0.0017037870002241107, 'part_1_csharp_solution': 0.0014404890002879256}}
