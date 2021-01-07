// define libraries
#include <vector>
#include <algorithm>
#include "proj.h"
#include "gdal_alg.h"
#include "gridCreation.h"

// default constructor
GridCreation::GridCreation(){
    // define the default values for class attributs
    gridPoints = NULL;
    zMin = zMax = 0.;
}
// parameterized constructor
GridCreation::GridCreation(int gridWidth){
    m_gridWidth = gridWidth;
    gridPoints = new double[m_gridWidth*m_gridWidth];
    memset(gridPoints, 0, sizeof(double)*m_gridWidth*m_gridWidth);
    zMin = zMax = 0.;
}
// destructor
GridCreation::~GridCreation(){}
// getter (read m_gridWidth)
int GridCreation::getGridWidth() const{
    return m_gridWidth;
}
// setter (write m_gridWidth)
void GridCreation::setGridWidth(int gridWidth){
    m_gridWidth = gridWidth;
    gridPoints = new double[m_gridWidth*m_gridWidth];
    memset(gridPoints, 0, sizeof(double)*m_gridWidth*m_gridWidth);
}
// getter (read gridPoints)
double* GridCreation::getGridPoints() const{
    return gridPoints;
}
// getter (read zMin)
double GridCreation::getZMin() const{
    return zMin;
}
// getter (read zMax)
double GridCreation::getZMax() const{
    return zMax;
}
// getter (read dfNoDataValue)
double GridCreation::getDfNoDataValue() const{
    return dfNoDataValue;
}
// grid creation method
int GridCreation::create(std::vector<PJ_XYZ> &cartCoords){
    // define the grid data points number
    int pointsNb = cartCoords.size();

    // define 3 vectors containing respectively x,y and z cartesian coordinates
    std::vector<double> xPoints;
    std::vector<double> yPoints;
    std::vector<double> zPoints;

    // fill xPoints, yPoints and zPoints variables with their coressponding values from cartCoords variable
	for (int i = 0; i < pointsNb; i++){
        xPoints.push_back(cartCoords[i].x);
        yPoints.push_back(cartCoords[i].y);
        zPoints.push_back(cartCoords[i].z);
    }
    // search for the lowest and highst values of x, y cartesian coordinates
	std::vector<double>::iterator it = std::min_element(xPoints.begin(), xPoints.end());
	double  xMin = *it;
	it = std::max_element(xPoints.begin(), xPoints.end());
	double  xMax = *it;

	it = std::min_element(yPoints.begin(), yPoints.end());
	double  yMin = *it;
	it = std::max_element(yPoints.begin(), yPoints.end());
	double  yMax = *it;

    // configure Delaunay triangulation
	GDALGridLinearOptions *poOptions = new GDALGridLinearOptions();
	// define a no data marker to fill empty points
    poOptions->dfNoDataValue = dfNoDataValue;
    // define a redius to search a nearest neighbour in case a point to be interpolated
    // does not fit into a triangle of Delaunay triangulation
	poOptions->dfRadius = 0;
    // GGA_NearestNeighbor
    // grid points calculation using Delaunay triangulation
	int err = GDALGridCreate(GGA_Linear, poOptions, pointsNb, &(xPoints[0]), &(yPoints[0]), &(zPoints[0]),\
			xMin, xMax, yMin, yMax, m_gridWidth, m_gridWidth, GDT_Float64, gridPoints, NULL, NULL);
    
    if(err){
        throw "Grid creation faill!"; // raise an error if the grid points calculation doesn't perform well
    }

    // calculate the lowest and highst elevation values
    std::vector<double> gridPointsVect(gridPoints, gridPoints+m_gridWidth*m_gridWidth);
    gridPointsVect.erase(std::remove(gridPointsVect.begin(), gridPointsVect.end(), poOptions->dfNoDataValue), gridPointsVect.end());

    it = std::min_element(gridPointsVect.begin(), gridPointsVect.end());
	zMin = *it;
	it = std::max_element(gridPointsVect.begin(), gridPointsVect.end());
	zMax = *it;
}