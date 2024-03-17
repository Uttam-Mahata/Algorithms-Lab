import matplotlib.pyplot as plt

partition_sizes = [3, 5, 7, 9, 11] # Update with your partition sizes
times = [0.015, 0.019, 0.015, 0.012, 0.019] # Update with your corresponding times

plt.plot(partition_sizes, times)
plt.xlabel('Partition Size')
plt.ylabel('Time (seconds)')
plt.title('Time vs Partition Size')
plt.grid(True)
plt.show()
