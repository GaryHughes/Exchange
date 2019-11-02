#!/usr/bin/python3

import sys
import os
import timeit
import gzip
import shutil

# The Azure pipeline for each solution is required to publish a flattened artifact named like the following examples:
#
# Part 1/cpp_solution -> part_1_cpp_solution
# Part 1/csharp_solution -> part_1_csharp_solution
#
# The Benchmark pipelines will extract these artifacts and then run this script.
#
# The artifact directory must contain a script or executable called 'runner' that will execute the program. This
# just simplifies this script so it does not need to know the vagaries of running executables for different languages.

def usage():
    print(sys.argv[0] + ' <directory> <prefix> <iterations> input...')

if len(sys.argv) < 4:
    usage()
    sys.exit(1)

directory = sys.argv[1]
prefix = sys.argv[2]
iterations = int(sys.argv[3])
input_files = sys.argv[4:]

if iterations <= 0 or len(input_files) == 0:
    usage()
    sys.exit(1)

def line_count(file):
    with open(file) as f:
        for i, _ in enumerate(f):
            pass
    return i + 1

def uncompress(input):
    if not input.endswith('.gz'):
        return input
    output = input[:-3]
    with gzip.open(input, 'rb') as f_in:
        with open(output, 'wb') as f_out:
            shutil.copyfileobj(f_in, f_out)
    return output

results = {}

print(os.path.curdir)

for input in input_files:
    print(input)
    input = uncompress(input)
    order_count = line_count(input)
    results[input] = {}
    for solution in os.listdir(directory):
        print(solution)
        if not os.path.isdir(solution):
            continue
        if not solution.startswith(prefix):
            continue
        runner = os.path.join(solution, 'runner')
        if not os.path.exists(runner):
            print('{0} does not contain a runner script'.format(solution))
            continue
        command = "subprocess.run(['./runner < ../{} > /dev/null'], shell=True, cwd='{}')".format(input, os.path.realpath(solution))
        result = timeit.repeat(stmt = command, setup = "import subprocess", number = 1, repeat = iterations)
        print(result)
        results[input][solution] = min(result)

print(results)

# {'orders-100K.txt': {'part_1_csharp_solution': 0.44683831100000004, 'part_1_cpp_solution': 16.479982948, 'part_1_rust_solution': 2.1821637549999977}}

# |     | 100K         |
# |-----|:------------:|
# |C++  | 00:00:15.122 |
# |Go   | 00:00:28.458 |
# |F#   | 00:00:09.994 |
# |C#   | 00:00:00:479 |
# |rust | 00:00:02:260 |









