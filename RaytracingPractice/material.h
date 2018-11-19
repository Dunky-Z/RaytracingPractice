#pragma once

#include "hitable.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const = 0;
};

//在单位球体内生成随机方向向量
RowVector3d random_in_unit_sphere()
{
	RowVector3d p;
	do
	{
		p = 2.0*RowVector3d(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX)) - RowVector3d(1, 1, 1);
	} while (p.dot(p) >= 1.0);
	return p;
}

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
	metal(const RowVector3d& a) : albedo(a) {}

	//这里具体实现metal::scatter()。做两件事情：获取镜面反射的反射光线；获取材料的衰减系数
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const
	{
		RowVector3d reflected = reflect(r_in.direction() / ((r_in.direction()).norm()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return ((scattered.direction()).dot(rec.normal) > 0);
	}
};