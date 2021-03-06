// RaytracingPractice.cpp: 定义控制台应用程序的入口点。

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "ray.h"
#include "hitable.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include <float.h>
#include "material.h"

#define RAND() (rand()/double(RAND_MAX))

using namespace std;
using namespace Eigen;


hitable *random_scene() 
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(RowVector3d(0, -1000, 0), 1000, new lambertian(RowVector3d(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) 
	{
		for (int b = -11; b < 11; b++) 
		{
			double choose_mat =RAND();
			RowVector3d center(a + 0.9*RAND(), 0.2, b + 0.9*RAND());
			if ((center - RowVector3d(4, 0.2, 0)).norm() > 0.9) 
			{
				if (choose_mat < 0.8)
				{  // diffuse
					list[i++] = new sphere(center, 0.2, new lambertian(RowVector3d(RAND()*RAND(), RAND()*RAND(), RAND()*RAND())));
				}
				else if (choose_mat < 0.95) 
				{ // metal
					list[i++] = new sphere(center, 0.2,	new metal(RowVector3d(0.5*(1 + RAND()), 0.5*(1 + RAND()), 0.5*(1 + RAND()))));
				}
				else
				{  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(RowVector3d(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(RowVector3d(-4, 1, 0), 1.0, new lambertian(RowVector3d(0.4, 0.2, 0.1)));
	list[i++] = new sphere(RowVector3d(4, 1, 0), 1.0, new metal(RowVector3d(0.7, 0.6, 0.5)));

	return new hitable_list(list, i);
}


RowVector3d color(const ray& r, hitable *world, int depth)
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
		return (1.0 - t)*RowVector3d(1.0, 1.0, 1.0) + t * RowVector3d(0.5, 0.7, 1.0);
	}
}


int main()
{
	int nx = 1200;		//图像长度
	int ny = 800;		//图像高度
	int ns = 10;		//每个像素点采样次数

	ofstream outfile("mytest.ppm", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[5];
	list[0] = new sphere(RowVector3d(0, 0, -1), 0.5, new lambertian(RowVector3d(0.1, 0.2, 0.5)));
	list[1] = new sphere(RowVector3d(0, -100.5, -1), 100, new lambertian(RowVector3d(0.8, 0.8, 0.0)));
	list[2] = new sphere(RowVector3d(1, 0, -1), 0.5, new metal(RowVector3d(0.8, 0.6, 0.2)));
	list[3] = new sphere(RowVector3d(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(RowVector3d(-1, 0, -1), -0.5, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);
	world = random_scene();

	RowVector3d lookfrom(13, 2, 3);
	RowVector3d lookat(0, 0, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.1;

	camera cam(lookfrom, lookat, RowVector3d(0, 1, 0), 20, double(nx) / double(ny), aperture, dist_to_focus);
	
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			RowVector3d clr(0, 0, 0);
			//进行ns次采样，消除锯齿
			for (int s = 0; s < ns; s++)
			{
				double u = double(i + RAND()) / double(nx);
				double v = double(j + RAND()) / double(ny);
				ray r = cam.get_ray(u, v);
				RowVector3d p = r.point_at_parameter(2.0);
				clr += color(r, world, 0);	//将随机采样的颜色值相加
			}
			clr /= double(ns);				//求平均获得最终颜色值
			clr = RowVector3d(sqrt(clr[0]), sqrt(clr[1]), sqrt(clr[2]));	//伽马校正
			//设置光的颜色
			int ir = int(255.99*clr[0]);
			int ig = int(255.99*clr[1]);
			int ib = int(255.99*clr[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	system("pause");
}