.
orders-100K.txt.gz
subprocess.run(['/home/vsts/work/1/part_1_rust_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_rust_solution/trades && ls -l /home/vsts/work/1/part_1_rust_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_rust_solution')
[0.002135913000074652, 0.0024575140000706597]
output file does not exist: /home/vsts/work/1/part_1_rust_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_fsharp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_fsharp_solution/trades && ls -l /home/vsts/work/1/part_1_fsharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_fsharp_solution')
[0.0021190120000937895, 0.0020476119998420472]
output file does not exist: /home/vsts/work/1/part_1_fsharp_solution/trades
subprocess.run(['/home/vsts/work/1/part_1_csharp_solution/runner < /home/vsts/work/1/s/orders-100K.txt > /home/vsts/work/1/part_1_csharp_solution/trades && ls -l /home/vsts/work/1/part_1_csharp_solution/trades'], shell=True, cwd='/home/vsts/work/1/part_1_csharp_solution')
[0.0020741119997182977, 0.002120711999850755]
output file does not exist: /home/vsts/work/1/part_1_csharp_solution/trades
{'orders-100K.txt': {'part_1_rust_solution': 0.002135913000074652, 'part_1_fsharp_solution': 0.0020476119998420472, 'part_1_csharp_solution': 0.0020741119997182977}}
