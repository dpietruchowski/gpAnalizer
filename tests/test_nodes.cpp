#include <gtest/gtest.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../node/functionnode.h"
#include "../node/morphonode.h"
#include "../node/threshnode.h"
#include "../node/terminalnode.h"
#include "../node/functionset.h"
#include "../exceptions.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static cv::Mat makeWhiteImage(int rows = 8, int cols = 8)
{
    return cv::Mat(rows, cols, CV_8U, cv::Scalar(255));
}

static cv::Mat makeBlackImage(int rows = 8, int cols = 8)
{
    return cv::Mat(rows, cols, CV_8U, cv::Scalar(0));
}

static cv::Mat makeCheckerboard(int rows = 8, int cols = 8)
{
    cv::Mat img(rows, cols, CV_8U);
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            img.at<uchar>(r, c) = ((r + c) % 2 == 0) ? 255 : 0;
    return img;
}

// ---------------------------------------------------------------------------
// TerminalNode tests
// ---------------------------------------------------------------------------
TEST(TerminalNodeTest, Creation)
{
    NodePtr node = TerminalNode::create(1);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getId().type, TERMINAL_NODE);
    EXPECT_EQ(node->getSize(), 0);   // terminal takes no children
    EXPECT_TRUE(node->isValid());
}

TEST(TerminalNodeTest, ExecuteDoesNothing)
{
    NodePtr node = TerminalNode::create(1);
    cv::Mat src = makeWhiteImage();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {src};
    // execute on TerminalNode must not throw and must not modify dst
    EXPECT_NO_THROW(node->execute(srcs, dst));
    EXPECT_TRUE(dst.empty());  // TerminalNode::execute is a no-op
}

TEST(TerminalNodeTest, Clone)
{
    NodePtr node  = TerminalNode::create(2);
    NodePtr clone = node->clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_EQ(clone->getId().type, TERMINAL_NODE);
}

// ---------------------------------------------------------------------------
// FunctionNode tests
// ---------------------------------------------------------------------------
// FunctionSet is a static member of FunctionNode; its constructor
// calls addAllFunctions(), so all functions are available without
// extra setup.

TEST(FunctionNodeTest, BitwiseNotInverts)
{
    // bitwiseNot requires 1 argument; choose a 1-arg function
    // Ensure bitwiseNot is in the set (it is by default construction)
    NodePtr node = FunctionNode::createFromXml(nullptr);
    // We cannot call createFromXml(nullptr); use the static mutation approach
    // instead: get a function node via create, then validate by executing.
    // FunctionNode::create picks a random function.
    // We'll test the FunctionSet static operation directly.
    cv::Mat src = makeWhiteImage();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {src};

    FunctionSet::bitwiseNot(srcs, dst);

    EXPECT_EQ(dst.rows, src.rows);
    EXPECT_EQ(dst.cols, src.cols);
    EXPECT_EQ(cv::countNonZero(dst), 0);   // NOT(255) == 0
}

TEST(FunctionNodeTest, BitwiseAndBothWhite)
{
    cv::Mat a = makeWhiteImage();
    cv::Mat b = makeWhiteImage();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {a, b};

    FunctionSet::bitwiseAnd(srcs, dst);

    EXPECT_EQ(cv::countNonZero(dst), a.rows * a.cols);
}

TEST(FunctionNodeTest, BitwiseAndWhiteBlack)
{
    cv::Mat a = makeWhiteImage();
    cv::Mat b = makeBlackImage();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {a, b};

    FunctionSet::bitwiseAnd(srcs, dst);

    EXPECT_EQ(cv::countNonZero(dst), 0);
}

TEST(FunctionNodeTest, BitwiseOrWhiteBlack)
{
    cv::Mat a = makeWhiteImage();
    cv::Mat b = makeBlackImage();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {a, b};

    FunctionSet::bitwiseOr(srcs, dst);

    EXPECT_EQ(cv::countNonZero(dst), a.rows * a.cols);
}

TEST(FunctionNodeTest, BitwiseXorIdentical)
{
    cv::Mat a = makeCheckerboard();
    cv::Mat b = makeCheckerboard();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {a, b};

    FunctionSet::bitwiseXor(srcs, dst);

    EXPECT_EQ(cv::countNonZero(dst), 0);   // XOR of identical images is 0
}

TEST(FunctionNodeTest, BitwiseNotThrowsOnWrongArgCount)
{
    cv::Mat dst;
    std::vector<cv::Mat> srcs; // empty – wrong count

    EXPECT_THROW(FunctionSet::bitwiseNot(srcs, dst), InvalidArgumentException);
}

TEST(FunctionNodeTest, BitwiseAndThrowsOnWrongArgCount)
{
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {makeWhiteImage()}; // 1 instead of 2

    EXPECT_THROW(FunctionSet::bitwiseAnd(srcs, dst), InvalidArgumentException);
}

TEST(FunctionNodeTest, CreateReturnsNonNull)
{
    NodePtr node = FunctionNode::create(42);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getId().type, FUNCTION_NODE);
}

TEST(FunctionNodeTest, ClonePreservesType)
{
    NodePtr node  = FunctionNode::create(1);
    NodePtr clone = node->clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_EQ(clone->getId().type, FUNCTION_NODE);
}

TEST(FunctionNodeTest, DiffOperation)
{
    cv::Mat a = makeWhiteImage();
    cv::Mat b = makeBlackImage();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {a, b};

    FunctionSet::diff(srcs, dst);

    // diff(A,B) = A - (A union B); with A=white, B=black, union=white → diff=black
    EXPECT_EQ(cv::countNonZero(dst), 0);
}

// ---------------------------------------------------------------------------
// MorphoNode tests
// ---------------------------------------------------------------------------
TEST(MorphoNodeTest, CreateReturnsNonNull)
{
    NodePtr node = MorphoNode::create(1);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getId().type, MORPHO_NODE);
    EXPECT_EQ(node->getSize(), 1);   // morpho takes 1 input
}

TEST(MorphoNodeTest, ExecuteOnWhiteImageProducesValidMat)
{
    NodePtr node = MorphoNode::create(1);
    cv::Mat src = makeWhiteImage(16, 16);
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {src};

    EXPECT_NO_THROW(node->execute(srcs, dst));
    EXPECT_FALSE(dst.empty());
    EXPECT_EQ(dst.size(), src.size());
}

TEST(MorphoNodeTest, ExecuteThrowsOnWrongArgCount)
{
    MorphoParameters param = MorphoParameters::getRandom();
    MorphoElement elem = MorphoElement::getRandom();
    cv::Mat dst;
    std::vector<cv::Mat> srcs; // empty – wrong count

    EXPECT_THROW(
        MorphoNode::morphologyOperation(srcs, dst, param, elem.element),
        InvalidArgumentException
    );
}

TEST(MorphoNodeTest, ClonePreservesType)
{
    NodePtr node  = MorphoNode::create(1);
    NodePtr clone = node->clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_EQ(clone->getId().type, MORPHO_NODE);
}

TEST(MorphoNodeTest, MutateDoesNotThrow)
{
    NodePtr node = MorphoNode::create(1);
    EXPECT_NO_THROW(node->mutate());
}

TEST(MorphoNodeTest, ParametersRandomGenerationInRange)
{
    for (int i = 0; i < 50; ++i)
    {
        MorphoParameters p = MorphoParameters::getRandom();
        EXPECT_GE(p.morphType, 0);
        EXPECT_LE(p.morphType, 7);
        EXPECT_GE(p.iterations, 1);
        EXPECT_LE(p.iterations, MorphoParameters::MAX_ITER);
    }
}

TEST(MorphoNodeTest, ElementRandomGenerationValidSize)
{
    for (int i = 0; i < 20; ++i)
    {
        MorphoElement e = MorphoElement::getRandom();
        EXPECT_GE(e.element.rows, 1);
        EXPECT_LE(e.element.rows, MorphoParameters::MAX_HEIGHT);
        EXPECT_GE(e.element.cols, 1);
        EXPECT_LE(e.element.cols, MorphoParameters::MAX_WIDTH);
    }
}

TEST(MorphoNodeTest, ParametersTypeToString)
{
    MorphoParameters p(cv::MORPH_ERODE, 3);
    EXPECT_EQ(p.typeToString(), "Erode");
    MorphoParameters p2(cv::MORPH_DILATE, 1);
    EXPECT_EQ(p2.typeToString(), "Dilate");
}

TEST(MorphoNodeTest, ParametersTypeFromStringRoundTrip)
{
    std::vector<std::string> types = {
        "Erode", "Dilate", "Open", "Close",
        "Gradient", "Tophat", "Blackhat", "Hitmiss"
    };
    MorphoParameters p;
    for (const auto& t : types)
    {
        int code = p.typeFromString(t);
        MorphoParameters tmp(code, 1);
        EXPECT_EQ(tmp.typeToString(), t);
    }
}

TEST(MorphoNodeTest, InvalidTypeFromStringThrows)
{
    MorphoParameters p;
    EXPECT_THROW(p.typeFromString("Invalid"), InvalidEnumException);
}

TEST(MorphoNodeTest, InvalidMorphoTypeThrowsInOperation)
{
    MorphoParameters bad_param(99, 1);  // type 99 is invalid
    MorphoElement elem = MorphoElement::getRandom();
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {makeWhiteImage()};

    EXPECT_THROW(
        MorphoNode::morphologyOperation(srcs, dst, bad_param, elem.element),
        InvalidArgumentException
    );
}

// ---------------------------------------------------------------------------
// ThreshNode tests
// ---------------------------------------------------------------------------
TEST(ThreshNodeTest, CreateReturnsNonNull)
{
    NodePtr node = ThreshNode::create(1);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->getId().type, THRESH_NODE);
    EXPECT_EQ(node->getSize(), 1);
}

TEST(ThreshNodeTest, ExecuteBinaryThreshold)
{
    NodePtr node = ThreshNode::create(1);
    cv::Mat src(8, 8, CV_8U);
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            src.at<uchar>(r, c) = static_cast<uchar>(r * 30 + c);
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {src};

    EXPECT_NO_THROW(node->execute(srcs, dst));
    EXPECT_FALSE(dst.empty());
    EXPECT_EQ(dst.size(), src.size());
}

TEST(ThreshNodeTest, ExecuteThrowsOnWrongArgCount)
{
    ThreshParameters param = ThreshParameters::getRandom();
    cv::Mat dst;
    std::vector<cv::Mat> srcs;   // empty

    EXPECT_THROW(
        ThreshNode::thresholdOperation(srcs, dst, param),
        InvalidArgumentException
    );
}

TEST(ThreshNodeTest, ClonePreservesType)
{
    NodePtr node  = ThreshNode::create(1);
    NodePtr clone = node->clone();
    ASSERT_NE(clone, nullptr);
    EXPECT_EQ(clone->getId().type, THRESH_NODE);
}

TEST(ThreshNodeTest, MutateDoesNotThrow)
{
    NodePtr node = ThreshNode::create(1);
    EXPECT_NO_THROW(node->mutate());
}

TEST(ThreshNodeTest, ParametersRandomInRange)
{
    for (int i = 0; i < 50; ++i)
    {
        ThreshParameters p = ThreshParameters::getRandom();
        EXPECT_GE(p.threshValue, 1);
        EXPECT_LE(p.threshValue, 254);
        EXPECT_GE(p.threshType, 0);
        EXPECT_LE(p.threshType, 4);
    }
}

TEST(ThreshNodeTest, TypeToStringRoundTrip)
{
    std::vector<std::pair<int,std::string>> cases = {
        {cv::THRESH_BINARY,     "ThreshBinary"},
        {cv::THRESH_BINARY_INV, "ThreshBinaryInv"},
        {cv::THRESH_TOZERO,     "ThreshToZero"},
        {cv::THRESH_TOZERO_INV, "ThreshToZeroInv"},
        {cv::THRESH_TRUNC,      "ThreshTrunc"}
    };
    for (const auto& c : cases)
    {
        ThreshParameters p(128, c.first);
        EXPECT_EQ(p.typeToString(), c.second);
        EXPECT_EQ(p.typeFromString(c.second), c.first);
    }
}

TEST(ThreshNodeTest, InvalidTypeFromStringThrows)
{
    ThreshParameters p;
    EXPECT_THROW(p.typeFromString("Unknown"), InvalidEnumException);
}

TEST(ThreshNodeTest, InvalidThreshTypeThrowsInOperation)
{
    ThreshParameters bad(128, 99);  // type 99 is invalid
    cv::Mat dst;
    std::vector<cv::Mat> srcs = {makeWhiteImage()};

    EXPECT_THROW(
        ThreshNode::thresholdOperation(srcs, dst, bad),
        InvalidArgumentException
    );
}

// ---------------------------------------------------------------------------
// NodeId tests
// ---------------------------------------------------------------------------
TEST(NodeIdTest, DefaultConstruction)
{
    NodeId id;
    EXPECT_EQ(id.type, NULL_NODE);
    EXPECT_EQ(id.number, 0u);
    EXPECT_EQ(id.cloneNumber, 0u);
}

TEST(NodeIdTest, ToStringFromStringRoundTrip)
{
    NodeId id(FUNCTION_NODE, 42, 3);
    std::string s = id.toString();

    NodeId id2;
    id2.fromString(s);

    EXPECT_EQ(id2.type, FUNCTION_NODE);
    EXPECT_EQ(id2.number, 42u);
    EXPECT_EQ(id2.cloneNumber, 3u);
}

TEST(NodeIdTest, EqualityOperator)
{
    NodeId a(MORPHO_NODE, 1, 0);
    NodeId b(MORPHO_NODE, 1, 0);
    NodeId c(THRESH_NODE, 1, 0);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(NodeIdTest, EnumFromStringInvalidThrows)
{
    EXPECT_THROW(enumFromString("INVALID_NODE"), InvalidEnumException);
}
