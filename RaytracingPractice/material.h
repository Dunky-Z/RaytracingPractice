#pragma once

#include "hitable.h"
#include "ray.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const = 0;
};


//漫反射类
class lambertian : public material
{
public:
	RowVector3d albedo;
	lambertian(const RowVector3d& a) : albedo(a) {}
	//主要是为了保存衰减系数,这里具体实现lambertian::scatter()。做两件事情：获取漫反射的反射光线；获取材料的衰减系数
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const
	{
		RowVector3d target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};

//获取镜面反射的反射光线的方向向量
RowVector3d reflect(const RowVector3d& v, const RowVector3d& n)
{
	return v - 2 * (v.dot(n)) * n;
}



//镜面反射类
class metal : public material
{
public:
	//double fuzz;	//模糊系数，调整镜面反射的清晰程度
	RowVector3d albedo;
	double fuzz;
	metal(const RowVector3d& a) : albedo(a){}

	//这里具体实现metal::scatter()。做两件事情：获取镜面反射的反射光线；获取材料的衰减系数
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const
	{
		RowVector3d reflected = reflect(r_in.direction() / ((r_in.direction()).norm()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return ((scattered.direction()).dot(rec.normal) > 0);
	}
};


bool refract(const RowVector3d& v, const RowVector3d& n, double ni_over_nt, RowVector3d& refracted)
{
	//计算折射光线的方向向量
	RowVector3d uv = v / (v.norm());
	double dt = uv.dot(n);
	double discriminat = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminat > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminat);
		return true;
	}
	else
		return false;
}


class dielectric : public material
{
public:
	double ref_idx;
	dielectric(double ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const
	{
		RowVector3d outward_normal;
		RowVector3d reflected = reflect(r_in.direction(), rec.normal);
		double ni_over_nt;
		attenuation = RowVector3d(1.0, 1.0, 1.0);	//介质的衰减向量为（1，1，1）不是光线不衰减*
		RowVector3d refracted;
		//光线的方向向量和球的法向量的点乘大于零，说明光线是从球体内部射入空气。所以，入射时的法向量和球的法向量方向相反；
		//注意，ref_idx是指光密介质的折射指数和光疏介质的折射指数的比值，此时入射介质是光密介质，折射介质是光疏介质，所以ni_over_nt=ref_idx
		if (r_in.direction().dot(rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
		}
		//光线的方向向量和球的法向量的点乘bu大于零，说明光线是从空气射入球体气。所以，入射时的法向量和球的法向量方向同向
		//注意，ref_idx是指光密介质的折射指数和光疏介质的折射指数的比值，此时入射介质是光疏介质，折射介质是光密介质，所以ni_over_nt=1.0/ref_idx
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
		}
		//如果有折射，计算反射系数。如果没有反射，即发生了全反射，反射系数为1
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			scattered = ray(rec.p, refracted);
		}
		else
		{
			scattered = ray(rec.p, reflected);
			return false;
		}
			/*产生一个（0，1）的随机数，如果随机数小于反射系数，则设置为反射光线，反之，设置为折射光线。
		也就是只有反射光线或折射光线中的一个，为什么？不是说好反射光线和折射光线都有吗？考虑到一个像素点被设置为采样100次，
		这100次中反射光线的条数基本和reflect_prob的值正相关，所以，100次的平均值也就是该像素点反射光线和折射光线的叠加*/
		return true;
	}
};
