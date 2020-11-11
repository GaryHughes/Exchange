#!/usr/bin/python3

import sys
import os
import timeit
import gzip
import shutil
import glob
import datetime
import statistics

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

for input in input_files:
    input = uncompress(input)
    order_count = line_count(input)
    results[order_count] = []
    for solution in os.listdir(directory):
        if not os.path.isdir(os.path.join(directory, solution)):
            continue
        if not solution.startswith(prefix):
            continue
        runner = os.path.relpath(os.path.join(directory, solution, 'runner'))
        if not os.path.exists(runner):
            continue
        input_file = os.path.realpath(input)
        working_directory = os.path.realpath(os.path.join(directory, solution))
        output_file = os.path.realpath(os.path.join(working_directory, 'trades'))
        for solution_file in glob.glob(os.path.join(working_directory, '*')):
            os.chmod(solution_file, 0o755)
        command = "subprocess.run(['./runner < {} > {}'], shell=True, cwd='{}')".format(input_file, output_file, working_directory)
        try:
            # TODO - support an exlusion file so we don't hard code this.
            if solution.find('python') >= 0 or solution.find('swift') >= 0:
                # These solutions are too slow for the big file and relatively slow on
                # the small file. Because the comparison with other solutions isn't as important
                # we don't care so much about the validity of the results.
                if order_count > 100000:
                    continue
                actual_iterations = 1
            else:
                actual_iterations = iterations
            result = timeit.repeat(stmt = command, setup = "import subprocess", number = 1, repeat = actual_iterations)
            if not os.path.exists(output_file):
                continue
            results[order_count].append((solution, statistics.median(result), line_count(output_file)))
        except Exception as ex:
            print(str(ex))
            continue

def human_format(num):
    magnitude = 0
    while abs(num) >= 1000:
        magnitude += 1
        num /= 1000
    return '{}{}'.format(int(num), ['', 'K', 'M', 'G', 'T', 'P'][magnitude])

languages = {
    "cpp"       : "C++",
    "csharp"    : "C#",
    "fsharp"    : "F#",
    "rust"      : "Rust",
    "go"        : "Go",
    "swift"     : "Swift",
    "java"      : "Java",
    "python"    : "Python",
    "cython"    : "Cython",
    "c"         : "C"
}

def language(solution):
    name = solution.split('_')[2]
    try:
        return languages[name]
    except KeyError:
        return name

for order_count, solutions in results.items():
    solutions.sort(key=lambda solution: solution[1])
    print('||{} orders|trades|'.format(human_format(order_count)))
    print('-|:-:|:-:|')
    for solution in solutions:
        time = str(datetime.timedelta(seconds=solution[1]))
        print('|{}|{}|{}|'.format(language(solution[0]), time, solution[2]))
    print("\n")








