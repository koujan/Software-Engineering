// Include our own code
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Include the gtest library
#include <gtest/gtest.h>

// Create our own test to check the different function of our class
// This function will take two arguments:
// * The test case name 
// * The test name
TEST(DummyBuild, NoInitialisation) {
    
    cv::Mat test_image=cv::imread("../src/tests/unit/common/imagetest.bmp");
    GTEST_ASSERT_EQ(test_image.cols,640);
    GTEST_ASSERT_EQ(test_image.rows,480);

}
