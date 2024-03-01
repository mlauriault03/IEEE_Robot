import csv
import matplotlib.pyplot as plt

def plot_coordinates(csv_file):
    x_values = []
    y_values = []

    # Read coordinates from CSV file
    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) >= 2:
                x_values.append(float(row[0]))
                y_values.append(float(row[1]))

    # Set up plot
    plt.figure(figsize=(6, 6))  # Equal aspect ratio
    plt.scatter(x_values, y_values, color='blue', marker='o')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Robot Path')
    plt.grid(True)
    plt.axis('equal')  # Equal scaling on both axes
    plt.show()

# Provide the path to your CSV file here
csv_file_path = 'testing/path.csv'
plot_coordinates(csv_file_path)
