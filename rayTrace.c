#include <stdio.h>
#include <math.h>

// Define a 3D vector structure
typedef struct {
    double x, y, z;
} Vec3;

// Define a structure for representing rays
typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

// Define a structure for representing spheres
typedef struct {
    Vec3 center;
    double radius;
} Sphere;

// Define a structure for representing cubes
typedef struct {
    Vec3 min;
    Vec3 max;
} Cube;

// Function to calculate the dot product of two vectors
double dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Function to calculate the length of a vector
double length(Vec3 v) {
    return sqrt(dot(v, v));
}

// Function to subtract two vectors
Vec3 subtract(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

// Function to check if a ray intersects with a sphere
int hit_sphere(Sphere sphere, Ray ray) {
    Vec3 oc = subtract(ray.origin, sphere.center);
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(oc, ray.direction);
    double c = dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

int main() {
    int width = 200;
    int height = 100;

    printf("P3\n%d %d\n255\n", width, height);

    Vec3 lower_left_corner = { -2.0, -1.0, -1.0 };
    Vec3 horizontal = { 4.0, 0.0, 0.0 };
    Vec3 vertical = { 0.0, 2.0, 0.0 };
    Vec3 origin = { 0.0, 0.0, 0.0 };

    Sphere sphere = { {0, 1, -1}, 0.5 };  // Sphere position

    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            double u = (double)i / (width - 1);
            double v = (double)j / (height - 1);

            Ray ray;
            ray.origin = origin;
            ray.direction = subtract(
                subtract(add(add(lower_left_corner, scale(horizontal, u)), scale(vertical, v)), origin),
                origin
            );

            // Checkered base
            int checker = ((int)(10 * ray.origin.x) + (int)(10 * ray.origin.z)) % 2;
            Vec3 base_color = { 0.8, 0.8, 0.8 };
            if (checker)
                base_color = scale(base_color, 0.2);  // Darker checkered color

            // Check intersection with the sphere
            if (hit_sphere(sphere, ray)) {
                printf("%d %d %d\n", 0, 0, 255);  // Blue for sphere
            } else {
                // Mix base color with background based on ray direction
                Vec3 unit_direction = ray.direction;
                double t = 0.5 * (unit_direction.y + 1.0);
                Vec3 color = scale(base_color, 1.0 - t);
                printf("%d %d %d\n", (int)(255.99 * color.x), (int)(255.99 * color.y), (int)(255.99 * color.z));
            }
        }
    }

    return 0;
}