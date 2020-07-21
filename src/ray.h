#pragma once
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;


//�ڵ�λ���������������������
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
	RowVector3d origin() const { return A; }		//��Դ
	RowVector3d direction() const { return B; }		//����
	RowVector3d point_at_parameter(double t) const { return A + t * B; }//��ͬ��С��t����Դﵽ��ͬ�ط�

};
