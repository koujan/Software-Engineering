#ifndef SEEDEDIMGSEG
#define SEEDEDIMGSEG
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "opencv2/core/core.hpp"
#include "opencv2/core/eigen.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include<QDebug>
using namespace std;
using namespace Eigen;
using namespace cv;
typedef Triplet<double> TrD;
typedef SparseMatrix<double> SparseD;
class seededimgseg {

public:
    seededimgseg(double=0.05,double=0.1,double=1,double=-1,double=pow(10,-6)); //default constructor
    seededimgseg(Mat seeds,Mat image,double B,double sig,double xb,double xf,double ebs); //User defined constructor
    ~seededimgseg();  // seededimgseg class destructor
    void set_sigma(const double); //set sigma parameter value
    double get_sigma(void);      //get sigma parameter value
    void set_xB(const double);   //set xB parameter value
    double get_xB(void);         //get xB parameter value
    void set_xF(const double);   //set xF parameter value
    double get_xF(void);         //get xF parameter value
    void set_Beta(const double); //set Beta parameter value
    double get_Beta(void); //get Beta parameter value
    void set_epsilon(const double); //set epsilon parameter value
    double get_epsilon(void);  //get epsilon parameter value
    Mat get_segmentedImg(void); // return the segmented image
    void Create_seeds_vec(); // create the seeds vector from the matrix given by the GUI
    void set_seed_based_mat(SparseMatrix<double>&);  //Calculate vector Ls of equation:(Is+L^2)*x=b
    void create_b_vec(int const identifier);         //Caculate vector b of the equation:(Is+L^2)*x=b
    SparseMatrix<double> compute_Laplacian_Mat(bool=false); // Calcuate the Laplacian Matrix
    int get_max(double,double,double=-20000,double=-20000); // function to return the index of the maximum pixel
    void segmentImage(bool=false); // function to start the segmentation procedure
    void show_seg_im(); // show the segmented image
    void show_org_im(); // show the original image
    seededimgseg operator=(seededimgseg temp);  //overloading assignment operator

private:
Mat image;  // Matrix that contains the image to be segmented
Mat seeds;  //Matrix that contains the seeds that passed as an image from the GUI
Mat segmentedImg;  //Matrix that contains the segmented image
VectorXd b;  // Vector that represents the left hand side of the following linear system of equations:(Is+L^2)*x=b
vector<Vector3i> vecb; // vector that contains the  seeds of the image to be segmented
double Beta,sigma,xB,xF,epsilon; //different parameters that are used to find the solution to the equation: (Is+L^2)*x=b


};


#endif // SEEDEDIMGSEG

