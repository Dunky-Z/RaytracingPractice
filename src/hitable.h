#pragma once

#include "ray.h"

class material;


struct hit_record
{
	double t;			//离光线起点的距离
	RowVector3d p;		//撞击点的坐标
	RowVector3d normal;	//撞击点的法向量
	material *mat_ptr;	//材质信息
};

class hitable
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};