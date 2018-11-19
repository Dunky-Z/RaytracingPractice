#pragma once

#include "ray.h"

class material;

struct hit_record
{
	double t;
	RowVector3d p;
	RowVector3d normal;
	material *mat_ptr;
};/*�ýṹ���¼��ײ�㡱������Ϣ����������ľ���t��ײ�����������p��ײ����ķ�����normal��*/

class hitable
{/*hitable������ʾ�ܹ�������ײ�ϵ��κ����塣���磬����*/
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
	/*hit()�ڴ˱�����Ϊ�麯������hitableΪ�����ࡣ������������б���ʵ�����麯��*/
};