#ifndef AABB_H
#define AABB_H

#include "utils.h"

class aabb {
    public:
        interval x, y, z;

        aabb() {}

        aabb(const interval &x, const interval &y, const interval &z) : x(x), y(y), z(z) {
            pad_to_minimums();
        }

        aabb(const point3 &a, const point3 &b) {
            x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
            y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
            z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
            pad_to_minimums();
        }

        aabb(const aabb &box0, const aabb &box1) {
            x = interval(box0.x, box1.x);
            y = interval(box0.y, box1.y);
            z = interval(box0.z, box1.z);
        }

        const interval &axis_interval(int n) const {
            if (n == 1) {
                return y;
            } else if (n == 2) {
                return z;
            } else {
                return x;
            }
        }

        bool hit(const ray &r, interval ray_t) const {
            const point3 &ray_origin = r.origin();
            const vec3 &ray_dir = r.direction();

            for (int axis = 0; axis < 3; axis++) {
                const interval &ax = axis_interval(axis);
                const double adinv = 1.0 / ray_dir[axis];

                double t0 = (ax.min - ray_origin[axis]) * adinv;
                double t1 = (ax.max - ray_origin[axis]) * adinv;

                if (t0 < t1) {
                    ray_t.min = std::max(ray_t.min, t0);
                    ray_t.max = std::min(ray_t.max, t1);
                } else {
                    ray_t.min = std::max(ray_t.min, t1);
                    ray_t.max = std::min(ray_t.max, t0);
                }
            }

            return (ray_t.max > ray_t.min);
        }

        int longest_axis() const {
            if (x.size() > y.size()) {
                return x.size() > z.size() ? 0 : 2;
            } else {
                return y.size() > z.size() ? 1 : 2;
            }
        }

        static const aabb empty, universe;

    private:
        void pad_to_minimums() {
            double delta = 0.0001;
            if (x.size() < delta) x = x.expand(delta);
            if (y.size() < delta) y = y.expand(delta);
            if (z.size() < delta) z = z.expand(delta);
        }
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif
