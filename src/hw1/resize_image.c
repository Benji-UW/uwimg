#include <math.h>
#include "image.h"

float nn_interpolate(image im, int c, float h, float w)
{
    // TODO
    int h_int = (int)roundf(h);
    int w_int = (int)roundf(w);

    return get_pixel(im, c, h_int, w_int);
}

image nn_resize(image im, int h, int w)
{
    // TODO
    image resized = make_image(im.c,h,w);

    double scale_h = h / (float)im.h;
    double scale_w = w / (float)im.w;
    double new_value = 0;
    double h_coord = 0.f;
    double w_coord = 0.f;

    for (int c = 0; c < resized.c; c++) {
        for (int h = 0; h < resized.h; h++) {
            for (int w = 0; w < resized.w; w++) {
                h_coord = ((h + 0.5) / scale_h) - 0.5;
                w_coord = ((w + 0.5) / scale_w) - 0.5;
                new_value = nn_interpolate(im, c, h_coord, w_coord);
                set_pixel(resized, c, h, w, new_value);
            }
        }
    }

    return resized;
}

float bilinear_interpolate(image im, int c, float h, float w)
{
    // TODO
    // // start at the top-left
    int h_= (int)floor(h);
    int w_= (int)floor(w);

    float v1 = get_pixel(im, c, h_, w_);
    float v2 = get_pixel(im, c, h_, w_+1);
    float v3 = get_pixel(im, c, h_+1, w_);
    float v4 = get_pixel(im, c, h_+1, w_+1);

    double d1 = w - w_;
    double d2 = w_ - w + 1;
    double d3 = h - h_;
    double d4 = h_ - h + 1;

    double q1 = (v1 * d2) + (v2 * d1);
    double q2 = (v3 * d2) + (v4 * d1);

    return (q1 * d4) + (q2 * d3);
}

image bilinear_resize(image im, int h, int w)
{
    // TODO
    image resized = make_image(im.c,h,w);

    double scale_h = h / (float)im.h;
    double scale_w = w / (float)im.w;
    double new_value = 0;
    double h_coord = 0.f;
    double w_coord = 0.f;

    for (int c = 0; c < resized.c; c++) {
        for (int h = 0; h < resized.h; h++) {
            for (int w = 0; w < resized.w; w++) {
                h_coord = ((h + 0.5) / scale_h) - 0.5;
                w_coord = ((w + 0.5) / scale_w) - 0.5;
                new_value = bilinear_interpolate(im, c, h_coord, w_coord);
                set_pixel(resized, c, h, w, new_value);
            }
        }
    }

    return resized;
}

