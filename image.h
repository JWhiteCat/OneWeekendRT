#ifndef IMAGE_H
#define IMAGE_H

#include "rtweekend.h"
#include <sstream>
#include <fstream>

using std::string;
using std::vector;

class image
{
public:
    std::unique_ptr<vector<vec3>> data;

    image(int width, int height)
    {
        std::ostringstream oss;
        oss << "P3\n"
            << width << ' ' << height << "\n255\n";
        meta = oss.str();

        data = std::make_unique<vector<vec3>>(width * height);
    };

    void output()
    {
        std::ofstream outfile("image_mt.ppm");
        if (!outfile)
        {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }

        outfile << meta;

        for (const auto &num : *data)
        {
            write_color(outfile, num);
        }

        outfile.close();
        std::clog << "\nData written to image_mt.ppm" << std::endl;
    }

private:
    string meta = "";

    inline double linear_to_gamma(double linear_component)
    {
        if (linear_component > 0)
            return pow(linear_component, 1.0 / 2.2);

        return 0;
    }

    void write_color(std::ofstream &out, const color &pixel_color)
    {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        // Apply a linear to gamma transform for gamma 2
        r = linear_to_gamma(r);
        g = linear_to_gamma(g);
        b = linear_to_gamma(b);

        // Translate the [0,1] component values to the byte range [0,255].
        static const interval intensity(0.000, 0.999);
        int rbyte = int(256 * intensity.clamp(r));
        int gbyte = int(256 * intensity.clamp(g));
        int bbyte = int(256 * intensity.clamp(b));

        // Write out the pixel color components.
        out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
    }
};

#endif