#ifndef VISION_H
#define VISION_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class Vision {
    public:
        typedef std::array<cv::Point, 4> Square;
        Vision(int device, bool debug = true);

        void detectSquares(const cv::Mat& image, std::vector<Square>& output, int tresh, int dist, int minArea, int distDiff, int axisDiff);
        void simplifySquares(const std::vector<Square>& squares, std::vector<Square>& output);
        void keepSimilar(const std::vector<Square>& squares, std::vector<Square>& output, int tresh);
        void orderSquares(std::vector<Square>& squares, std::vector<std::vector<Square>>& output);
        void drawSquares(const cv::Mat& colors, const std::vector<Square>& squares, cv::Mat drawing, bool fill);
        void groupColors(const std::vector<cv::Scalar>& colors, std::vector<int>& output, int tresh);
        cv::Scalar detectColor(const cv::Mat& img, const Square& square);
        bool isSquare(const std::vector<cv::Point>& approx, int minArea, int distDiff, int axisDiff);
        cv::Point center(const Square& square);
        float getRadius(const Square& square);
        float distance(const cv::Point& p1, const cv::Point& p2);
        Square sortSquare(const Square& square);
        cv::Rect squareToRect(const Square& square);

        void updateDebug();
        void detectStickers(int faceNb, bool debug = false);
        void scanCube(std::vector<std::string>& output);

    private:
        cv::VideoCapture _cap;
        bool _debug;
        int _detectTresh;
        int _detectDist;
        int _minArea;
        int _distDiff;
        int _axisDiff;
        int _similarTresh;
        int _step;
        std::array<std::vector<std::vector<cv::Scalar>>, 6> _stickers;
};

#endif

