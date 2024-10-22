#!/usr/bin/env python3

import os


directory = input("Enter the directory to list: ")

command = f"ls {directory}"  # Vulnerable to Command Injection

os.system(command)
