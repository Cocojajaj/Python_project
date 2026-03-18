#!/usr/bin/env python3
import subprocess
import itertools

# --- Command Codes ---
CMD_ADD = 0xA1
CMD_MIX = 0xB2
CMD_CLEAR = 0xC3

# --- Color Codes ---
COLOR_FAIL = 0x00
COLOR_GREEN = 0x01
COLOR_RED = 0x02
COLOR_BLUE = 0x03
COLOR_MAGENTA = 0x04
COLOR_YELLOW = 0x05
COLOR_CYAN = 0x06
COLOR_WHITE = 0xFF

COLOR_NAMES = {
    COLOR_FAIL: "FAIL",
    COLOR_GREEN: "GREEN",
    COLOR_RED: "RED",
    COLOR_BLUE: "BLUE",
    COLOR_MAGENTA: "MAGENTA",
    COLOR_YELLOW: "YELLOW",
    COLOR_CYAN: "CYAN",
    COLOR_WHITE: "WHITE",
}

C_DIR = "/home/hubert/python_project/c_storefile"


import select

def run_mixer(executable, colors):

    try:
        proc = subprocess.Popen(
            [executable],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE
        )

        # send commands
        for c in colors:
            proc.stdin.write(bytes([CMD_ADD]))
            proc.stdin.write(bytes([c]))

        proc.stdin.write(bytes([CMD_MIX]))
        proc.stdin.flush()

        # wait max 1.0s for output
        ready, _, _ = select.select([proc.stdout], [], [], 1.0)

        if ready:
            result = proc.stdout.read(1)
            proc.kill()
            return result[0] if result else None

        # no output
        proc.kill()
        return None

    except Exception:
        return None


def test_all_pairs(executable):

    primaries = [COLOR_GREEN, COLOR_RED, COLOR_BLUE]

    expected = {
        (COLOR_GREEN, COLOR_GREEN): COLOR_GREEN,
        (COLOR_RED, COLOR_RED): COLOR_RED,
        (COLOR_BLUE, COLOR_BLUE): COLOR_BLUE,
        (COLOR_RED, COLOR_BLUE): COLOR_MAGENTA,
        (COLOR_BLUE, COLOR_RED): COLOR_MAGENTA,
        (COLOR_RED, COLOR_GREEN): COLOR_YELLOW,
        (COLOR_GREEN, COLOR_RED): COLOR_YELLOW,
        (COLOR_BLUE, COLOR_GREEN): COLOR_CYAN,
        (COLOR_GREEN, COLOR_BLUE): COLOR_CYAN,
    }

    print(f"\nTesting {executable}")

    for a, b in itertools.product(primaries, repeat=2):

        result = run_mixer(executable, [a, b])
        expected_val = expected.get((a, b), a)

        name = COLOR_NAMES.get(result, "NO_OUTPUT")

        status = "PASS" if result == expected_val else "FAIL"

        print(f"{COLOR_NAMES[a]} + {COLOR_NAMES[b]} -> {name} (expected {COLOR_NAMES[expected_val]}) [{status}]")


def test_white_permutation(executable):

    perms = itertools.permutations([COLOR_GREEN, COLOR_RED, COLOR_BLUE])

    print("\nWhite permutation tests")

    for p in perms:

        result = run_mixer(executable, list(p))

        name = COLOR_NAMES.get(result, "NO_OUTPUT")

        status = "PASS" if result == COLOR_WHITE else "FAIL"

        print(f"{[COLOR_NAMES[x] for x in p]} -> {name} [{status}]")


def stress_test_v3(executable):

    print("\nStress test (1000 mixes)")

    try:
        proc = subprocess.Popen(
            [executable],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE
        )

        for _ in range(1005):

            proc.stdin.write(bytes([CMD_ADD, COLOR_GREEN]))
            proc.stdin.write(bytes([CMD_ADD, COLOR_GREEN]))
            proc.stdin.write(bytes([CMD_MIX]))
            proc.stdin.flush()

            proc.stdout.read(1)

        print("Stress test finished")

        proc.kill()

    except Exception as e:
        print("Stress test error:", e)


def main():

    print("=== CMX-500 Color Mixer QA Suite ===")

    executables = [
        f"{C_DIR}/v1",
        f"{C_DIR}/v2",
        f"{C_DIR}/v3",
    ]

    for exe in executables:

        test_all_pairs(exe)
        test_white_permutation(exe)

        if exe.endswith("v3"):
            stress_test_v3(exe)

    print("\n=== QA Tests Complete ===")


if __name__ == "__main__":
    main()
