// RaytracingPractice.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include "ray.h"
#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <float.h>
#include "material.h"
 
//#define RAND() (rand()/double(RAND_MAX))

using namespace std;
using namespace Eigen;

////在单位球体内生成随机方向向量
//RowVector3d random_in_unit_sphere()
//{
//	RowVector3d p;
//	do
//	{
//		p = 2.0*RowVector3d(rand()/double(RAND_MAX), rand()/double(RAND_MAX), rand()/double(RAND_MAX)) - RowVector3d(1, 1, 1);
//	} while (p.dot(p) >= 1.0);
//	return p;
//}

RowVector3d color(const ray& r, hitable *world,int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		RowVector3d attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation.cwiseProduct(color(scattered, world, depth + 1));
		}
		else
		{
			return RowVector3d(0, 0, 0);
		}
	}
	else 
	{
		RowVector3d unit_direction = ((r.direction()) / ((r.direction()).norm()));	//单位向量
		double t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*RowVector3d(1.0, 1.0, 1.0) + t * RowVector3d(1.0, 0.5, 0.7);
	}
}


int main()
{
	int nx = 200;
	int ny = 100;
	int ns = 50;		//每个像素点采样次数

	ofstream outfile("mytest.ppm", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[4];
	list[0] = new sphere(RowVector3d(0, 0, -1), 0.5, new lambertian(RowVector3d(0.8, 0.3, 0.3)));
	list[1] = new sphere(RowVector3d(0, -100.5, -1), 100, new lambertian(RowVector3d(0.8, 0.0, 0.0)));
	list[2] = new sphere(RowVector3d(1, 0, -1), 0.5, new metal(RowVector3d(0.8, 0.6, 0.2)));
	list[3] = new sphere(RowVector3d(-1, 0, -1), 0.5, new metal(RowVector3d(0.8, 0.8, 0.8)));
	hitable *world = new hitable_list(list, 4);
	camera cam;
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			RowVector3d col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				double u = double(i + rand()/double(RAND_MAX)) / double(nx);
				double v = double(j + rand()/double(RAND_MAX)) / double(ny);
				ray r = cam.get_ray(u, v);
				RowVector3d p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= double(ns);
			col = RowVector3d(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));	//伽马校正
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	system("pause");
}