#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
    public:
        double aspect_ratio = 1.0;         // Image aspect ratio (width / height)
        int image_width = 100;             // Image width in pixels
        int samples_per_pixel = 10;        // Random samples for each pixel
        int max_depth = 10;                // Maximum number of ray bounces

        double vfov = 90;                  // Field of view
        point3 lookfrom = point3(0, 0, 0); // Point camera is looking from
        point3 lookat = point3(0, 0, -1);  // Point camera is looking at
        vec3 vup = vec3(0, 1, 0);          // "Up" direction relative to camera

        double defocus_angle = 0;          // Variation angle of rays through each pixel
        double focus_dist = 10;            // Distance from camera lookfrom point to plane of perfect focus

        void render(const hittable &scene) {
            initialize();

            std::vector<colour> image(image_height * image_width);
            int rows_processed = 0;        // For progress bar

            omp_set_num_threads(8);
            #pragma omp parallel for
            for (int row = 0; row < image_height; row++) {
                for (int col = 0; col < image_width; col++) {
                    colour pixel_colour(0, 0, 0);

                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(col, row);
                        pixel_colour += ray_colour(r, max_depth, scene);
                    }

                    image[row * image_width + col] = pixel_colour * pixel_samples_scale;
                }

                #pragma omp atomic
                rows_processed++;

                #pragma omp critical
                std::clog << "\rScanlines remaining: " << (image_height - rows_processed) << ' ' << std::flush;
            }

            std::clog << "\rDone.                       \n";
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int row = 0; row < image_height; row++) {
                for (int col = 0; col < image_width; col++) {
                    write_colour(std::cout, image[row * image_width + col]);
                }
            }

        }

    private:
        int image_height;                  // Image height in pixels
        double pixel_samples_scale;        // Colour scale factor for a sum of pixel samples
        point3 center;                     // Camera center
        point3 pixel00_loc;                // Pixel at (0, 0)
        vec3 pixel_delta_u;                // Horizontal pixel offset
        vec3 pixel_delta_v;                // Vertical pixel offset
        vec3 u, v, w;                      // Camera frame orthonormal basis vectors
        vec3 defocus_disk_u;               // Defocus disk horizontal radius
        vec3 defocus_disk_v;               // Defocus disk vertical radius

        void initialize() {
            // Calculate image height based on aspect ratio
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
            pixel_samples_scale = 1.0 / samples_per_pixel;

            // Set camera center
            center = lookfrom;

            // Viewport dimensions
            double theta = degrees_to_radians(vfov);
            double h = std::tan(theta / 2);
            double viewport_height = 2 * h * focus_dist;
            double viewport_width = viewport_height * (double(image_width) / image_height);

            // Calculate orthonormal basis vectors
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Vectors across horizontal and down vertical viewport edges
            vec3 viewport_u = viewport_width * u;
            vec3 viewport_v = viewport_height * -v;

            // Horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Location of upper left pixel
            point3 viewport_upper_left = center - (focus_dist * w) - (viewport_u / 2) - (viewport_v / 2);
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Camera defocus disk basis vectors
            double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        ray get_ray(int i, int j) const {
            vec3 offset = sample_square();
            point3 pixel_sample = pixel00_loc 
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

            point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            vec3 ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
            vec3 p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        colour ray_colour(const ray &r, int depth, const hittable &scene) const {
            if (depth <= 0) return colour(0, 0, 0);

            hit_record rec;

            if (scene.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                colour attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_colour(scattered, depth - 1, scene);
                } else {
                    return colour(0, 0, 0);
                }
            }

            vec3 unit_direction = unit_vector(r.direction());
            double a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
        }
};

#endif
