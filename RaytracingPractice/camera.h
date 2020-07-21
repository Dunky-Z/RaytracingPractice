#pragma once

#include "ray.h"

#define pi 3.1415926

RowVector3d random_in_unit_disk() {
	/*在z=0平面上产生一个“起点在原点，长度小于1，方向随机”的向量。为什么是z=0平面，这个和相机的倾斜方向有关。（相机的倾斜方向为view up （简称vup，一般设置为（0，1，0）））*/
	RowVector3d p;
	do
	{
		p = 2.0*RowVector3d((rand() % (100) / (double)(100)), (rand() % (100) / (double)(100)), 0) - RowVector3d(1, 1, 0);
	} while (p.dot(p) >= 1.0);
	return p;
}
class camera
{
public:
	RowVector3d lower_left_corner;
	RowVector3d horizontal;
	RowVector3d vertical;
	RowVector3d origin;
	RowVector3d u, v, w;
	double lens_radius;

	camera(RowVector3d lookfrom, RowVector3d lookat, RowVector3d vup, double vfov, double aspect, double aperture, double focus_dist)
	{
		lens_radius = aperture / 2.0;
		double theta = vfov * pi / 180.0;
		double half_height = tan(theta / 2.0);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = (lookfrom - lookat) / ((lookfrom - lookat).norm());
		u = (vup.cross(w)) / ((vup.cross(w)).norm());
		v = w.cross(u);
		lower_left_corner = origin - half_width * focus_dist* u - half_height * focus_dist * v - focus_dist * w;
		vertical = 2 * half_height*focus_dist*v;
		horizontal = 2 * half_width*focus_dist*u;
	}
	ray get_ray(double s, double t)
	{
		RowVector3d rd = lens_radius * random_in_unit_disk();
		RowVector3d offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}
};
