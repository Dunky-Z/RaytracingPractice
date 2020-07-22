#pragma once

#include "ray.h"

class material;


struct hit_record
{
	double t;			//��������ľ���
	RowVector3d p;		//ײ���������
	RowVector3d normal;	//ײ����ķ�����
	material *mat_ptr;	//������Ϣ
};

class hitable
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};