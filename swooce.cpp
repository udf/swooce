#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <optional>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

struct Kernel {
    ssize_t w = 0;
    ssize_t h = 0;
    std::unique_ptr<float[]> data;
};

static void usage() {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "./swooce [kernel] [input] [output]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Kernel format:" << std::endl;
    std::cerr << "W H values... [scale]" << std::endl;
    std::cerr << "If scale is not present then the kernel is ";
    std::cerr << "scaled so it sums to 1." << std::endl;
}

static std::optional<Kernel> make_kernel(const char *str) {
    const auto err = [](const std::string msg) {
        std::cerr << msg << std::endl;
        return std::nullopt;
    };
    Kernel k;
    std::istringstream ss(str);

    if (!(ss >> k.w) || k.w <= 0)
        return err("Invalid kernel width");
    if (!(ss >> k.h) || k.h <= 0)
        return err("Invalid kernel height");

    const size_t num_values = k.w * k.h;
    k.data = std::make_unique<float[]>(num_values);
    for (size_t i = 0; i < num_values; i++) {
        if (!(ss >> k.data[i]))
            return err("Error reading value #" + std::to_string(i + 1));
    }

    float scale = 0;
    if (!(ss >> scale)) {
        // normalize if we didn't get a scale value
        float sum = 0;
        for (size_t i = 0; i < num_values; i++)
            sum += k.data[i];
        if (sum != 0)
            scale = 1.f / sum;
    }

    for (size_t i = 0; i < num_values; i++)
        k.data[i] *= scale;

    return k;
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        if (argc < 4) {
            std::cerr << "Too few arguments!";
        } else {
            std::cerr << "Too many arguments! Did you forget to put your kernel in quotes?";
        }
        std::cerr << std::endl;
        usage();
        return 1;
    }

    auto kd = make_kernel(argv[1]);
    if (!kd)
        return 2;
    auto input = argv[2];
    auto output = argv[3];

    auto img = cv::imread(input, cv::IMREAD_UNCHANGED);
    if (img.empty()) {
        std::cerr << "Failed to load input image" << std::endl;
        return 3;
    }

    const size_t num_channels = img.channels();
    cv::Mat channels[num_channels];
    cv::split(img, channels);

    auto &k = kd.value();
    cv::Mat kernel(k.h, k.w, CV_32F, k.data.get());

    #pragma omp parallel for
    for (size_t i = 0; i < num_channels; i++) {
        cv::filter2D(channels[i], channels[i], -1, kernel);
    }

    cv::merge(channels, num_channels, img);
    cv::imwrite(output, img);

    return 0;
}