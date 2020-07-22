
//记录击中所有的物体，也是继承hitable类，实现hit方法，去找出最近的物体。

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
*@brief  撞击检测，如果被光线撞到就将其信息保存
*@param[out] 
*@param[in]  const ray & r  光线信息
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
	//逐一判断列表中所有物体是否被光线撞到，如果被撞到，将撞击点信息存入hit_record
	//如果当前撞点在范围内，则将当前撞点的距离设置为范围的最大值。
	//也就是后面只考虑比该撞点更近的撞点。趋势是：找到的撞点是越来越近的，最终找到最近的撞点。
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


