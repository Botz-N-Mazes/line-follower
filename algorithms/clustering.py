import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

# Function to generate a synthetic dataset
def generate_dataset(size=100, noise_level=10):
    np.random.seed(42)
    black_data = np.random.normal(50, noise_level, size // 2)  # Black readings
    white_data = np.random.normal(200, noise_level, size // 2)  # White readings
    dataset = np.concatenate((black_data, white_data))
    np.random.shuffle(dataset)
    return dataset

# Function to perform k-means clustering
def classify_data_with_kmeans(data, n_clusters=2):
    kmeans = KMeans(n_clusters=n_clusters, random_state=42)
    labels = kmeans.fit_predict(data.reshape(-1, 1))
    centroids = kmeans.cluster_centers_
    return labels, centroids

# Function to plot data and centroids
def plot_data_with_centroids_and_save(data, labels, centroids):
    plt.figure(figsize=(10, 6))
    unique_labels = np.unique(labels)
    for label in unique_labels:
        plt.scatter(data[labels == label], [label] * len(data[labels == label]), label=f'Cluster {label}')
    plt.scatter(centroids, [-0.5] * len(centroids), color='red', marker='x', s=100, label='Centroids')
    plt.title("IR Sensor Data Classification")
    plt.xlabel("Sensor Readings")
    plt.ylabel("Clusters")
    plt.legend()
    plt.show()
    open("output.img", "w").write(f"Dataset: {dataset}\nCentroids: {centroids.ravel()}")

# Generate dataset
dataset = generate_dataset(size=100, noise_level=10)

# Classify data using k-means
labels, centroids = classify_data_with_kmeans(dataset)

# Plot data and centroids
plot_data_with_centroids_and_save(dataset, labels, centroids)

# Display dataset and centroids for verification
dataset, centroids.ravel()
