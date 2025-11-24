#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
    public:
        double aspect_ratio = 1.0;        // Image aspect ratio (width / height)
        int image_width = 100;            // Image width in pixels
        int samples_per_pixel = 10;       // Random samples for each pixel
        int max_depth = 10;               // Maximum number of ray bounces

        void render(const hittable &scene) {
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    colour pixel_colour(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_colour += ray_colour(r, max_depth, scene);
                    }

                    write_colour(std::cout, pixel_samples_scale * pixel_colour);
                }
            }

            std::clog << "\rDone.                           \n";
        }

    private:
        int image_height;                 // Image height in pixels
        double pixel_samples_scale;       // Colour scale factor for a sum of pixel samples
        point3 center;                    // Camera center
        point3 pixel00_loc;               // Pixel at (0, 0)
        vec3 pixel_delta_u;               // Horizontal pixel offset
        vec3 pixel_delta_v;               // Vertical pixel offset

        void initialize() {
            // Calculate image height based on aspect ratio
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
            pixel_samples_scale = 1.0 / samples_per_pixel;

            // Set camera center to (0, 0, 0)
            center = point3(0, 0, 0);

            // Viewport dimensions
            double focal_length = 1.0;
            double viewport_height = 2.0;
            double viewport_width = viewport_height * (double(image_width) / image_height);

            // Vectors across horizontal and down vertical viewport edges
            vec3 viewport_u = vec3(viewport_width, 0, 0);
            vec3 viewport_v = vec3(0, -viewport_height, 0);

            // Horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Location of upper left pixel
            point3 viewport_upper_left = center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        ray get_ray(int i, int j) const {
            vec3 offset = sample_square();
            point3 pixel_sample = pixel00_loc 
                                + ((i + offset.x()) * pixel_delta_u)
                                + ((j + offset.y()) * pixel_delta_v);

            point3 ray_origin = center;
            vec3 ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
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
