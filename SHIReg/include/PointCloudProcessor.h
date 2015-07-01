#ifndef POINTCLOUDPROCESSOR_H
#define POINTCLOUDPROCESSOR_H
#include "Accelerator.h"

using namespace std;
using namespace pcl;
using namespace Eigen;
using namespace Nabo;

#ifndef Registration_API
#ifdef REGILIB_EXPORTS
#define Registration_API __declspec(dllexport)
#else
#define Registration_API __declspec(dllimport)
#endif
#endif

class Registration_API PointCloudProcessor
{
public:
	void destoryDetector();
	void setFilename(char* file);
	void setDuplicationRemoval(int remove);
	void fileLoading();
	void saveFile();
	void calculateProperties();
	void sphereEstimation();
	void setSelectedPlane(Vector4d &m_plane);
	void removeSelection();
	void subsampling(double size);

	////////////////////////////////////////////////////////////////////////////////////////////////
	// SHI 제거
	////////////////////////////////////////////////////////////////////////////////////////////////
	//void drawPoint();
	//void drawsubSampled();
	//void drawNormal();
	//void drawFiletedPoint();
	//void drawSpherePoint();
	//void drawExtractedPlane(bool drawPlane);
	//void drawNeighbor();
	//void drawOverlapRegion();
	//void drawKeypoints();
	///////////////////////////////////////////////////////////////////////////////////////////////

	void setTransformMatrix(Matrix4d &m_mat);
	void applyTransform();
	void setRadius(double radius);
	void setSelectedPt(PointSurfel &pt);
	void makeCoordinate();
	void setFilteringsize(double m_size);
	int get_datasize();
	void set_datasize(int size);
	void find3DKeypoint();
	void filterHarrisKeypoints();
	void merge(PointCloud<PointSurfel>::Ptr s_, vector<int> &overlap);
	void addPointsCloud(PointCloud<PointSurfel>::Ptr s_);
	
	vector<sphere> getSphere();
	PointSurfel getClosestPoint(Vector4d &Ray_ori, Vector4d &Ray_dir, int &index, double &distance, double &t);
	PointCloud<PointXYZ> getNeighbor(PointXYZ &pt, int nn);
	vector<Vector3d> getselectedPts();
	vector<Vector4d> getSelectedPlane();
	Vector3d getCoordinate();
	vector<Vector3d> getBasis();
	PointCloud<PointSurfel>::Ptr getPointCloud();
	PointCloud<PointSurfel>::Ptr getSamplingCloud();
	void resetCloud();
	double getLength();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline vector<bool> getFilter_results() { return filter_results; }                                       //shi 추가
	void cloudLoading(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud);                                            //shi 추가
	pcl::PointXYZ getBBMin() { return b_min; }                                                               //shi 추가
	pcl::PointXYZ getBBMax() { return b_max; }                                                               //shi 추가


private:
	PointCloud<PointSurfel>::Ptr normal_cloud, sampling_cloud;
	PointCloud<PointXYZ>::Ptr model_points;
	//PointCloud<PointXYZ>::Ptr keypoints3D;
	vector<Vector3d> selectedPts;
	vector<Vector4d> selectedPlanes;
	Vector3d selectedCoordinate;
	vector<Vector3d> selectedBasis;
	vector<sphere> spheres;
	vector<bool> filter_results;	
	//vector<int> filter_results;
	string fileName;
	bool removeDuplication;
	int data_size;
	double m_radius, m_filtering_size;
	Matrix4d mat;
	PointXYZ b_min, b_max;
};
#endif