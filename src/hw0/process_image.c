#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int c, int h, int w)
{
    // TODO Fill this in
    // Per the CHW convention:
    int width = im.w;
    int height = im.h;
    int pixels = width * height;

    if (h >= height) {
        h = height - 1;
    } else if (h < 0) {
        h = 0;
    }

    if (w >= width) {
        w = width - 1;
    } else if (w < 0) {
        w = 0;
    }

    int index = (c * pixels) + (h * width) + w;

    return im.data[index];
}

void set_pixel(image im, int c, int h, int w, float v)
{
    if (c > im.c || h > im.h || w > im.w) {
        return;
    }
    int width = im.w;
    int height = im.h;
    int pixels = width * height;

    int index = (c * pixels) + (h * width) + w;

    im.data[index] = v;

    // TODO Fill this in
}

image copy_image(image im)
{
    image copy = make_image(im.c, im.h, im.w);
    // TODO Fill this in
    memcpy(copy.data, im.data, (im.c * im.h * im.w + 1)*4);

    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(1, im.h, im.w);
    // TODO Fill this in

    // int pixels = im.w * im.h;
    float gray_val;
    float r,g,b;
    // int index = (c * pixels) + (h * width) + w;

    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            // r = im.data[(h * im.w) + w];
            r = get_pixel(im, 0, h, w);
            g = get_pixel(im, 1, h, w);
            b = get_pixel(im, 2, h, w);

            gray_val = (0.299 * r) + (0.587 * g) + (0.144 * b);
            if (gray_val > 1.0) {
                gray_val = 1.0;
            }
            set_pixel(gray,0,h,w,gray_val);
        }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    int pixels = im.w * im.h;

    for (int i = 0; i < pixels; i++) {
        float old = get_pixel(im, c, i / im.w, i % im.w);
        set_pixel(im, c, i / im.w, i % im.w, old + v);
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.w; k++) {
                float old = get_pixel(im, i, j, k); 
                if (old > 1.0) {
                    set_pixel(im, i, j, k, 1.0);
                } else if (old < 0.0) {
                    set_pixel(im, i, j, k, 0.0);
                }
            }
        }
    } 
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    float R,G,B;
    float H,S,V;
    float C = 0;
    float m = 0;
    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            R = get_pixel(im, 0, h, w);
            G = get_pixel(im, 1, h, w);
            B = get_pixel(im, 2, h, w);

            V = three_way_max(R,G,B);

            if (V == 0) {
                S = 0.f;
            } else {
                m = three_way_min(R,G,B);
                C = V - m;
                S = C / V;
            }

            if (C == 0) {
                H = 0.f;
            } else if (V == R) {
                // If the max is equal to R:
                H = (G - B) / C;
            } else if (V == G) {
                // If the max is equal to G:
                H = 2 + ((B - R) / C);
            } else {
                // If the max is equal to B:
                H = 4 + ((R - G) / C);
            }

            if (H < 0) {
                H = (H / 6) + 1;
            } else {
                H = (H / 6);
            }

            set_pixel(im, 0, h, w, H);
            set_pixel(im, 1, h, w, S);
            set_pixel(im, 2, h, w, V);
            // if (h == 0 && w ==0) {
            //     printf("RGB Values: %f, %f, %f \n", R, G, B);
            //     printf("HSV Values: %f, %f, %f \n", H, S, V);
            // }
        }
    }
}

void hsv_to_rgb(image im)
{
    float R,G,B;
    float H,S,V;
    float C,m,X;

    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            H = get_pixel(im, 0, h, w);
            S = get_pixel(im, 1, h, w);
            V = get_pixel(im, 2, h, w);

            C = V * S;
            H = H * 6;
            m = V - C;

            X = C * (1 - (float) fabs(fmod(H, 2) - 1));

            if (H < 1) {
                R = C;
                G = X;
                B = 0;
            } else if (H < 2) {
                R = X;
                G = C;
                B = 0;
            } else if (H < 3) {
                R = 0;
                G = C;
                B = X;
            } else if (H < 4) {
                R = 0;
                G = X;
                B = C;
            } else if (H < 5) {
                R = X;
                G = 0;
                B = C;
            } else {
                R = C;
                G = 0;
                B = X;
            }

            set_pixel(im, 0, h, w, R + m);
            set_pixel(im, 1, h, w, G + m);
            set_pixel(im, 2, h, w, B + m);
            // if (h == 0 && w ==0) {
            //     printf("RGB Values: %f, %f, %f \n", R, G, B);
            //     printf("HSV Values: %f, %f, %f \n", H, S, V);
            // }
        }
    }
}
