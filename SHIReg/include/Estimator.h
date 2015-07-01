#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET 

//header file for PCL
#undef min
#undef max
#include <pcl/point_types.h>
#include <pcl/point_representation.h>
#include <pcl/features/normal_3d.h>
#include <pcl/registration/icp.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/sample_consensus/sac_model_sphere.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/keypoints/iss_3d.h>
#include <pcl/keypoints/uniform_sampling.h>
#include <pcl/common/intersections.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/io/pcd_io.h>
#include <pcl/common/time.h>

//header for STL
//#include <Windows.h>
#include <MMSystem.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

//header for TBB
#include <tbb/tbb.h>
#include <tbb/blocked_range.h>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <tbb/partitioner.h>

//header for OpenGL
//#include <GL/glew.h>
//#include <GL/glut.h>

//header for KDTree
#include <nabo/nabo.h>

#define NUM_OF_DATA 4000
#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);a[k][l]=h+s*(g-h*tau);
#define TRUE 1
#define FALSE 0
#define EPS 0.001

using namespace Eigen;
using namespace std;
using namespace pcl;

#ifndef Registration_API
#ifdef REGILIB_EXPORTS
#define Registration_API __declspec(dllexport)
#else
#define Registration_API __declspec(dllimport)
#endif
#endif

struct Registration_API curve{
	Vector3d tangent, p0, p1, p2, a0, a1, a2, b, t, m;
	double angle;
	double u;
	double curvature;
	double NormalCurvature;
};

struct Registration_API sphere{
	PointCloud<PointXYZ> sph_points;
	double m_radius;
	Vector3d center;
	int count;
};

struct Registration_API CompFunctorVector {
	CompFunctorVector( vector<Vector4d> &a ): _data(a) {  }
	bool operator()( Vector4d &first, Vector4d &second ) const {
		return first.w() < second.w();
	}
private:
	vector<Vector4d> &_data;
};

class Registration_API distanceCalculator{
public :
	vector<Vector4d>* const m_pts;
	vector<Vector3d>* const m_data;
	Vector3d* const m_ori;
	void operator()(tbb::blocked_range<int> &r)const{
		vector<Vector4d>* pts = m_pts;
		vector<Vector3d>* data = m_data;
		Vector3d* ori = m_ori;
		for(int i = r.begin(); i < r.end();i++){
			Vector3d p = (*data)[i];
			double length = ((*ori) - p).norm();
			(*pts)[i].x() = (*data)[i].x();
			(*pts)[i].y() = (*data)[i].y();
			(*pts)[i].z() = (*data)[i].z();
			(*pts)[i].w() = length;
		}
	}
	distanceCalculator(vector<Vector4d> &pts, vector<Vector3d> &data, Vector3d &ori):m_pts(&pts), m_data(&data), m_ori(&ori){}
};

struct Registration_API c_pair {
	double First[3];
	double Base[3];
	double dist;
	c_pair *next;
};

struct Registration_API EP{
	double total_err, average_err, variance, standard_deviation, accepted_ratio;
	double whole_total_err, whole_average_err, whole_variance, whole_standard_deviation;
	int accepted_pt, rejected_pt;
	double distance_threshold, minimum_err, maximum_err;
};

Registration_API double distf(double a[3],double b[3]);
Registration_API double areaTr(double v1[3],double v2[3],double v3[3]);
Registration_API Vector4d xyzToVec4d(PointXYZ& p);
Registration_API Vector3d xyzToVec3d(PointXYZ& p);
Registration_API Vector3d xyzToVec3d(PointSurfel& p);
Registration_API Vector4f Vector4dToVector4f(Vector4d& p);
Registration_API Vector3f Vector3dToVector3f(Vector3d& p);
Registration_API void flipNormal(Vector3d& pt, Vector3d &normal);
Registration_API void convertMatrix3fToMatrix3d(Matrix3f& from, Matrix3d &to);
Registration_API void flipNormal(Vector3d pt, double &normal_x, double &normal_y, double &normal_z);
Registration_API double sqrPointToLineDistance2 (const Eigen::Vector4d &pt, const Eigen::Vector4d &line_pt, const Eigen::Vector4d &line_dir);
Registration_API int compute_sphere_parameter(vector<Vector3d> samples, int no_samples, Vector4d &model);
Registration_API vector<int> ransacSphere(PointCloud<PointXYZ>::Ptr pts, double threshold_);
Registration_API VectorXf ransacPlane(PointCloud<PointXYZ> &pts, double threshold_);
Registration_API bool find_in_samples (PointCloud<PointXYZ>::Ptr pts, vector<Vector3d> samples, int no_sample);
Registration_API void  get_samples(PointCloud<PointXYZ>::Ptr pts, vector<Vector3d> &samples, int no_samples);
Registration_API double compute_distance(Vector3d &estimated_model, Vector3d pt);
Registration_API void model_verification (PointCloud<PointXYZ>::Ptr pts, vector<Vector3d> &inliers, Vector4d &estimated_model);
Registration_API Vector4d SphereEstimation(PointCloud<PointXYZ>::Ptr pts);
Registration_API void planeEstimation(PointCloud<PointXYZ> &pts, Vector4d &planeParameter, Vector3d &n1, Vector3d &n2, Vector3d &xyz_centroid);
Registration_API void planeEstimation(vector<Vector3d> pts, Vector4d &planeParameter, Vector3d &n1, Vector3d &n2, Vector3d &xyz_centroid);
Registration_API Vector4d planeEstimation(PointCloud<PointXYZ> &pts);
Registration_API void removeDuplicatedVertices(PointCloud<PointXYZ> &pts, PointCloud<PointXYZ> &result);
Registration_API void verticesDeduplication(vector<int> pts, vector<int> &result);
Registration_API void VoxelFiltering(PointCloud<PointXYZ>::Ptr pts_, PointCloud<PointXYZ>::Ptr result, double d_size);
Registration_API void VoxelFiltering(vector<Vector3d> pts, vector<Vector3d> &result_, double d_size);
Registration_API void VoxelFiltering(PointCloud<PointXYZ> &pts, PointCloud<PointXYZ> &result, double d_size);
Registration_API Vector3d calculatePoint(Vector3d surface1, Vector3d surface2, Vector3d target);
Registration_API double orthogonalProjection(Vector3d surface1, Vector3d surface2, Vector3d P, Vector3d &result);
Registration_API void globalTolocal(PointCloud<PointXYZ> &pts, PointXYZ &ori, Matrix3d &R);
Registration_API void globalTolocal(vector<Vector3d> &pts, Vector3d ori, Matrix3d R);
Registration_API void calculateSurface(PointCloud<PointXYZ> &pts, Vector3d &model, Vector3d &model2);
Registration_API void calculateSurface(vector<Vector3d> pts, Vector3d &model, Vector3d &model2);
Registration_API void surfaceEstimator(PointCloud<PointXYZ> &sp1, PointCloud<PointXYZ> &sp2,PointXYZ &pt, Vector3d &s1, Vector3d &s2, Matrix3d &Om);
Registration_API void surfaceEstimator(vector<Vector3d> sp1, vector<Vector3d> &sp2,Vector3d pt, Vector3d &s1, Vector3d &s2, Matrix3d &Om);
Registration_API void getIntersectionPoint( Vector4d &plane1, Vector4d &plane2, Vector4d &plane3, Vector3d &interpoint );
template <typename Iterator> bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
	if ((first == last) || (first == k) || (last == k))
		return false;
	Iterator itr1 = first;
	Iterator itr2 = last;
	++itr1;
	if (last == itr1)
		return false;
	itr1 = last;
	--itr1;
	itr1 = k;
	--itr2;
	while (first != itr1)
	{
		if (*--itr1 < *itr2)
		{
			Iterator j = k;
			while (!(*itr1 < *j)) ++j;
			iter_swap(itr1,j);
			++itr1;
			++j;
			itr2 = k;
			rotate(itr1,j,last);
			while (last != j)
			{
				++j;
				++itr2;
			}
			rotate(k,itr2,last);
			return true;
		}
	}
	rotate(first,k,last);
	return false;
}

#endif