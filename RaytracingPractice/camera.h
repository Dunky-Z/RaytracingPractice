#pragma once

#include "ray.h"

class camera
{
public:
	RowVector3d lower_left_corner;
	RowVector3d horizontal;
	RowVector3d vertical;
	RowVector3d origin;
	camera() 
	{
		lower_left_corner = RowVector3d(-2.0, -1.0, -1.0);
		horizontal		  = RowVector3d(4.0, 0.0, 0.0);
		vertical		  = RowVector3d(0.0, 2.0, 0.0);
		origin			  = RowVector3d(0.0, 0.0, 0.0);
	}
	ray get_ray(double u, double v) { return ray(origin, lower_left_corner + u * horizontal + v * vertical); }


};
