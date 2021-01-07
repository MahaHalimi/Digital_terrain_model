#ifndef __IMAGECREATION_H__ // avoid cyclic inclusions
#define __IMAGECREATION_H__

// define libraries
#include <vector>
#include <string>

// create grey-level and colored images
class ImageCreation
{
    public:
        ImageCreation(); // default constructor
        ImageCreation(std::string imgName, int imgWidth); // parameterized constructor
        ~ImageCreation(); // destructor
        std::string getImgName() const; // getter (read m_imgName)
        void setImgName(std::string imgName); // setter (write m_imgName)
        int getImgWidth() const; // getter (read m_imgWidth)
        void setImgWidth(int imgWidth); // setter (write m_imgWidth)
        // image creation method
        void create(double* &gridPoints, double zMin, double zMax, double dfNoDataValue);
        
    private:
        // private method to interpolate grid points to grey level pixels
        std::vector<unsigned int> interpolateGreyLevel(double* &gridPoints, double zMin, double zMax, double dfNoDataValue);
        // private method to interpolate grid points to colored pixels
        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> interpolateHaxby(double* &gridPoints, double zMin, double zMax, double dfNoDataValue);
        int m_imgWidth = 0; // image width
        std::string m_imgName = ""; // image name
    };
#endif