// define libraries
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "proj.h"
#include "gdal_alg.h"
#include "lambert93Proj.h"
#include "gridCreation.h"
#include "imageCreation.h"

int main(int argc, const char *argv[]){
    std::string wgs84CoordFilePath = argv[1]; // accept input file name
    std::vector<PJ_XYZ> cartCoords; // define cartesian coordinates vector

    try{
        Lambert93Proj lamProj(wgs84CoordFilePath); // define a Lambert93Proj object
        lamProj.project(); // perform lambert93 projection
        cartCoords = lamProj.getCartCoords(); // store cartesian coordinates

    }
    catch(const char* msg){
        std::cerr<<msg<<std::endl; // handle errors
        return 1;
    }
    
    // if no projection was done, cartCoords variable will still empty, if so raise an error message
    if(cartCoords.empty()){
        std::cerr<<"No projection was done"<<std::endl;
        return 1;
    }
    // else continue our pipline
    else{
        int gridWidth;
        std::stringstream intArg(argv[2]); // accept image width in pixels
        intArg>>gridWidth; // store image width into gridWidth varible
        GridCreation grid(gridWidth); // define a GridCreation object
        double* gridPoints = NULL; // define grid points container

        try{
            grid.create(cartCoords); // create a grid
            gridPoints = grid.getGridPoints(); // store grid points
        }
        catch(const char* msg){ // handle errors
            std::cerr<<msg<<std::endl;
            return 1;
        }
        // if no grid calculation was done, the gridPoints variable will still pointing to NULL, if so raise an error message
        if(gridPoints == NULL){
            std::cerr<<"Griding doesn't work properly"<<std::endl;
        }
        // else continue our pipline
        else{
            // store grid information (lowest and highst elevation and the value corresponding to no data)
            double zMin = grid.getZMin();
            double zMax = grid.getZMax();
            double dfNoDataValue = grid.getDfNoDataValue();

        	ImageCreation img("output_img", gridWidth); // define an ImageCreation object
            img.create(gridPoints, zMin, zMax, dfNoDataValue); // create our images
        }
    }
}