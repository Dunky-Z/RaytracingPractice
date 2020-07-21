#pragma once

#include "hitable.h"
#include "ray.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, RowVector3d& attenuation, ray& scattered) const = 0;
};


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
	double fuzz;
	metal(const RowVector3d& a) : albedo(a){}

	//�������ʵ��metal::scatter()�����������飺��ȡ���淴��ķ�����ߣ���ȡ���ϵ�˥��ϵ��
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
	//����������ߵķ�������
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
		attenuation = RowVector3d(1.0, 1.0, 1.0);	//���ʵ�˥������Ϊ��1��1��1�����ǹ��߲�˥��*
		RowVector3d refracted;
		//���ߵķ�����������ķ������ĵ�˴����㣬˵�������Ǵ������ڲ�������������ԣ�����ʱ�ķ���������ķ����������෴��
		//ע�⣬ref_idx��ָ���ܽ��ʵ�����ָ���͹�����ʵ�����ָ���ı�ֵ����ʱ��������ǹ��ܽ��ʣ���������ǹ�����ʣ�����ni_over_nt=ref_idx
		if (r_in.direction().dot(rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
		}
		//���ߵķ�����������ķ������ĵ��bu�����㣬˵�������Ǵӿ������������������ԣ�����ʱ�ķ���������ķ���������ͬ��
		//ע�⣬ref_idx��ָ���ܽ��ʵ�����ָ���͹�����ʵ�����ָ���ı�ֵ����ʱ��������ǹ�����ʣ���������ǹ��ܽ��ʣ�����ni_over_nt=1.0/ref_idx
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
		}
		//��������䣬���㷴��ϵ�������û�з��䣬��������ȫ���䣬����ϵ��Ϊ1
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			scattered = ray(rec.p, refracted);
		}
		else
		{
			scattered = ray(rec.p, reflected);
			return false;
		}
			/*����һ����0��1�������������������С�ڷ���ϵ����������Ϊ������ߣ���֮������Ϊ������ߡ�
		Ҳ����ֻ�з�����߻���������е�һ����Ϊʲô������˵�÷�����ߺ�������߶����𣿿��ǵ�һ�����ص㱻����Ϊ����100�Σ�
		��100���з�����ߵ�����������reflect_prob��ֵ����أ����ԣ�100�ε�ƽ��ֵҲ���Ǹ����ص㷴����ߺ�������ߵĵ���*/
		return true;
	}
};
