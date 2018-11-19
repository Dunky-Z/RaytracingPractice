#pragma once

#include "hitable.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const = 0;
};

//�ڵ�λ���������������������
RowVector3d random_in_unit_sphere()
{
	RowVector3d p;
	do
	{
		p = 2.0*RowVector3d(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX)) - RowVector3d(1, 1, 1);
	} while (p.dot(p) >= 1.0);
	return p;
}

//��������
class lambertian : public material
{
public:
	RowVector3d albedo;
	lambertian(const RowVector3d& a) : albedo(a) {}
	//��Ҫ��Ϊ�˱���˥��ϵ��,�������ʵ��lambertian::scatter()�����������飺��ȡ������ķ�����ߣ���ȡ���ϵ�˥��ϵ��
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const
	{
		RowVector3d target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};

//��ȡ���淴��ķ�����ߵķ�������
RowVector3d reflect(const RowVector3d& v, const RowVector3d& n)
{
	return v - 2 * (v.dot(n)) * n;
}



//���淴����
class metal : public material
{
public:
	//double fuzz;	//ģ��ϵ�����������淴��������̶�
	RowVector3d albedo;
	metal(const RowVector3d& a) : albedo(a) {}

	//�������ʵ��metal::scatter()�����������飺��ȡ���淴��ķ�����ߣ���ȡ���ϵ�˥��ϵ��
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const
	{
		RowVector3d reflected = reflect(r_in.direction() / ((r_in.direction()).norm()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return ((scattered.direction()).dot(rec.normal) > 0);
	}
};