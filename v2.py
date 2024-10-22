#!/usr/bin/env python3

import pickle

serialized_data = input("Enter serialized data: ")

deserialized_data = pickle.loads(serialized_data.encode('latin1'))  # Unsafe deserialization

user_input = input("Enter expression: ")

result = eval(user_input)  # Unsafe


user_input = input("Enter filename: ")

with open(user_input, 'r') as file:  # Vulnerable to directory traversal
    content = file.read()
