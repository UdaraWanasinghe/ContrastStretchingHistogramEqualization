#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

void show_histogram(Mat image, String name);

void histogram_equalization(Mat image);

void contrast_stretching(Mat image);

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Image path is required" << endl;
        return 128;
    }
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);
    imshow("Image", image);
    show_histogram(image, "Initial");
    Mat image2 = image.clone();
    contrast_stretching(image2);
    imshow("contrast_stretching", image2);
    show_histogram(image2, "_contrast_stretching");
    Mat image3 = image.clone();
    histogram_equalization(image3);
    imshow("histogram_equalization", image3);
    show_histogram(image3, "_histogram_equalization");
    waitKey(0);
}

void histogram_equalization(Mat image) {
    int arr[256] = {0};
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int val = (int) image.at<uchar>(i, j);
            arr[val]++;
        }
    }
    int total = image.rows * image.cols;
    int transform[256] = {0};
    int sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += arr[i];
        int y = sum * 255 / total;
        transform[i] = y;
    }
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int val = (int) image.at<uchar>(i, j);
            image.at<uchar>(i, j) = transform[val];
        }
    }
}

void contrast_stretching(Mat image) {
    // get histogram
    int arr[256] = {0};
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int val = (int) image.at<uchar>(i, j);
            arr[val]++;
        }
    }
    // calculate c and d
    int a = 150;
    int b = 200;
    int total = image.rows * image.cols;
    int depth[3] = {0};
    for (int i = 0; i < a; i++) {
        depth[0] += arr[i];
    }
    for (int i = a; i < b; i++) {
        depth[1] += arr[i];
    }
    for (int i = b; i < 256; i++) {
        depth[2] += arr[i];
    }
    int c = 255 * depth[0] / total;
    int d = c + 255 * depth[1] / total;
    // calculate transform matrix
    int transform[256];
    for (int i = 0; i < 256; i++) {
        if (i < a) {
            transform[i] = c * i / a;
        } else if (i < b) {
            transform[i] = c + (i - a) * (d - c) / (b - a);
        } else {
            transform[i] = d + (i - b) * (255 - d) / (255 - b);
        }
    }
    // transform
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int val = (int) image.at<uchar>(i, j);
            image.at<uchar>(i, j) = transform[val];
        }
    }
}

void show_histogram(Mat image, String name) {
    int arr[256] = {0};
    int max = 0;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int val = (int) image.at<uchar>(i, j);
            arr[val]++;
            if (max < arr[val]) {
                max = arr[val];
            }
        }
    }
    Mat histogram = Mat(256, 256, CV_8UC1, Scalar(255));
    for (int i = 0; i < 256; i++) {
        Point start = Point(i, 255);
        int h = 255 * arr[i] / max;
        Point end = Point(i, 255 - h);
        line(histogram, start, end, Scalar(0));
    }
    imshow(name, histogram);
}