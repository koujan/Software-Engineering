#include "seededimgseg.h"
#include "seededimgseg.h"

seededimgseg::seededimgseg(Mat imsds,Mat im,double B,double sig,double xb,double xf,double ebs)
{
    segmentedImg.create(im.rows,im.cols,CV_8UC3); //creating an OpenCv library Matrix of Uchar & 8bit_depth & 3channels
    image=im.clone();     //initializing the image Mat data member with the "im" Mat
    seeds=imsds.clone();  //initializing the seeds Mat data member with the "imsds" Mat
    b= VectorXd::Zero(image.rows*image.cols,1);   //initializing the b vector with zeros and size according to the original image
    Beta=B;
    sigma=sig;
    xB=xb;
    xF=xf;
    epsilon=ebs;
}

seededimgseg::seededimgseg(double B,double sig,double xb,double xf,double ebs){
    Beta=B;
    sigma=sig;
    xB=xb;
    xF=xf;
    epsilon=ebs;

}

void seededimgseg::set_sigma(const double s){
    if(sigma>0)
        sigma=s;
}

double seededimgseg::get_sigma(){
    return sigma;
}

void seededimgseg::set_Beta(const double B){
    if(Beta>0)
         Beta=B;
}

double seededimgseg::get_Beta(){
    return Beta;
}

void seededimgseg::set_epsilon(const double ebs){
    if(epsilon>0)
        epsilon=ebs;
}

double seededimgseg::get_epsilon(){
    return epsilon;
}

void seededimgseg::set_xF(const double xf){
    xF=xf;
}

double seededimgseg::get_xF(){
    return xF;
}

void seededimgseg::set_xB(const double xb){
    xB=xb;
}

double seededimgseg::get_xB(){
    return xB;
}
void seededimgseg::Create_seeds_vec(){ // seeds matrix contain pixels of four different colours (Red,Blue,Green,Yellow)

    vecb.push_back(Vector3i(0,0,0)); // initializing the first triplet of vecb vector to zero because
    for(int i=0;i<seeds.rows;i++)    //* it is then used to indicate the number of the pixels that belong to each region
    {                               // two nested for loops used to iterate through each element of the seeds matrix in order to assign them to the vector "vecb" with their region number in order to be used later to calcualate the seeds based vectors and matrices
        for(int j=0;j<seeds.cols;j++){

                if(seeds.at<Vec3b> (i,j)[0]==0 && seeds.at<Vec3b> (i,j)[1]==0 && seeds.at<Vec3b> (i,j)[2]==255){ //checking if the pixel has a red colour
                      vecb.push_back(Vector3i(i,j,1));  // assign the location of this pixel with its region number to vecb variable
                      vecb.at(0)[0]++; // increamenting the first element of the first triplet which is used to indicate the number of pixels which belong to the region #1
                 }
                else if(seeds.at<Vec3b> (i,j)[0]==255 && seeds.at<Vec3b> (i,j)[1]==0 && seeds.at<Vec3b> (i,j)[2]==0){ //checking if the pixel has a blue colour
                    vecb.push_back(Vector3i(i,j,4));  // assign the location of this pixel with its region number to vecb variable

                }
                else if(seeds.at<Vec3b> (i,j)[0]==0 && seeds.at<Vec3b> (i,j)[1]==255 && seeds.at<Vec3b> (i,j)[2]==0){ //checking if the pixel has a green colour
                   vecb.push_back(Vector3i(i,j,2));  // assign the location of this pixel with its region number to vecb variable
                     vecb.at(0)[1]++;     // increamenting the first element of the first triplet which is used to indicate the number of pixels which belong to the region #2
                }
                else if (seeds.at<Vec3b> (i,j)[0]==0 && seeds.at<Vec3b> (i,j)[1]==255 && seeds.at<Vec3b> (i,j)[2]==255){  //checking if the pixel has a yellow colour
                    vecb.push_back(Vector3i(i,j,3)); // assign the location of this pixel with its region number to vecb variable
                    vecb.at(0)[2]++;  // increamenting the first element of the first triplet which is used to indicate the number of pixels which belong to the region #3
                }
            }
        }
}

void seededimgseg::create_b_vec(int const identifier){
    for(uint i=1;i<vecb.size();i++){ // two nested for loops used to iterate through each element of the vecb vector in order to assign the corresponding locations in the b vector as foreground with value XF or background with value XB based on the value of the identifier
        if(identifier==vecb[i][2]){  // checking if the identifier is equal to the pixel's region number,if yes then,
            b(vecb[i][0]*image.cols+vecb[i][1])=xF;  // it is deemed as a foreground pixel and consequently b at the corresponding  location is assigned XF value
        }
        else{
            b(vecb[i][0]*image.cols+vecb[i][1])=xB;  //otherwise it a background pixel  and consequently b at the corresponding location is assigned XF value
        }
    }
}

void seededimgseg::set_seed_based_mat(SparseMatrix<double>& Is){

    vector<TrD> fill_Is;  //vector of Triplets in order to fill the sparse matrix Is
    int ind;
    for(uint i=1;i<vecb.size();i++){  // iterating through every pixel of vecb
        ind=vecb[i][0]*image.cols+vecb[i][1]; // converting the index of this pixel into a number which represents its order in the orgianl image matrix
        fill_Is.push_back(TrD(ind,ind,1.0)); // assigning the vector fill_Is a unitary value at the index calculated previously
    }
Is.setFromTriplets(fill_Is.begin(),fill_Is.end());  //filling Is matrix by using the vector of triplets "fill_Is" whose values where set in the above line

}

int seededimgseg::get_max(double x1,double x2,double x3,double x4){ // function to return the index of the maximum pixel among 4 which belong to four different regions (used )
    if(x1>x2 && x1>x3 && x1>x4)
        return 1;
    else if(x2>x1 && x2>x3 && x2>x4)
        return 2;
    else if(x3>x1 && x3>x2 && x3>x4)
        return 3;
    else
        return 4;

}

SparseMatrix<double> seededimgseg:: compute_Laplacian_Mat(bool eight_neighbourhood){

    int irow, icol, jrow, jcol, j;
    double infnorm, val1, val2,a;
    int numpixel=image.cols*image.rows;  //total number of pixels of the original image
    int numcol=image.cols, numrow=image.rows;
    vector< TrD> weighttripletsW, weighttripletsD;   // two vectors of triplets, used to set the sparse matrices W and D
    MatrixXd tempD = MatrixXd::Zero(numrow,numcol);  //a temporary matrix used to ......
    SparseD L(numpixel,numpixel),D(numpixel,numpixel),W(numpixel,numpixel);  // sparse matrices L(Laplacian),D(diagonal Matrix),W(wieghted adjacency Matrix)

    if(eight_neighbourhood){  // checking the required type of neighbourhood rule, 4 or 8
            int val3,val4;    //
            for (int i=0;i<numpixel;i++){ // loop to iterate through all pixels
                irow = i/numcol;          // calculating the row index of each pixel from the for loop index
                icol = i%numcol;          // calculating the row index of each pixel from the for loop index
                if (irow==numrow-1)       // if the pixel  is in the last row, there are no neigbour pixels below it
                {
                    if (icol<numcol-1)    // checking if it is in the last column
                    {
                        jcol = icol+1;    // column index for the right neighbour
                        jrow = irow;      // row index for the right neighbour
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));  // caculating the infinity norm using three differen pixel colours RGB
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);            // caculating the infinity norm using three differen pixel colours RGB
                        infnorm = max(infnorm,a);              // caculating the infinity norm using three differen pixel colours RGB
                        val1 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;          // caculting Wij
                        j=jrow*numcol+jcol;   // caculatig the order of the right neighbour
                        weighttripletsW.push_back(TrD(i,j,val1));  // storing the wight and its indeces
                        weighttripletsW.push_back(TrD(j,i,val1));

                        tempD(irow,icol) += (val1);
                        tempD(jrow,jcol) += (val1);

                    }
                }
                else
                {
                    if (icol==0)                  // if the pixel  is in the first column, there are no neigbour pixels to the left of this pixel
                    {
                        jcol = icol+1;           // column index for the right neighbour
                        jrow = irow;             // row index for the right neighbour
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));     // caculating the infinity norm using three differen pixel's colours RGB
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);  // caculating the infinity norm using three differen pixel's colours RGB
                        infnorm = max(infnorm,a);          // caculating the infinity norm using three differen pixel's colours RGB
                        val1 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;   // caculting Wij
                        j=jrow*numcol+jcol;     // caculatig the order of the right neighbour
                        weighttripletsW.push_back(TrD(i,j,val1));   // storing the wight and its indices
                        weighttripletsW.push_back(TrD(j,i,val1));   // storing the same wight and its indices in the opposite pixel

                        tempD(jrow,jcol) += (val1);

                        jcol = icol;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val2 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val2));
                        weighttripletsW.push_back(TrD(j,i,val2));
                        tempD(jrow,jcol) += (val2);

                        jcol = icol+1;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val3 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val3));
                        weighttripletsW.push_back(TrD(j,i,val3));
                        tempD(jrow,jcol) += (val3);

                        tempD(irow,icol) += (val1 + val2 + val3);



                    }
                    else if (icol==numcol-1)    // checking if it is in the last column
                    {
                        jcol = icol;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val1 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val1));
                        weighttripletsW.push_back(TrD(j,i,val1));
                        tempD(jrow,jcol) += (val1);

                        jcol = icol-1;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val2 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val2));
                        weighttripletsW.push_back(TrD(j,i,val2));
                        tempD(jrow,jcol) += (val2);

                        tempD(irow,icol) += (val1 + val2);

                    }
                    else              //all other cases
                    {
                        jcol = icol+1;
                        jrow = irow;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val1 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val1));
                        weighttripletsW.push_back(TrD(j,i,val1));
                        tempD(jrow,jcol) += (val1);

                        jcol = icol;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val2 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val2));
                        weighttripletsW.push_back(TrD(j,i,val2));
                        tempD(jrow,jcol) += (val2);

                        jcol = icol+1;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val3 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val3));
                        weighttripletsW.push_back(TrD(j,i,val3));
                        tempD(jrow,jcol) += (val3);

                        jcol = icol-1;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val4 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val4));
                        weighttripletsW.push_back(TrD(j,i,val4));
                        tempD(jrow,jcol) += (val4);

                        tempD(irow,icol) += (val1 + val2 + val3 + val4);


                    }

                }
                weighttripletsD.push_back(TrD(i,i,tempD(irow,icol)));

            }
    }

    else{           // same as previous calculation but for 4 neighbours instead of 8

        for (int i=0;i<numpixel;i++)
         {

             irow = i/numcol;
             icol = i%numcol;
             if (irow==numrow-1)
             {
                 if (icol<numcol-1)
                 {
                       jcol = icol+1;
                        jrow = irow;

                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                      infnorm = max(infnorm,a);
                      val1 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                       weighttripletsW.push_back(TrD(i,i+1,val1));
                       weighttripletsW.push_back(TrD(i+1,i,val1));

                      tempD(irow,icol) += (val1);
                      tempD(jrow,jcol) += (val1);

                 }
              }
                else
              {

                 if (icol==numcol-1)
                  {
                      jcol = icol;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val1 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val1));
                        weighttripletsW.push_back(TrD(j,i,val1));
                        tempD(jrow,jcol) += (val1);

                        tempD(irow,icol) += (val1);
                    }
                    else
                    {
                        jcol = icol+1;
                        jrow = irow;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val1 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val1));
                        weighttripletsW.push_back(TrD(j,i,val1));

                        tempD(jrow,jcol) += (val1);

                        jcol = icol;
                        jrow = irow+1;
                        infnorm = max(abs(image.at<Vec3b>(irow,icol).val[0]-image.at<Vec3b>(jrow,jcol).val[0] ),abs(image.at<Vec3b>(irow,icol).val[1]-image.at<Vec3b>(jrow,jcol).val[1]));
                        a=abs(image.at<Vec3b>(irow,icol).val[2]-image.at<Vec3b>(jrow,jcol).val[2]);
                        infnorm = max(infnorm,a);
                        val2 = exp((-Beta*(pow(infnorm,2.0)))/sigma)+epsilon;
                        j=jrow*numcol+jcol;
                        weighttripletsW.push_back(TrD(i,j,val2));
                        weighttripletsW.push_back(TrD(j,i,val2));

                        tempD(jrow,jcol) += (val2);

                        tempD(irow,icol) += (val1 + val2);

                 }

               }

             weighttripletsD.push_back(TrD(i,i,tempD(irow,icol)));
            }
    }

    W.setFromTriplets(weighttripletsW.begin(), weighttripletsW.end());
    D.setFromTriplets(weighttripletsD.begin(), weighttripletsD.end());
    L = D-W;
    return L;

}

void seededimgseg::segmentImage(bool eight_neighbourhood){

    VectorXd x1,x2,x3,x4;      //4 vectors that represent the 4 number of regions, which is assumed to be the maximum number of regions in this code
    Vec3b red(0,0,255);        // red pixel
    Vec3b green(0,255,0);      // green pixel
    Vec3b blue(255,0,0);      // blue pixel
    Vec3b yellow(0,255,255);  // yellow pixel
    double t = (double)getTickCount(),t1;      // recording the starting time
    int irow,icol;
    int numpixel=image.cols*image.rows;        // calculating the totla #pixels
    SparseD Lap(numpixel,numpixel),Is(numpixel,numpixel),A(numpixel,numpixel);
    Lap=compute_Laplacian_Mat(eight_neighbourhood);
    Create_seeds_vec();  // Caculate "vecb", whcih should contain the locations of each pixel of the different regions
    set_seed_based_mat(Is); // Caculate Matrix Is
    A = (Is + Lap*Lap);
    SimplicialLDLT<SparseD> chol(A);
    create_b_vec(1);       //create vector b using region 1 as foreground (2 regions segmentaion case)
    x1 = chol.solve(b);    //solving the linear system



     if(vecb[0][2]!=0){         // four regions segmentation case

         create_b_vec(3);       //create vector b using region 3 as foreground
         x3 = chol.solve(b);    // solving the linear system
         create_b_vec(4);       //create vector b using region 4 as foreground
         x4 = chol.solve(b);   // solving the linear system
         create_b_vec(2);      //create vector b using region 2 as foreground
         x2 = chol.solve(b);   // solving the linear system
         for (int i=0;i<image.rows*image.cols;i++)      //loop for comparing the 4 solutions of 4 different linear systems based on 4 different regions
         {
             irow = i/image.cols;         //calcuating the row index
             icol = i%image.cols;         //calculating the column index
              switch(get_max(x1(i),x2(i),x3(i),x4(i))){
              case 1:
                segmentedImg.at<Vec3b>(irow,icol)=red;  //assigning red colour to region 1
                 break;
              case 2:
                segmentedImg.at<Vec3b>(irow,icol) = green;  //assigning green colour to region 2
                  break;
              case 3:
                segmentedImg.at<Vec3b>(irow,icol) = yellow; //assigning yellow colour to region 3
                  break;
              default:
                 segmentedImg.at<Vec3b>(irow,icol) = blue;  //assigning blue colour to region 4

              }
         }
     }

     else if(vecb[0][1]!=0) //three regions segmentation case
       {

             create_b_vec(4);
             x4 = chol.solve(b);
             create_b_vec(2);
             x2 = chol.solve(b);
             for (int i=0;i<image.rows*image.cols;i++)
             {
                 irow = i/image.cols;      //calcuating the row index
                 icol = i%image.cols;      //calculating the column index
                  switch(get_max(x1(i),x2(i),x4(i))){
                  case 1:
                    segmentedImg.at<Vec3b>(irow,icol)=red; //assigning red colour to region 1
                     break;
                  case 2:
                    segmentedImg.at<Vec3b>(irow,icol) = green; //assigning green colour to region 2
                      break;
                  default:
                    segmentedImg.at<Vec3b>(irow,icol) = blue; //assigning blue colour to region 4
                  }
           }
     }
    else{                 // two regions segmentation case

             for (int i=0;i<image.rows*image.cols;i++)   //loop for comparing the 2 diffrent solutions (x1,x4) of 2 different linear systems based on three different regions
             {
                 irow = i/image.cols;                  //calcuating the row index
                 icol = i%image.cols;                  //calculating the column index
                 if (x1(i)>=((xB+xF)/2))
                     segmentedImg.at<Vec3b>(irow,icol) = red;          //assigning red colour to region 1
                 else
                     segmentedImg.at<Vec3b>(irow,icol) = blue;        //assigning blue colour to region 4

              }
         }
     cout << "Segmentation Finished" << endl;
     t1 = ((double)getTickCount() - t)/getTickFrequency();
     cout << "Times passed in seconds: " << t1 << endl;
}

void seededimgseg::show_seg_im(){

    imshow("Segmented image",segmentedImg); //show the segmented image
    cvWaitKey(0);  // keep the image shown until the user press enter or space
}

void seededimgseg::show_org_im(){
    imshow("original image",image);  //show the segmented image
    cvWaitKey(0);  // keep the image shown until the user press enter or space
}

seededimgseg::~seededimgseg(){
 image.~Mat();   //deleteing original image matrix from the memory
 segmentedImg.~Mat(); //deleteing segmented image matrix from the memory
 seeds.~Mat();  //deleteing seeds matrix from the memory


}

Mat seededimgseg::get_segmentedImg(){
    return segmentedImg;
}

seededimgseg seededimgseg::operator=(seededimgseg temp){  //overloading the assignment operators

    Beta=temp.Beta;
    epsilon=temp.epsilon;
    sigma=temp.sigma;
    xB=temp.xB;
    xF=temp.xF;
    image=temp.image.clone();
    segmentedImg=temp.segmentedImg.clone();
    return *this;

}
