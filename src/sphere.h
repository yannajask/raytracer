#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const point3 &center, double radius, shared_ptr<material> mat) 
         : center(center), radius(std::fmax(0, radius)), mat(mat) {
            vec3 rvec = vec3(radius, radius, radius);
            bbox = aabb(center - rvec, center + rvec);
         }

        bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
            vec3 oc = center - r.origin();
            double a = r.direction().length_squared();
            double h = dot(r.direction(), oc);
            double c = oc.length_squared() - (radius * radius);
            auto discriminant = h * h - a * c;

            if (discriminant < 0) return false;

            double sqrtd = std::sqrt(discriminant);
            double root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat = mat;

            return true;
        }

        aabb bounding_box() const override { return bbox; }

    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;
        aabb bbox;

        static void get_sphere_uv(point3 &p, double &u, double &v) {
            double theta = std::acos(-p.y());
            double phi = std::atan2(-p.z(), p.x()) + pi;
            u = phi / (2 * pi);
            v = theta / pi;
        }
};

#endif
