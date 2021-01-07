#ifndef __LAMBERT93PROJ_H__ // avoid cyclic inclusions
#define __LAMBERT93PROJ_H__

// define libraries
#include <string>
#include <vector>
#include "proj.h"

// perform the lambert93 map projection
class Lambert93Proj
{
    public:
        Lambert93Proj(); // default constructor
        Lambert93Proj(std::string wgs84CoordFilePath); // parameterized constructor
        ~Lambert93Proj(); // destructor
        std::string getWgs84CoordFilePath() const; // getter (read m_wgs84CoordFilePath)
        void setWgs84CoordFilePath(std::string wgs84CoordFilePath); // setter (write m_wgs84CoordFilePath)
        std::vector<PJ_XYZ> getCartCoords() const; // getter (read cartCoords)
        void project(); // projection method from wgs84 to lambert93
        
    private:
        std::string m_wgs84CoordFilePath = ""; // path to wgs84 coordinates file
        std::vector<PJ_XYZ> cartCoords; // projected cartesian coordinates
        
        PJ_CONTEXT *m_context = NULL; // context of the current projection
        PJ *m_projection = NULL; // projection map between 2 coordinate systems
        // path to proj.db file
        const char* m_proj_dbPath = "../lib/proj/share/proj/proj.db";
        // define projection coordinate systems
        // source coordinate systems
        const char *sourceCrs = "+proj=longlat +datum=WGS84 +no_defs"; /*or EPSG:4326*/
        // target coordinate systems
        const char *targetCrs = "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs"; /*or EPSG:2154*/
    
    };
#endif