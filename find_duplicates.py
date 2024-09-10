import sys
import os
import time

def find_most_consecutive_common_lines(file_a, file_b):
    # Read the lines from both files
    with open(file_a, 'r') as fa, open(file_b, 'r') as fb:
        lines_a = [line.strip() for line in fa.readlines()]
        lines_b = [line.strip() for line in fb.readlines()]

    # Find the longest common consecutive lines using dynamic programming
    len_a, len_b = len(lines_a), len(lines_b)
    dp = [[0] * (len_b + 1) for _ in range(len_a + 1)]
    
    max_len = 0 # To store the length of the longest match
    max_pos = (0, 0) # To store the end position of the longest match

    for i in range(1, len_a + 1):
        for j in range(1, len_b + 1):
            if lines_a[i - 1] == lines_b[j - 1]:
                dp[i][j] = dp[i - 1][j - 1] + 1
                if dp[i][j] > max_len:
                    max_len = dp[i][j]
                    max_pos = (i, j)

    # Extract the longest common sequence from the dp table
    i, j = max_pos
    common_lines = lines_a[i - max_len:i]

    # Print the results
    if max_len > 0:
        print(f"Most common duplicate lines between given files are {max_len} consecutive lines:")
        for line in common_lines:
            print(line)
    else:
        print("No common consecutive lines found.")

# Call the function with the file paths
#find_most_consecutive_common_lines(sys.argv[1], sys.argv[2])

files = [os.path.join(root, file) 
	for root, _, files in os.walk('./') 
	for file in files]

for i in range(len(files)):
    for j in range(len(files)):
        if i != j:  # Ensure that we are not processing the same file
            file1 = files[i]
            file2 = files[j]
            print(f"Processing {file1} with {file2}")
            
            # Call the Python script with the two files as arguments
            find_most_consecutive_common_lines(file1, file2)
            
            # Sleep for 1 second
            time.sleep(1)