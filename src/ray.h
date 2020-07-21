#pragma once
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;


//在单位球体内生成随机方向向量
RowVector3d random_in_unit_sphere()
{
	RowVector3d p;
	do
	{
		p = 2.0*RowVector3d(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX)) - RowVector3d(1, 1, 1);
	} while (p.squaredNorm() >= 1.0);
	return p;
}

class ray
{
public:
	RowVector3d A;
	RowVector3d B;
	ray() {}
	ray(const RowVector3d a, const RowVector3d b) { A = a; B = b; }
	RowVector3d origin() const { return A; }		//光源
	RowVector3d direction() const { return B; }		//方向
	RowVector3d point_at_parameter(double t) const { return A + t * B; }//不同大小的t光可以达到不同地方

};
