filename = "3thread_2_single"

times_seen = [0,0,0]
ms_used = [0,0,0]

with open(filename, "r") as file:
    for line in file:
        # Remove any trailing newline or spaces
        line = line.strip()
        # Split the line into an array by spaces
        parts = line.split()
        
        times_seen[int(parts[0])] += 1
        ms_used[int(parts[0])] += int(parts[1])

print("Process 0 avg time: ", ms_used[0] / times_seen[0])
print("Process 1 avg time: ", ms_used[1] / times_seen[1])
print("Process 2 avg time: ", ms_used[2] / times_seen[2])