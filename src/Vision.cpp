#include "Vision.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdio>
#include <unordered_set>
#include <limits>

Vision::Vision(int device, bool debug) :
    _cap(device),
    _debug(debug),
    _detectTresh(20),
    _detectDist(3),
    _minArea(500),
    _distDiff(20),
    _axisDiff(20),
    _similarTresh(20),
    _step(3) {

    if(!_cap.isOpened()) {
        fprintf(stderr, "Cannot open the video cam\n");
    }
    if(_debug) {
        cv::namedWindow("Win", CV_WINDOW_AUTOSIZE);
        cv::createTrackbar("Tresh:", "Win", &_detectTresh, 128, 0);
        cv::createTrackbar("Distance:", "Win", &_detectDist, 128, 0);
        cv::createTrackbar("Area:", "Win", &_minArea, 5000, 0);
        cv::createTrackbar("Edge dist:", "Win", &_distDiff, 100, 0);
        cv::createTrackbar("Axis dist:", "Win", &_axisDiff, 100, 0);
        cv::createTrackbar("Similar tresh:", "Win", &_similarTresh, 100, 0);
        cv::createTrackbar("Step:", "Win", &_step, 4, 0);
    }
}

void Vision::detectSquares(const cv::Mat& image, std::vector<Square>& output) {
    for(int i = _detectDist; i < (255 - _detectDist); i += _detectDist) {
        cv::Mat current = (image > (i - _detectTresh)) & (image < (i + _detectTresh));
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        findContours(current, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
        for(int j = 0; j < contours.size(); ++j) {
            std::vector<cv::Point> approx;
            cv::approxPolyDP(cv::Mat(contours[j]), approx, cv::arcLength(cv::Mat(contours[j]), true)*0.04, true);
            if(isSquare(approx, _minArea, _distDiff, _axisDiff)) {
                output.push_back(sortSquare({{approx[0], approx[1], approx[2], approx[3]}}));
            }
        }
    }
}

void Vision::simplifySquares(const std::vector<Square>& squares, std::vector<Square>& output) {
    std::unordered_set<size_t> done;
    for(size_t i = 0; i < squares.size(); ++i) {
        std::vector<Square> similarSquares;
        similarSquares.push_back(squares[i]);
        if(done.count(i)) {
            continue;
        }
        done.insert(i);
        float radius = getRadius(squares[i]);
        cv::Point sqCenter = center(squares[i]);
        for(size_t j = i+1; j < squares.size(); ++j) {
            if(done.count(j)) {
                continue;
            }
            if(distance(sqCenter, center(squares[j])) < radius) {
                similarSquares.push_back(squares[j]);
                done.insert(j);
            }
        }
        //Calculate the average square
        Square averageSquare = {{cv::Point(0, 0), cv::Point(0, 0), cv::Point(0, 0), cv::Point(0, 0)}};
        for(const Square& square : similarSquares) {
            for(int j = 0; j < 4; ++j) {
                averageSquare[j] += square[j];
            }
        }
        for(int j = 0; j < 4; ++j) {
            averageSquare[j].x /= similarSquares.size();
            averageSquare[j].y /= similarSquares.size();
        }
        output.push_back(averageSquare);
    }
}

void Vision::keepSimilar(const std::vector<Square>& squares, std::vector<Square>& output) {
    if(squares.empty()) {
        return;
    }
    std::vector<int> diags;
    for(const Square& square : squares) {
        diags.push_back(distance(square[0], square[2]));
    }
    std::sort(diags.begin(), diags.end());
    int medArea = diags[diags.size()/2];
    for(const Square& square : squares) {
        if(fabs((distance(square[0], square[2]) / (float)medArea) - 1)*100 < _similarTresh) {
            output.push_back(square);
        }
    }
}

void Vision::orderSquares(std::vector<Square> squares, std::vector<std::vector<Square>>& output) {
    std::vector<std::vector<size_t>> cols;
    std::unordered_set<size_t> done;

    //Organize by column
    for(size_t i = 0; i < squares.size(); ++i) {
        if(done.count(i)) {
            continue;
        }
        done.insert(i);
        cols.push_back({i});
        float radius = getRadius(squares[i]);
        cv::Point sqCenter = center(squares[i]);
        for(size_t j = i+1; j < squares.size(); ++j) {
            if(done.count(j)) {
                continue;
            }
            if(fabs(sqCenter.x - center(squares[j]).x) < radius) {
                cols.back().push_back(j);
                done.insert(j);
            }
        }
    }

    //Check if nb of rows = nb of cols
    std::vector<int> knownRowsY;
    for(const std::vector<size_t>& col : cols) {
        for(size_t sqId : col) {
            float radius = getRadius(squares[sqId]);
            int yCenter = center(squares[sqId]).y;
            bool unknown = true;
            for(int rowY : knownRowsY) {
                if(fabs(yCenter - rowY) < radius) {
                    unknown = false;
                }
            }
            if(unknown) {
                knownRowsY.push_back(yCenter);
            }
        }
    }
    if(knownRowsY.size() != cols.size()) {
        fprintf(stderr, "The detected squares don't have the same number of row and columns\n");
        return;
    }

    //Sort columns
    std::sort(cols.begin(), cols.end(), [&](const std::vector<size_t>& a, const std::vector<size_t>& b) {
        return center(squares[a[0]]).x < center(squares[b[0]]).x;
    });

    //Sort rows
    for(std::vector<size_t>& col : cols) {
        std::sort(col.begin(), col.end(), [&](size_t a, size_t b) {
            return center(squares[a]).y < center(squares[b]).y;
        });
    }

    //Fill the holes!
    for(size_t i = 0; i < cols.size(); ++i) {
        std::vector<int> colPos;
        float avgRadius = 0;
        int colsChecked = 0;

        for(size_t j = 0; j < cols.size(); ++j) {
            if(i >= cols[j].size()) {
                continue;
            }
            cv::Point sqCenter = center(squares[cols[j][i]]);
            int radius = getRadius(squares[cols[j][i]]);
            avgRadius += radius;
            ++colsChecked;
            bool unknown = true;
            for(int pos : colPos) {
                if(fabs(sqCenter.y - pos) < radius) {
                    unknown = false;
                }
            }
            if(unknown) {
                colPos.push_back(sqCenter.y);
            }
        }

        avgRadius /= colsChecked;
        int firstRow = *std::min_element(colPos.begin(), colPos.end());

        for(size_t j = 0; j < cols.size(); ++j) {
            if(i >= cols[j].size() || fabs(center(squares[cols[j][i]]).y - firstRow) > avgRadius) { //Missing square
                int radius_2 = avgRadius / 2; //The square is probably missing because of a bad sticker
                int avgX = 0;
                for(size_t sqId : cols[j]) {
                    avgX += center(squares[sqId]).x;
                }
                avgX /= cols[j].size();
                Square square = {{cv::Point(avgX - radius_2, firstRow - radius_2), cv::Point(avgX + radius_2, firstRow - radius_2), cv::Point(avgX + radius_2, firstRow + radius_2), cv::Point(avgX - radius_2, firstRow + radius_2)}};
                cols[j].insert(cols[j].begin() + i, squares.size());
                squares.push_back(square);
            }
        }
    }

    for(const std::vector<size_t>& col : cols) {
        if(col.size() != cols.size()) {
            fprintf(stderr, "The adjusted square list is not a square\n");
            return;
        }
    }

    output.resize(cols.size());
    for(size_t i = 0; i < cols.size(); ++i) {
        for(size_t j = 0; j < cols[i].size(); ++j) {
            output[j].push_back(squares[cols[i][j]]);
        }
    }
}

void Vision::drawSquares(const cv::Mat& img, const std::vector<Square>& squares, cv::Mat drawing, bool fill) {
    if(squares.empty()) {
        return;
    }
    for(size_t i = 0; i < squares.size(); ++i) {
        const Square& square = squares[i];
        cv::Scalar color = detectColor(img, square);
        if(fill) {
            int nb = 4;
            const cv::Point* data = square.data();
            cv::fillPoly(drawing, &data, &nb, 1, color);
        } else {
            for(int j = 0; j < 4; ++j) {
                cv::line(drawing, square[j], square[(j+1)%4], color, 1, CV_AA);
            }
        }
    }
}

float Vision::colorDistance(cv::Scalar c1, cv::Scalar c2) {
    int l = (int)c1[0] - (int)c2[0];
    int a = (int)c1[1] - (int)c2[1];
    int b = (int)c1[2] - (int)c2[2];
    return sqrt(l*l + a*a + b*b);
}

void Vision::groupColors(const std::vector<cv::Scalar>& colors, std::vector<int>& output, int tresh) {
    int nextColor = 0;
    output.assign(colors.size(), -1);
    for(size_t i = 0; i < colors.size(); ++i) {
        if(output[i] == -1) {
            output[i] = nextColor;
            for(size_t j = i+1; j < colors.size(); ++j) {
                if(output[j] == -1 && colorDistance(colors[i], colors[j]) < tresh) {
                    output[j] = nextColor;
                }
            }
            ++nextColor;
        }
    }
}

cv::Scalar Vision::detectColor(const cv::Mat& img, const Square& square) {
    return cv::mean(img(squareToRect(square)));
}

bool Vision::isSquare(const std::vector<cv::Point>& approx, int minArea, int distDiff, int axisDiff) {
    //A square must have 4 edges and shall not have a small area (this remove artifacts)
    if(approx.size() == 4 && fabs(cv::contourArea(cv::Mat(approx))) > minArea) {
        int lowestDist = std::numeric_limits<int>::max();
        int highestDist = 0;
        for(int i = 0; i < 4; ++i) {
            cv::Point p1 = approx[i];
            cv::Point p2 = approx[(i+1)%4];
            int dist = (int)distance(p1, p2);
            if(dist > highestDist) {
                highestDist = dist;
            }
            if(dist < lowestDist) {
                lowestDist = dist;
            }
        }
        //A square must have the edges of the same length
        if((highestDist - lowestDist) <= distDiff) {
            //A square must be axis-aligned
            int lastAlign = -1;
            for(int i = 0; i < 4; ++i) {
                cv::Point edge = approx[i] - approx[(i+1)%4];
                float edgeLength = distance(cv::Point(0, 0), edge);
                if(fabs(edge.x / edgeLength)*100 < axisDiff && lastAlign != 0) {
                    lastAlign = 0;
                    continue;
                }
                if(fabs(edge.y / edgeLength)*100 < axisDiff && lastAlign != 1) {
                    lastAlign = 1;
                    continue;
                }
                return false;
            }
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

cv::Point Vision::center(const Square& square) {
    cv::Point result;
    result.x = (square[0].x + square[1].x + square[2].x + square[3].x) / 4;
    result.y = (square[0].y + square[1].y + square[2].y + square[3].y) / 4;
    return result;
}

float Vision::getRadius(const Square& square) {
    float radius = 0;
    for(int j = 0; j < 4; ++j) {
        radius += distance(square[j], square[(j+1)%4]);
    }
    return radius / 8;
}

float Vision::distance(const cv::Point& p1, const cv::Point& p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

//Sort square like this:
//0 1
//3 2
Vision::Square Vision::sortSquare(const Square& square) {
    int pt0 = 0;
    int pt2 = 0;
    for(int i = 1; i < 4; ++i) {
        int dist = square[i].x + square[i].y;
        int pt0Dist = square[pt0].x + square[pt0].y;
        int pt2Dist = square[pt2].x + square[pt2].y;
        if(dist < pt0Dist) {
            pt0 = i;
        }
        if(dist > pt2Dist) {
            pt2 = i;
        }
    }
    int pt1 = -1;
    int pt3 = -1;
    for(int i = 0; i < 4; ++i) {
        if(i != pt0 && i != pt2) {
            if(pt1 == -1) {
                pt1 = i;
            } else {
                pt3 = i;
            }
        }
    }
    if(square[pt1].x < square[pt3].x) {
        std::swap(pt1, pt3);
    }
    return {{square[pt0], square[pt1], square[pt2], square[pt3]}};
}

cv::Rect Vision::squareToRect(const Square& square) {
    return cv::Rect(square[0], square[2]);
}

void Vision::detectStickers(int faceNb, bool debug) {
    if(debug && !_debug) {
        fprintf(stderr, "Vision::detectStickers() called with debug = true, but Vision was initialized with debug = false\n");
        return;
    }

    cv::Mat frame;
    bool success = _cap.read(frame);
    if (!success) {
        fprintf(stderr, "Cannot read a frame from video stream\n");
        return;
    }

    if(_step == 0 && debug) {
        imshow("Win", frame);
    } else {
        cv::medianBlur(frame, frame, 5);
        if(_step == 1 && debug) {
            imshow("Win", frame);
        } else {
            cv::Mat frameHsv;
            cv::cvtColor(frame, frameHsv, CV_RGB2HSV);
            std::vector<cv::Mat> channels(3);
            cv::split(frameHsv, channels);
            cv::Mat grey = channels[2];
            cv::medianBlur(grey, grey, 3);
            cv::medianBlur(grey, grey, 3);
            cv::GaussianBlur(grey, grey, cv::Size(5, 5), 0, 0);
            if(_step == 2 && debug) {
                imshow("Win", grey);
            } else {
                cv::Mat drawing = cv::Mat::zeros(frame.size(), CV_8UC3);

                std::vector<Square> squares;
                detectSquares(grey, squares);
                if(_step == 3 && debug) {
                    drawSquares(frame, squares, drawing, false);
                    imshow("Win", drawing);
                } else {
                    std::vector<Square> similarSquares;
                    keepSimilar(squares, similarSquares);
                    std::vector<Square> simplifiedSquares;
                    simplifySquares(similarSquares, simplifiedSquares);
                    std::vector<std::vector<Square>> ordered;
                    orderSquares(simplifiedSquares, ordered);
                    printf("Detected a %dx%d face\n", ordered.size(), ordered.size());
                    cv::Mat frameLab;
                    cv::cvtColor(frame, frameLab, CV_RGB2Lab);
                    if(faceNb != -1 && ordered.size() != 0) {
                        _stickers[faceNb].assign(ordered.size(), std::vector<cv::Scalar>(ordered.size()));
                        for(size_t i = 0; i < ordered.size(); ++i) {
                            for(size_t j = 0; j < ordered[i].size(); ++j) {
                                _stickers[faceNb][i][j] = detectColor(frameLab, ordered[i][j]);
                            }
                        }
                    }
                    if(_step == 4 && debug) {
                        simplifiedSquares.clear();
                        for(const auto& vec : ordered) {
                            simplifiedSquares.insert(simplifiedSquares.end(), vec.begin(), vec.end());
                        }
                        drawSquares(frame, simplifiedSquares, drawing, true);
                        imshow("Win", drawing);
                    }
                }
            }
        }
    }
}

void Vision::updateDebug() {
    detectStickers(-1, true);
}

void Vision::scanCube(std::vector<std::string>& output) {
    int faceNb = 0;
    while(true) {
        updateDebug();
        if(cv::waitKey(30) == 32) {
            detectStickers(faceNb, false);
            ++faceNb;
            if(faceNb == 6) {
                break;
            }
        }
    }

    std::vector<cv::Scalar> colors;
    for(int i = 0; i < 6; ++i) {
        for(size_t j = 0; j < _stickers[i].size(); ++j) {
            colors.insert(colors.end(), _stickers[i][j].begin(), _stickers[i][j].end());
        }
    }

    std::vector<int> groupedColors;

    bool done = false;
    int colorTresh = 1;
    while(!done) {
        groupColors(colors, groupedColors, colorTresh);
        if(*std::max_element(groupedColors.begin(), groupedColors.end()) == 5) {
            done = true;
        } else {
            ++colorTresh;
        }
    }

    int currentPos = 0;
    for(int i = 0; i < 6; ++i) {
        int squareSize = _stickers[i].size() * _stickers[i].size();
        output.push_back("");
        for(int j = 0; j < squareSize; ++j) {
            output.back().push_back('0' + groupedColors[currentPos + j]);
        }
        currentPos += squareSize;
    }
}
