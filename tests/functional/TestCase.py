##
## EPITECH PROJECT, 2025
## B-NWP-400-NAN-4-1-myftp-eliott.tesnier
## File description:
## TestCase
##

from Colors import *
from Utils import strip_ansi_codes
import subprocess
import time
import pty
import os
import signal

class TestCase:
    def __init__(self, name, desc, input, output, value, output_folder):
        self.name = name
        self.desc = desc
        self.input = input
        self.output = output
        self.value = value
        self.tty_mode = False
        self.tty_input = []

        self.real_output = None
        self.real_value = None
        self.raw_output = None

        if isinstance(self.input, dict) and "tty_mode" in self.input:
            self.tty_mode = True
            self.tty_input = self.input.get("commands", [])
            self.input = self.input.get("program", [])

        if "FILE CONTENT - " in self.output:
            self.output = self.output.replace("FILE CONTENT - ", "")
            self.output = self.output.strip()
            with open(output_folder + self.output, "r") as file:
                self.output = file.read()
                self.output = self.output.replace("\n", "")

    def execute(self):
        if self.tty_mode:
            self._execute_tty()
        else:
            self._execute_normal()

    def _execute_normal(self):
        process = subprocess.Popen(self.input, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        process.wait(timeout=30)

        raw_output = process.stdout.read().decode('utf-8')
        raw_output += process.stderr.read().decode('utf-8')

        self.raw_output = raw_output
        self.real_output = strip_ansi_codes(raw_output).replace("\n", "")
        self.real_value = process.returncode

    def _execute_tty(self):
        master, slave = pty.openpty()
        process = subprocess.Popen(
            self.input,
            stdin=slave,
            stdout=slave,
            stderr=slave,
            text=True,
            universal_newlines=True,
            start_new_session=True
        )

        os.close(slave)
        output_buffer = ""

        for cmd in self.tty_input:
            try:
                time.sleep(0.1)
                os.write(master, (cmd + "\n").encode())
                time.sleep(0.1)

                try:
                    os.set_blocking(master, False)
                    while True:
                        try:
                            output = os.read(master, 1024).decode('utf-8')
                            if output:
                                output_buffer += output
                        except BlockingIOError:
                            break
                except:
                    pass

            except Exception as e:
                print(f"Error sending command: {e}")
                break

        timeout = time.time() + 3

        try:
            os.set_blocking(master, True)
            while time.time() < timeout:
                try:
                    from select import select
                    readable, _, _ = select([master], [], [], 0.5)
                    if master in readable:
                        output = os.read(master, 1024).decode('utf-8')
                        if output:
                            output_buffer += output
                        else:
                            break
                    else:
                        break
                except:
                    break
        except:
            pass

        os.close(master)

        try:
            if process.poll() is None:
                process.terminate()
                try:
                    process.wait(timeout=1)
                except subprocess.TimeoutExpired:
                    os.killpg(os.getpgid(process.pid), signal.SIGKILL)
        except:
            pass

        self.raw_output = output_buffer
        self.real_output = strip_ansi_codes(output_buffer).replace("\n", "")
        self.real_value = process.returncode if process.returncode is not None else 0

    def check(self):
        if self.output.startswith("CONTAINS - "):
            search_text = self.output[11:]
            if search_text not in self.real_output and self.output != "ANYTHING":
                return False
        elif self.real_output != self.output and self.output != "ANYTHING":
            return False

        if self.real_value != self.value:
            return False

        return True

    def displayPassed(self, index):
        print(f"{WHITE}Test {index + 1:03d} {BLUE}|{END} {self.name}{END} {GREEN}OK ✅{END}")

    def displayFailed(self, index):
        print(f"{WHITE}Test {index + 1:03d} | {self.name}{END} {RED}KO ❌{END}")
        print(f"    {YELLOW}Description: {END}{self.desc}")
        print(f"    {YELLOW}Expected : {RED}{self.value}{END} | {RED}{self.output}{END}")
        print(f"    {YELLOW}Got      : {RED}{self.real_value}{END} | {RED}{self.real_output}{END}")
