
//��¼�������е����壬Ҳ�Ǽ̳�hitable�࣬ʵ��hit������ȥ�ҳ���������塣

#pragma once
#include "hitable.h"

class hitable_list : public hitable
{
public:
	hitable_list() {}
	hitable **list;
	int list_size;
	hitable_list(hitable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

};


/*!
*@brief  ײ����⣬���������ײ���ͽ�����Ϣ����
*@param[out] 
*@param[in]  const ray & r  ������Ϣ
*@param[in]  double t_min  
*@param[in]  double t_max  
*@param[in]  hit_record & rec  
*@return     bool  
*/
bool hitable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	//��һ�ж��б������������Ƿ񱻹���ײ���������ײ������ײ������Ϣ����hit_record
	//�����ǰײ���ڷ�Χ�ڣ��򽫵�ǰײ��ľ�������Ϊ��Χ�����ֵ��
	//Ҳ���Ǻ���ֻ���Ǳȸ�ײ�������ײ�㡣�����ǣ��ҵ���ײ����Խ��Խ���ģ������ҵ������ײ�㡣
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

