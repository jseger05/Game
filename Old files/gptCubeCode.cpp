#include <iostream>
#include <vector>
#include <cmath>

// Define a structure to hold 3D coordinates
struct Vec3 {
    double x, y, z;
};

// Function to perform rotation around the x-axis (roll)
Vec3 rotateX(const Vec3& v, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return {v.x, v.y * cosA - v.z * sinA, v.y * sinA + v.z * cosA};
}

// Function to perform rotation around the y-axis (pitch)
Vec3 rotateY(const Vec3& v, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return {v.x * cosA + v.z * sinA, v.y, -v.x * sinA + v.z * cosA};
}

// Function to perform rotation around the z-axis (yaw)
Vec3 rotateZ(const Vec3& v, double angle) {
    double cosA = cos(angle);
    double sinA = sin(angle);
    return {v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA, v.z};
}

// Function to rotate a vector using Tait-Bryan angles
Vec3 rotate(const Vec3& v, double roll, double pitch, double yaw) {
    Vec3 rotated = rotateX(v, roll);
    rotated = rotateY(rotated, pitch);
    rotated = rotateZ(rotated, yaw);
    return rotated;
}

// Function to compute the 8 vertices of the rotated cube
std::vector<Vec3> getCubeVertices(const Vec3& center, double lx, double ly, double lz, double roll, double pitch, double yaw) {
    // Half lengths
    double hx = lx / 2.0;
    double hy = ly / 2.0;
    double hz = lz / 2.0;

    // Vertices of the cube before rotation (relative to the center)
    std::vector<Vec3> vertices = {
        {-hx, -hy, -hz},
        { hx, -hy, -hz},
        { hx,  hy, -hz},
        {-hx,  hy, -hz},
        {-hx, -hy,  hz},
        { hx, -hy,  hz},
        { hx,  hy,  hz},
        {-hx,  hy,  hz}
    };

    // Apply rotation and translation to each vertex
    for (auto& vertex : vertices) {
        vertex = rotate(vertex, roll, pitch, yaw);
        vertex.x += center.x;
        vertex.y += center.y;
        vertex.z += center.z;
    }

    return vertices;
}

int main() {
    // Example usage
    Vec3 center = {0, 0, 0};
    double lx = 2, ly = 2, lz = 2;
    double roll = M_PI / 0.5; // 45 degrees
    double pitch = M_PI / 0.5; // 45 degrees
    double yaw = M_PI / 4; // 45 degrees

    std::vector<Vec3> vertices = getCubeVertices(center, lx, ly, lz, roll, pitch, yaw);

    // Print the vertices
    for (const auto& v : vertices) {
        std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    }

    return 0;
}
