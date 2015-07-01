#ifndef ACCELERATOR_H
#define ACCELERATOR_H
#include "Estimator.h"

#ifndef Registration_API 
#ifdef REGILIB_EXPORTS
#define Registration_API  __declspec(dllexport)
#else
#define Registration_API  __declspec(dllimport)
#endif
#endif

using namespace std;
using namespace pcl;
using namespace Eigen;
using namespace Nabo;

class Registration_API planeVerifier{
public :
	void quadricCurveGenerator(Vector3d pt, vector<Vector3d> neighbors, vector<curve> &curves);
	void normalEstimator(vector<curve> curves, Vector3d &estimated_vector);
	bool filtering(vector<curve> curves, Vector3d normal);
};

class Registration_API KNNSearch{
public :
	NNSearchF* m_nns;
	MatrixXi* const m_indices;
	MatrixXf* const m_dists;
	MatrixXf* const m_pts;
	int* const m_size;
	void operator()(const tbb::blocked_range<int>& r )const;
	KNNSearch(NNSearchF *nns, MatrixXi &indices, MatrixXf &dists, MatrixXf &pts, int &size):m_nns(nns), m_indices(&indices), m_dists(&dists), m_pts(&pts), m_size(&size){}
};

class Registration_API PointCalculation{
public :
	planeVerifier* verifier;
	MatrixXi* const m_indices;
	PointCloud<PointXYZ>::Ptr const m_pts;
	vector<bool>* const m_filtering;
	PointCloud<PointSurfel>::Ptr const m_surf;
	void operator()(const tbb::blocked_range<int>& r )const;
	PointCalculation(PointCloud<PointXYZ>::Ptr pts,PointCloud<PointSurfel>::Ptr model_surf, MatrixXi &indices, vector<bool> &filter_results)
		:m_pts(pts), m_surf(model_surf), m_indices(&indices), m_filtering(&filter_results){}
};

class Registration_API lineDistance{
public:
	PointCloud<PointSurfel>::Ptr m_pts;
	Vector4d * m_origin;
	Vector4d * m_direction;
	double * m_distance;
	double * m_t;
	int* m_index;
	void operator()( const tbb::blocked_range<int>& r );
	lineDistance( lineDistance& x, tbb::split) : m_pts(x.m_pts), m_origin(x.m_origin), m_direction(x.m_direction), m_distance(x.m_distance), m_index(x.m_index), m_t(x.m_t){}
	void join( const lineDistance& y );
	lineDistance(PointCloud<PointSurfel>::Ptr pts, Vector4d &origin, Vector4d &direction, double &distance, int &index, double &t):
	m_pts(pts), m_origin(&origin), m_direction(&direction), m_distance(&distance), m_index(&index), m_t(&t){}

};

class Registration_API OverlapFinder{
public :
	MatrixXf* const m_M1;
	MatrixXf* const m_M2;
	MatrixXi* const m_indices1;
	MatrixXi* const m_indices2;
	double* const m_distance_threshold;
	vector<bool>* const m_accept;
	vector<double>* const m_errList;
	int* const m_nn;
	void operator()(const tbb::blocked_range<int>& r )const;
	OverlapFinder(MatrixXf &M1, MatrixXf &M2, MatrixXi &indices1, MatrixXi &indices2, vector<double> &errList, double &distance_threshold, int &nn, vector<bool> &accept):
	m_M1(&M1), m_M2(&M2),m_indices1(&indices1), m_indices2(&indices2), m_errList(&errList), m_distance_threshold(&distance_threshold), m_nn(&nn), m_accept(&accept){}
};
#endif