#ifndef __GRIDCREATION_H__ // avoid cyclic inclusions
#define __GRIDCREATION_H__

// define libraries
#include <vector>
#include "proj.h"

// create the grid points
class GridCreation
{
    public:
        GridCreation(); // default constructor
        GridCreation(int gridWidth); // parameterized constructor
        ~GridCreation(); // destructor
        int getGridWidth() const; // getter (read m_gridWidth)
        void setGridWidth(int grid); // setter (write m_gridWidth)
        double* getGridPoints() const; // getter (read gridPoints)
        double getZMin() const; // getter (read zMin)
        double getZMax() const; // getter (read zMax)
        double getDfNoDataValue() const; // getter (read dfNoDataValue)
        int create(std::vector<PJ_XYZ> &cartCoords); // grid creation method
        
    private:
        int m_gridWidth = 0; // grid width
        double* gridPoints = NULL; // grid points
        double zMin = 0., zMax = 0.; // lowest and highst elevation values
        const double dfNoDataValue = -9999; // no data value
    };
#endif