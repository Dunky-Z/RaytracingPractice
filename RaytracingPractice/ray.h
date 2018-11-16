#pragma once
#ifndef RAY_H
#define RAY_H
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

class ray
{
public:
	RowVector3d A;
	RowVector3d B;
	ray() {}
	ray(const RowVector3d a, const RowVector3d b) { A = a; B = b; }
	RowVector3d origin() const { return A; }		//��Դ
	RowVector3d direction() const { return B; }		//����
	RowVector3d point_at_parameter(double t) const { return A + t * B; }//��ͬ��С��t����Դﵽ��ͬ�ط�

};
#endif // RAY_H