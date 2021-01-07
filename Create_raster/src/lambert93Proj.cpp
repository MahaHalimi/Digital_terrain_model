// define libraries
#include <fstream>
#include <string>
#include <vector>
#include "proj.h"
#include "lambert93Proj.h"

// default constructor
Lambert93Proj::Lambert93Proj(){
    // define the default values for class attributs
    m_wgs84CoordFilePath = "";
    m_context = NULL;
    m_projection = NULL;
}
// parameterized constructor
Lambert93Proj::Lambert93Proj(std::string wgs84CoordFilePath){
    m_wgs84CoordFilePath = wgs84CoordFilePath;

    m_context = proj_context_create(); // create new context
    // define contexte database
    bool b = proj_context_set_database_path(m_context, m_proj_dbPath, NULL, NULL);
    
    if(!b){
        throw "Invalid proj.db!"; // raise an error if the context database used is not valid
    }
    
    // define the projection map elements
    m_projection = proj_create_crs_to_crs(m_context, sourceCrs, targetCrs, NULL);
    
    if(m_projection==0){
        throw "Invalid projection!"; // raise an error if the projection map isn't properly defined
    }
    // normalize projection map
    m_projection = proj_normalize_for_visualization(m_context, m_projection);

    if(m_projection==0){
        throw "Invalid projection!";  // raise an error if the normalizition isn't properly performed
    }
}
// destructor
Lambert93Proj::~Lambert93Proj(){}
// getter (read_wgs84CoordFilePath)
std::string Lambert93Proj::getWgs84CoordFilePath() const{
    return m_wgs84CoordFilePath;
}
// setter (write m_wgs84CoordFilePath)
void Lambert93Proj::setWgs84CoordFilePath(std::string wgs84CoordFilePath){
    m_wgs84CoordFilePath = wgs84CoordFilePath;
}
// getter (read cartCoords)
std::vector<PJ_XYZ> Lambert93Proj::getCartCoords() const{
    return cartCoords;
}
// projection method from wgs84 to lambert93
void Lambert93Proj::project(){
    // open the data input
    std::ifstream wgs84CoordFile(m_wgs84CoordFilePath);
    double lat, longt, z;

    // extract latitude, longitude and elevation line by line
    while(wgs84CoordFile >> lat >> longt >> z){
        // define the input data point
        PJ_COORD wgs84Coord = proj_coord (longt, lat, z, 0);
        // project the input data point
        PJ_COORD outputCoord = proj_trans(m_projection, PJ_FWD, wgs84Coord);
        // store the cartesian projected points
        cartCoords.push_back(outputCoord.xyz);
    }
    // close the file
    wgs84CoordFile.close();

    // clean up
    proj_destroy(m_projection);
    proj_context_destroy(m_context);
}