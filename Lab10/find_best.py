#!/usr/bin/env python3

import subprocess

# Set this to true if you want to
# see each iteration
from typing import List

import math

VERBOSE_MODE = False
BI_LIMIT = 100


def build_with_flags(flags: List[str]):
    subprocess.check_output(["make", "clean"])

    if VERBOSE_MODE:
        flags.append("-DVERBOSE")

    subprocess.check_output(["make all VERBOSE=ON EXTRA_FLAGS='%s'" % " ".join(flags)], shell=True)


def get_output_line(output: bytes, start: str) -> str:
    for line in output.decode("utf-8").split("\n"):
        line: str
        line = line.strip()
        if line.startswith(start):
            return line

    assert False, "Error line starting with %s not found!: %s" % (start, output)


def get_qn_error(qn: int) -> float:
    build_with_flags([f"-DQN={qn}", f"-DBI_LIMIT={BI_LIMIT}"])

    # Run the mem test to avoid timing
    if VERBOSE_MODE:
        output = subprocess.run(["./intTest_mem"], stderr=subprocess.PIPE)
    else:
        output = subprocess.run(["./intTest_mem"], stderr=subprocess.PIPE, stdout=subprocess.PIPE)

    if output.returncode != 0:
        return math.nan

    line = get_output_line(output.stderr, "Qn ")
    try:
        split = line.split(" ")
        qn_val = int(split[1])
        error = float(split[3])
    except ValueError:
        raise ValueError("Failed to parse error on line %s" % line)

    assert qn_val == qn, "Expected Qn %d, got %d" % (qn, qn_val)
    return error


def find_best_qn() -> int:
    upper_bound = 35
    lower_bound = 15

    best_guess = -1
    best_guess_error = 0.0

    non_error_qn = []

    # Search for best error
    i = lower_bound
    while i < upper_bound + 1:
        current_error = get_qn_error(i)
        if math.isnan(current_error):
            current_error = "err"
        else:
            non_error_qn.append((i, current_error))
            if best_guess < 0:
                best_guess = i
                best_guess_error = current_error
                print()
            elif current_error < best_guess_error:
                best_guess = i
                best_guess_error = current_error
        print("%sQn=%i (error = %f, best=%d, current = %s)"
              % ("" if VERBOSE_MODE else "\r", i, best_guess_error, best_guess, current_error),
              end="\n\n" if VERBOSE_MODE else "")
        i += 1

    print("\rBest guess is Qn=%d error=%f" % (best_guess, best_guess_error))
    print("All valid Qn and their absolute errors")
    for qn, abs_error in non_error_qn:
        print("Qn: %d  ERR=%f" % (qn, abs_error))

    return best_guess


def find_best_optimization(qn: int):
    out = {}
    for optimization_level in (0, 1, 2, 3):
        build_with_flags([f"-DQN={qn}", f"-DBI_LIMIT={BI_LIMIT}", f"-O{optimization_level}"])

        output = subprocess.run(["./intTest"], stderr=subprocess.PIPE, stdout=subprocess.PIPE)

        prefix_str = "Elapsed CPU Time (Qn_timer) = "
        line = get_output_line(output.stdout, prefix_str)
        qn_time = float(line[len(prefix_str):])

        prefix_str = "Elapsed CPU Time (Floating_timer) = "
        line = get_output_line(output.stdout, prefix_str)
        floating_time = float(line[len(prefix_str):])

        out[optimization_level] = qn_time, floating_time
    return out


def main():
    # qn = find_best_qn()
    qn = 22
    opt_results = find_best_optimization(qn)
    for opt_level, (qn_time, float_time) in opt_results.items():
        print("-O%d: Qn: %f, FP: %f" % (opt_level, qn_time, float_time))


if __name__ == "__main__":
    main()
