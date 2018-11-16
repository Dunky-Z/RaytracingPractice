#pragma once
#include "hitable.h"

class sphere : public hitable
{
public:
	double radius;		//��뾶
	sphere() {}
	sphere(RowVector3d cen, double r) : center(cen), radius(r) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	RowVector3d center;
};


bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	RowVector3d oc = r.origin() - center;			//���ĵ�����ʼ�������
	double a = r.direction().dot(r.direction());	//dot(B,B)
	double b = oc.dot(r.direction());				//dot(B,A-c)
	double c = oc.dot(oc) - radius * radius;		//dot(A-C,A-C)-R*R
	double discriminant = b * b - a * c;			//�б�ʽ������b��2*oc.dot��Ϊ�˼򻯾Ͷ�ͬʱȥ��4
	if (discriminant > 0)
	{
		double temp = (-b - sqrt(discriminant)) / a;//�б�ʽ����0�����������н���������㣬���Ҽ�¼�������Ϣ
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;		//�б�ʽС��0��û�н���
}