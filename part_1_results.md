.
orders-100K.txt.gz
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/s/trades'], shell=True, cwd='/home/vsts/work/1/part_1_rust_solution')
[0.0026596009997774672, 0.0017617999997128209]
output file does not exist: /home/vsts/work/1/s/trades
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/s/trades'], shell=True, cwd='/home/vsts/work/1/part_1_cpp_solution')
[0.0016192999996746948, 0.0015802009997969435]
output file does not exist: /home/vsts/work/1/s/trades
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/s/trades'], shell=True, cwd='/home/vsts/work/1/part_1_fsharp_solution')
[0.0015558009999949718, 0.0015892009996605339]
output file does not exist: /home/vsts/work/1/s/trades
subprocess.run(['./runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/s/trades'], shell=True, cwd='/home/vsts/work/1/part_1_csharp_solution')
[0.0016366009999728703, 0.0015823009998712223]
output file does not exist: /home/vsts/work/1/s/trades
{'orders-100K.txt': {'part_1_rust_solution': 0.0017617999997128209, 'part_1_cpp_solution': 0.0015802009997969435, 'part_1_fsharp_solution': 0.0015558009999949718, 'part_1_csharp_solution': 0.0015823009998712223}}
