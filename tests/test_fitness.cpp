#include <gtest/gtest.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../fitness/hamming.h"
#include "../fitness/hausdorff.h"
#include "../fitness/hausdorffcanny.h"
#include "../fitness/hausdorffsmall.h"
#include "../fitness/hausdorffmodified.h"
#include "../exceptions.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static cv::Mat makeGray(int rows, int cols, uchar fill)
{
    return cv::Mat(rows, cols, CV_8U, cv::Scalar(fill));
}

// Create a small gradient image (values 0..255 spread across pixels)
static cv::Mat makeGradient(int rows = 16, int cols = 16)
{
    cv::Mat img(rows, cols, CV_8U);
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            img.at<uchar>(r, c) = static_cast<uchar>((r * cols + c) % 256);
    return img;
}

// Create an image with a white rectangle in the centre
static cv::Mat makeRectImage(int rows = 32, int cols = 32)
{
    cv::Mat img = makeGray(rows, cols, 0);
    cv::rectangle(img, cv::Point(4, 4), cv::Point(cols - 5, rows - 5),
                  cv::Scalar(255), cv::FILLED);
    return img;
}

// ---------------------------------------------------------------------------
// Hamming fitness tests
// ---------------------------------------------------------------------------
TEST(HammingTest, IdenticalImagesGiveZeroScore)
{
    cv::Mat ref = makeRectImage();
    Hamming hamming(ref);

    cv::Mat candidate = ref.clone();
    int score = hamming.measure(candidate);

    // Identical images → XOR == 0 → score == 0
    EXPECT_EQ(score, 0);
}

TEST(HammingTest, OppositeImagesGiveHighScore)
{
    cv::Mat ref = makeGray(16, 16, 255);
    Hamming hamming(ref);

    cv::Mat candidate = makeGray(16, 16, 0);   // completely different
    int score = hamming.measure(candidate);

    EXPECT_GT(score, 0);
}

TEST(HammingTest, ScoreIsNonNegative)
{
    cv::Mat ref = makeGradient(16, 16);
    Hamming hamming(ref);

    cv::Mat candidate = makeGradient(16, 16);
    candidate.at<uchar>(0, 0) = 0;  // one pixel different

    int score = hamming.measure(candidate);
    EXPECT_GE(score, 0);
}

TEST(HammingTest, MoreDifferentImageHasHigherOrEqualScore)
{
    cv::Mat ref = makeRectImage(32, 32);
    Hamming hamming(ref);

    cv::Mat slightly_different = ref.clone();
    slightly_different.at<uchar>(0, 0) = 255 - ref.at<uchar>(0, 0);
    int score1 = hamming.measure(slightly_different);

    cv::Mat very_different = makeGray(32, 32, 0);
    int score2 = hamming.measure(very_different);

    EXPECT_LE(score1, score2);
}

TEST(HammingTest, StaticCreateFactory)
{
    cv::Mat ref = makeRectImage();
    Fitness* f = Hamming::create(ref);
    ASSERT_NE(f, nullptr);

    cv::Mat candidate = ref.clone();
    EXPECT_EQ(f->measure(candidate), 0);

    delete f;
}

// ---------------------------------------------------------------------------
// Hausdorff fitness tests
// ---------------------------------------------------------------------------
TEST(HausdorffTest, IdenticalImagesGiveLowScore)
{
    cv::Mat ref = makeRectImage(32, 32);
    Hausdorff hausdorff(ref);

    cv::Mat candidate = ref.clone();
    int score = hausdorff.measure(candidate);

    EXPECT_EQ(score, 0);
}

TEST(HausdorffTest, ScoreIsNonNegative)
{
    cv::Mat ref = makeRectImage(32, 32);
    Hausdorff hausdorff(ref);

    cv::Mat candidate = makeGray(32, 32, 0);
    int score = hausdorff.measure(candidate);
    EXPECT_GE(score, 0);
}

TEST(HausdorffTest, StaticCreateFactory)
{
    cv::Mat ref = makeRectImage();
    Fitness* f = Hausdorff::create(ref);
    ASSERT_NE(f, nullptr);

    cv::Mat candidate = ref.clone();
    EXPECT_EQ(f->measure(candidate), 0);

    delete f;
}

// ---------------------------------------------------------------------------
// HausdorffCanny fitness tests
// ---------------------------------------------------------------------------
TEST(HausdorffCannyTest, IdenticalImagesGiveLowScore)
{
    cv::Mat ref = makeRectImage(32, 32);
    HausdorffCanny hc(ref);

    cv::Mat candidate = ref.clone();
    int score = hc.measure(candidate);

    // After Canny edge detection, identical images → 0
    EXPECT_EQ(score, 0);
}

TEST(HausdorffCannyTest, StaticCreateFactory)
{
    cv::Mat ref = makeRectImage();
    Fitness* f = HausdorffCanny::create(ref);
    ASSERT_NE(f, nullptr);
    delete f;
}

// ---------------------------------------------------------------------------
// HausdorffSmall fitness tests
// ---------------------------------------------------------------------------
TEST(HausdorffSmallTest, IdenticalImagesGiveLowScore)
{
    cv::Mat ref = makeRectImage(64, 64);
    HausdorffSmall hs(ref);

    cv::Mat candidate = ref.clone();
    int score = hs.measure(candidate);

    EXPECT_EQ(score, 0);
}

TEST(HausdorffSmallTest, StaticCreateFactory)
{
    cv::Mat ref = makeRectImage(64, 64);
    Fitness* f = HausdorffSmall::create(ref);
    ASSERT_NE(f, nullptr);
    delete f;
}

// ---------------------------------------------------------------------------
// HausdorffModified fitness tests
// ---------------------------------------------------------------------------
TEST(HausdorffModifiedTest, IdenticalImagesGiveLowScore)
{
    cv::Mat ref = makeRectImage(32, 32);
    HausdorffModified hm(ref);

    cv::Mat candidate = ref.clone();
    int score = hm.measure(candidate);

    EXPECT_EQ(score, 0);
}

TEST(HausdorffModifiedTest, StaticCreateFactory)
{
    cv::Mat ref = makeRectImage();
    Fitness* f = HausdorffModified::create(ref);
    ASSERT_NE(f, nullptr);
    delete f;
}

// ---------------------------------------------------------------------------
// FitnessType enum helpers (in fitness.h)
// ---------------------------------------------------------------------------
TEST(FitnessTypeTest, TypeToStringRoundTrip)
{
    std::vector<FitnessType> types = {
        HAMMING, HAUSDORFF_MODIFIED, HAUSDORFF_CANNY
    };
    for (FitnessType t : types)
    {
        std::string s = fitnessTypeToString(t);
        FitnessType back = fitnessTypeFromString(s);
        EXPECT_EQ(back, t);
    }
}

TEST(FitnessTypeTest, InvalidStringThrows)
{
    std::string bad = "INVALID";
    EXPECT_THROW(fitnessTypeFromString(bad), InvalidEnumException);
}
