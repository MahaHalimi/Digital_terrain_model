#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <math.h>
#include <string>
#include <fstream>
#include "imageCreation.h"

// default constructor
ImageCreation::ImageCreation(){
  // define the default values for class attributs
    m_imgName = "";
    m_imgWidth = 0;
}
// parameterized constructor
ImageCreation::ImageCreation(std::string imgName, int imgWidth){
    m_imgName = imgName;
    m_imgWidth = imgWidth;
}
// destructor
ImageCreation::~ImageCreation(){}
// getter (read m_imgName)
std::string ImageCreation::getImgName() const{
  return m_imgName;
}
// setter (write m_imgName)
void ImageCreation::setImgName(std::string imgName){
  m_imgName = imgName;
}
// getter (read m_imgWidth)
int ImageCreation::getImgWidth() const{
  return m_imgWidth;
}
// setter (write m_imgWidth)
void ImageCreation::setImgWidth(int imgWidth){
  m_imgWidth = imgWidth;
}
// image creation method
void ImageCreation::create(double* &gridPoints, double zMin, double zMax, double dfNoDataValue){
  // interpolate grid points to grey level pixels
  std::vector<unsigned int> greyPixelValues = interpolateGreyLevel(gridPoints, zMin, zMax, dfNoDataValue);
  // interpolate grid points to colored pixels
  std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> coloredPixelValues = \
  interpolateHaxby(gridPoints, zMin, zMax, dfNoDataValue);

  // define image width, height and pixel maximum grey level or rgb component value
  int width = m_imgWidth, height = m_imgWidth, maxValue = 255;

  // define image names
  std::string greyImg = m_imgName + "_PGM.pgm";
  std::string coloredImg = m_imgName + "_PPM.ppm";

/*------------------------- Create grey level image -------------------------*/
  
  // define a buffer used to create grey level image row
  char* lineGreyBuff = new char[width];
  
  // define image header
  std::ofstream imgPgm(greyImg, std::ios_base::out|std::ios_base::binary|std::ios_base::trunc);
  imgPgm<<"P5\n"<<width<<" "<<height<<"\n"<<maxValue<<"\n";

  // fill image pixels
  for (int i = 0; i < height; i++){	
    for (int j = 0; j < width; j++){
      lineGreyBuff[j] = greyPixelValues[j+i*width];
    }
    imgPgm.write(reinterpret_cast<const char*>(lineGreyBuff), width);
  }

  imgPgm<<std::flush;

/*------------------------- Create colored image -------------------------*/
  // define a buffer used to create grey level image row
  char* lineColorBuff = new char[width*3];

  // define image header
  std::ofstream imgPpm(coloredImg, std::ios_base::out|std::ios_base::binary|std::ios_base::trunc);
  imgPpm<<"P6\n"<<width<<" "<<height<<"\n"<<maxValue<<"\n";
  
  // fill image pixels
  for (int i = 0; i < height*width;){	
    for (int j = 0; j < 3*width; j+=3){
      lineColorBuff[j] = std::get<0>(coloredPixelValues[i]);
      lineColorBuff[j + 1] = std::get<1>(coloredPixelValues[i]);
      lineColorBuff[j + 2] = std::get<2>(coloredPixelValues[i]);
      i++;
    }
    
    imgPpm.write(reinterpret_cast<const char*>(lineColorBuff), width*3);
  }
  imgPpm<<std::flush;
}
// method to interpolate grid points to colored pixels
std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> ImageCreation::interpolateHaxby\
(double* &gridPoints, double zMin, double zMax, double dfNoDataValue){
  // define interpolated pixels container
  std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> coloredPixelValues;
  
  // define haxby color space
  std::vector<std::tuple<int, int, int>> haxbyColors;
  haxbyColors.push_back(std::make_tuple(37, 57, 175));
  haxbyColors.push_back(std::make_tuple(40, 127, 251));
  haxbyColors.push_back(std::make_tuple(50, 190, 255));
  haxbyColors.push_back(std::make_tuple(106, 235, 255));
  haxbyColors.push_back(std::make_tuple(138, 236, 174));
  haxbyColors.push_back(std::make_tuple(205, 255, 162));
  haxbyColors.push_back(std::make_tuple(240, 236, 121));
  haxbyColors.push_back(std::make_tuple(255, 189, 87));
  haxbyColors.push_back(std::make_tuple(255, 161, 68));
  haxbyColors.push_back(std::make_tuple(255, 186, 133));
  haxbyColors.push_back(std::make_tuple(255, 255, 255));

  // define the number of color used at haxby color space
  int colorNb = 11;
  // define elevation value corresponding to the 11 colors used at haxby color space
  std::vector<double> linearZValues(colorNb, 0.);
  // define a pixel value used to fill coloredPixelValues variable
  std::tuple<unsigned int, unsigned int, unsigned int> pixelValue;

  // define an iterator used to calculate an elevation position within linerZValues variable
  std::vector<double>::iterator iter;
  
  // define an elevation position within linearZValues variable
  int position = 0;
  // define rgb component of a pixel
  unsigned int red = 0, green = 0, blue = 0;
	// define zValue to calculate 11 elevation values between zMin and zMax for linearZValues variable
  double zValue = zMin;

  // fill linearZValue variable
	for (int i = 0; i < linearZValues.size(); i++){
		linearZValues[i] = zValue;
		zValue += (zMax-zMin)/(colorNb-1);
	}
  
  // interpolate elevation values
  for (int i = 0; i < m_imgWidth*m_imgWidth; i++){
    // return a black pixel for grid points with a value == dfNoDataValue
    if(gridPoints[i] == dfNoDataValue){
      red = green = blue = 0;
      pixelValue = std::make_tuple(red, green, blue);
    }
    else{
      // calculate the position of an elevation value just higher or equal
      // a given elevation value (gridPoints[i]) within LinearZValues variable 
      iter = std::lower_bound(linearZValues.begin(), linearZValues.end(), gridPoints[i]);  
      position = iter - linearZValues.begin();

      // if the elevation value we aim to interpolate is zMin, we return the corresponding color
      // this condition is made because it is a special case of our interpolation
      if(position == 0){
        // calculate rgb component of the current pixel
        red = std::get<0>(haxbyColors[position]);
        green = std::get<1>(haxbyColors[position]);
        blue = std::get<2>(haxbyColors[position]);
        pixelValue = std::make_tuple(red, green, blue);
      }
      else{
        double DeltaX = linearZValues[position]-linearZValues[position-1];
        double XDiff = gridPoints[i]-linearZValues[position-1];
        
        double redYMin = std::get<0>(haxbyColors[position-1]);
        double redDeltaY = std::get<0>(haxbyColors[position])-std::get<0>(haxbyColors[position-1]);
        double greenYMin = std::get<1>(haxbyColors[position-1]);
        double greenDeltaY = std::get<1>(haxbyColors[position])-std::get<1>(haxbyColors[position-1]);
        double blueYMin = std::get<2>(haxbyColors[position-1]);
        double blueDeltaY = std::get<2>(haxbyColors[position])-std::get<2>(haxbyColors[position-1]);
        
        red = round(redYMin + XDiff*redDeltaY/DeltaX);
        green = round(greenYMin + XDiff*greenDeltaY/DeltaX);
        blue = round(blueYMin + XDiff*blueDeltaY/DeltaX);

        pixelValue = std::make_tuple(red, green, blue);
      }
    }
    coloredPixelValues.push_back(pixelValue);
  }
  return coloredPixelValues;
}

// method to interpolate grid points to grey level pixels
std::vector<unsigned int> ImageCreation::interpolateGreyLevel(double* &gridPoints, double zMin, double zMax, double dfNoDataValue){
  // define interpolated pixels container
  std::vector<unsigned int> greyPixelValues;
  // define maximum grey level value
  int maxGreyLevelValue = 255;
  // define a pixel value used to fill greyPixelValues variable
  unsigned int pixelValue = 0;

  // interpolate elevation values
  for (int i = 0; i < m_imgWidth*m_imgWidth; i++){
    if(gridPoints[i] == dfNoDataValue){
      pixelValue = 0;
    }

    else{
      pixelValue = round((gridPoints[i]-zMin)/(zMax-zMin)*maxGreyLevelValue);
    }
    greyPixelValues.push_back(pixelValue);
  }
  return greyPixelValues;
}