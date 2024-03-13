#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <thread>
#include <vector>
#include <chrono>

void getFramesFromVideo(const std::string &src_dir, const std::string &dest_dir);
void processVideo(const std::string &src_vid, const std::string &dest_dir);

using namespace std::filesystem;

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string root_src = "/home/rob/Documents/000_data/0_datasets/LDPolyp/";
    std::string src_normal = root_src + "videos_without_polyps/";
    std::string src_anomaly = root_src + "videos_with_polyps/";

    std::string root_dest = "/home/rob/Documents/3_projects/anomalies/datasets/LDPolyp/";
    std::string dest_normal = root_dest + "normal/";
    std::string dest_anomaly = root_dest + "anomaly/";

    getFramesFromVideo(src_normal, dest_normal);
    auto middle = std::chrono::high_resolution_clock::now();
    auto normalDuration = duration_cast<std::chrono::microseconds>(middle - start);

    getFramesFromVideo(src_anomaly, dest_anomaly);
    auto stop = std::chrono::high_resolution_clock::now();
    auto anomalyDuration = duration_cast<std::chrono::microseconds>(stop - start);
    auto totalDuration = duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "In total, it took : " << totalDuration.count() << " to convert all videos to frames." << std::endl;
    std::cout << "For Normal, it took : " << normalDuration.count() << " to convert all videos to frames." << std::endl;
    std::cout << "For Anomaly, it took : " << anomalyDuration.count() << " to convert all videos to frames." << std::endl;

    return 0;
}

void processVideo(const std::string &src_vid, const std::string &dest_dir) {
    std::string vid_name = std::filesystem::path(src_vid).filename().string();
    std::cout << "Thread | Processing video in | " << src_vid << std::endl;

    // Open video
    cv::VideoCapture cap(src_vid);
    if (!cap.isOpened()) {
        std::cerr << "Error. Could not open video at path: " << src_vid << std::endl;
        return;
    }

    // Create frames
    cv::Mat frame;
    int frameNb = 0;

    while (cap.read(frame)) {
        std::string filePath = dest_dir + vid_name + "_frame_" + std::to_string(frameNb++) + ".jpg";
        cv::imwrite(filePath, frame);
    }
}

void getFramesFromVideo(const std::string &src_dir, const std::string &dest_dir) {
    std::vector<std::thread> threads;

    for (const auto& dirEntry : recursive_directory_iterator(src_dir)) {
        std::string src_vid = dirEntry.path().string();

        threads.emplace_back(processVideo, src_vid, dest_dir);
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
