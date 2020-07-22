#pragma once
#include "hitable.h"

class sphere : public hitable
{
public:
	double radius;		//球半径
	sphere() {}
	material *ma;		//材质信息
	sphere(RowVector3d cen, double r, material *m) : center(cen), radius(r), ma(m) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	RowVector3d center;	//球心坐标
};


/*!
*@brief  撞击检测
*@param[out] 
*@param[in]  const ray & r  光线信息
*@param[in]  double t_min  方程根最小值
*@param[in]  double t_max  方程根最大值
*@param[in]  hit_record & rec  
*@return     bool  
*/
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	RowVector3d oc = r.origin() - center;			//球心到光起始点的向量
	double a = (r.direction()).dot(r.direction());	//dot(B,B)
	double b = oc.dot(r.direction());				//dot(B,A-c)
	double c = oc.dot(oc) - radius * radius;		//dot(A-C,A-C)-R*R
	double discriminant = b * b - a * c;			//判别式，本来b是2*oc.dot，为了简化就都同时去掉4
	if (discriminant > 0)
	{
		double temp = (-b - sqrt(discriminant)) / a;//判别式大于0光线与物体有交点求出交点，并且记录交点的信息
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = ma;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = ma;
			return true;
		}
	}
	return false;		//判别式小于0，没有交点
}