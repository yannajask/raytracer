#include "utils.h"
#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "quad.h"
#include "texture.h"

void in_one_weekend() {
    hittable_list scene;

    auto ground_material = make_shared<checker_texture>(0.32, colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9));
    scene.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(ground_material)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);

                } else {
                    // Glass
                    sphere_material = make_shared<dielectric>(1.5);
                }

                scene.add(make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    scene.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    scene.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    cam.background = colour(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(scene);
}

void checkered_spheres() {
    hittable_list scene;

    auto checker = make_shared<checker_texture>(0.32, colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9));

    scene.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    scene.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = colour(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(scene);
}

void infinity_room() {
    hittable_list scene;

    auto mirror = make_shared<metal>(colour(0.5, 0.55, 0.53), 0.0);
    auto ball = make_shared<metal>(colour(0.5, 0.6, 0.5), 0.05);
    auto white = make_shared<lambertian>(colour(.73, .73, .73));
    auto black = make_shared<lambertian>(colour(0.02, 0.02, 0.02));
    auto light = make_shared<diffuse_light>(colour(20, 20, 20));

    scene.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), mirror));
    scene.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), mirror));
    scene.add(make_shared<quad>(point3(5, 554, 5), vec3(545,0,0), vec3(0,0,545), light));
    scene.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), black));
    scene.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    scene.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), mirror));
    scene.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,555,0), mirror));

    for (int i = 0; i < 35; i++) {
        double radius = random_double(15, 35);
        point3 center(random_double(35, 520), radius, i + random_int(35, 520));
        scene.add(make_shared<sphere>(center, radius, ball));
    }

    for (int i = 0; i < 10; i++) {
        double radius = random_double(15, 20);
        point3 center(random_double(50, 500), random_double(100, 400), i + random_int(50, 500));
        scene.add(make_shared<sphere>(center, radius, ball));
    }

    scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 1000;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;
    cam.background = colour(0, 0, 0);

    cam.vfov = 20;
    cam.lookfrom = point3(200, 278, 5);
    cam.lookat = point3(278, 278, 100);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0;

    cam.render(scene);
}

int main() {
    infinity_room();
}