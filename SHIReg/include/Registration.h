#ifndef REGISTRATION_H
#define REGISTRATION_H
#include "PointCloudProcessor.h"

using namespace std;
using namespace pcl;
using namespace Eigen;

#ifndef Registration_API
#ifdef REGILIB_EXPORTS
#define Registration_API __declspec(dllexport)
#else
#define Registration_API __declspec(dllimport)
#endif
#endif

class Registration_API RegistrationModule{
public :
	RegistrationModule(){}
	RegistrationModule(int m_neghborcnt, double registration_threshold, double filteringSize);
	//Registration For Automatic Registration
	void automatedRegistration(vector<PointCloudProcessor> &PCS, Matrix4d &mat_reverse);
	void CombinePair(vector<sphere> spheres1, vector<sphere> spheres2, Matrix4d &mat, Vector3d &err);

	//Registration for Manual Registration
	void manualRegistration(PointCloudProcessor &base, PointCloudProcessor &target, Matrix4d &mat_reverse, bool manualMode);
	void manualRegistration2(PointCloudProcessor &base, PointCloudProcessor &target, Matrix4d &mat_reverse);
	void PlaneBasedRegistration(vector<Vector3d> Base, vector<Vector3d> Target, Matrix4d &mat, double &err_reg);
	void PlaneBasedRegistration2(PointCloud<PointXYZ> neighbor1[3], PointCloud<PointXYZ> neighbor2[3], Matrix4d &transformMat);

	//Reduce error using Orthogonal Projection
	int ReduceError(PointCloudProcessor &base, PointCloudProcessor &target, Matrix4d &transformMatF, PointCloud<PointXYZ> &overlap);
	void findOverlap(PointCloudProcessor &base, PointCloudProcessor &target, PointCloud<PointXYZ> &overlap, EP &present_status);
	void findOverlap2(PointCloud<PointXYZ>::Ptr base_pts, PointCloud<PointXYZ>::Ptr target_pts, vector<int> &overlap1, vector<int> &overlap2, EP &present_status);
	void P2PRegistration(PointCloud<PointSurfel>::Ptr base, PointCloud<PointXYZ> &target, Matrix4d &transformMat, double pre_err);
	void setThreshold(double m_threshold);
	void printStatus(EP status);
	void setFiltering_size(double m_size);
	void setCount(int m_cnt);
	void setNeighborCount(int m_cnt);

	//Reduce Error process
	void reduceError(PointCloudProcessor &base, PointCloudProcessor &target, Matrix4d &mat_reverse, vector<Vector3d> &overlapArea);
	void reduceError2(PointCloudProcessor &integratedPCS, PointCloudProcessor &target, Matrix4d &mat_reverse);
	void overlapSearch(PointCloudProcessor &base, PointCloudProcessor &target, Matrix4d &mat_reverse, vector<Vector3d> &overlapArea);
	
	//Merge Clouds
	void mergeClouds(PointCloudProcessor &integratedPCS, PointCloudProcessor &target, Matrix4d &mat_reverse);
	void mergeCloud(PointCloudProcessor &integratedPCS, PointCloudProcessor &target, Matrix4d &mat_reverse);
	void refine_merge(PointCloudProcessor &integratedPCS, vector<PointCloudProcessor> &PCS, Matrix4d &mat_reverse);
private:
	//Iteractive Point Closest
	c_pair *Make_Node(double First[3],double Base[3],double dist);
	c_pair* Make_List_Of_Pair(int n_set);
	void Free_List(c_pair *Head);
	void Assign_Points(c_pair* Current,double Points[3],double Min_Pair[3],double Min_Dist);
	double Find_Min_Dist_Pair(double Points[3],double Min_Pair[3],int n_base,double base[NUM_OF_DATA][3]);
	void Find_Closest_Pair_List(double set1[NUM_OF_DATA][3],int n_set1,double base[NUM_OF_DATA][3],int n_base,double MuP[3],double MuX[3],c_pair* Head);
	void Find_Closest_Pair_List2(double set1[NUM_OF_DATA][3],int n_set1,double base[NUM_OF_DATA][3],int n_base,double MuP[3],double MuX[3],c_pair* Head);
	void Find_Closest_Pair_List_p2p(vector<Vector3d> set1,int n_set1,vector<Vector3d> base,int n_base,double MuP[3],double MuX[3],c_pair* Head);
	void Calc_Covariance(double Covariance[3][3],c_pair *Head,double MuP[3],double MuX[3]);
	void Calc_Q(double Q[4][4],double Covariance[3][3]);
	void Rotation_Matrix(double Rot[3][3], double quaternion[4]);
	void jacobi(double a[5][5], int n, double d[5], double v[5][5], int *nrot);
	void eigsrt(double d[5], double v[5][5], int n);
	void Make_Translation_Vector(double qt[3],double Rot[3][3],double MuP[3],double MuX[3]);
	void Update(double Rot[3][3],double p[3],c_pair *Head);
	double Assign_Check_Final(double set1[NUM_OF_DATA][3],c_pair *Head);
	double Assign_Check_Final_p2p(vector<Vector3d> &set1,c_pair *Head);
	void Out_Trans_Rot(double Rot[3][3],double qt[3]);
	double local_reg(double base[NUM_OF_DATA][3],int n_base,double set1[NUM_OF_DATA][3],int n_set1);
	double local_reg_p2p(vector<Vector3d> &base, vector<Vector3d> &set1, double threshold);
	void Update_Point(double Rot[3][3],double p[3],double set[NUM_OF_DATA][3],int nset);
	
	//Registration For Automatic Registration
	double findCorresfondingSet(vector<Vector3d> &D, Vector4d &sp);
	void findCorresfondingSet2(vector<Vector3d> &D, Vector3d norm, Vector3d planePt);
	void modelBasedRegistration(vector<Vector3d> sphere1[3], vector<Vector3d> sphere2[3], MatrixXd &R2, Vector3d &err);
	void initialRegistration(vector<sphere> spheres1, vector<sphere> spheres2, vector<int> &index1, vector<int> &index2, MatrixXd &R, Vector3d &err);

	//Variable to save matrix
	double sRot[3][3], sqt[3];
	double target_threshold;
	double distance_threshold;
	double overlap_threshold;
	double filtering_size;
	int cnt;
	int neighbor_cnt;
};
#endif