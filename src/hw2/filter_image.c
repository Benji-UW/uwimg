#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float channel_norm[im.c];

    for (int c = 0; c < im.c; c++) {
        channel_norm[c] = 0.f;
        for (int h = 0; h < im.h; h++) {
            for (int w = 0; w < im.w; w++) {
                channel_norm[c] += get_pixel(im, c, h, w);
            }
        }
    }

    float norm_val;
    for (int c = 0; c < im.c; c++) {
        for (int h = 0; h < im.h; h++) {
            for (int w = 0; w < im.w; w++) {
                norm_val = get_pixel(im,c,h,w) / channel_norm[c];
                set_pixel(im, c, h, w, norm_val);
            }
        }
    }
}

image make_box_filter(int w)
{
    float c = (1.f / w) / w;
    image filter = make_image(1,w,w);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            set_pixel(filter, 1, i, j, c);
        }
    }
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    image to_return = make_image(im.c,im.h,im.w);

    int same_size = (im.c == filter.c);
    int filter_c = 0;
    float new_pixel_value = 0.f;

    int hcenter_ = filter.w / 2;
    for (int c = 0; c < im.c; c++) {
        if (same_size) {
            // Do the thing across multiple channels
            filter_c = c;
        }
        for (int h = 0; h < im.h; h++) {
            for (int w = 0; w < im.w; w++) {
                new_pixel_value = 0.f;

                for (int filter_h = -1 * hcenter_; filter_h < hcenter_; filter_h++) {
                    for (int filter_w = -1 * hcenter_; filter_w < hcenter_; filter_w++) {
                        new_pixel_value += get_pixel(im,c,h+filter_h,w+filter_w) * get_pixel(filter,filter_c,filter_h,filter_w);
                    }
                }
                set_pixel(to_return, c, h, w, new_pixel_value);
            }
        }
    }

    if (preserve) {
        return to_return;
    } else{
        image flat_to_return = make_image(1, im.h, im.w);
        for (int h = 0; h < im.h; h++) {
            for (int w = 0; w < im.w; w++) {
                new_pixel_value = 0.f;
                for (int c = 0; c < im.c; c++) {
                    new_pixel_value += get_pixel(to_return, c, h, w);
                }
                set_pixel(flat_to_return, 0, h, w, new_pixel_value);
            }
        }
        return flat_to_return;
    }
}

image make_highpass_filter()
{
    // TODO
    image high_pass_filter = make_image(1,3,3);

    //set_pixel(image_________,c,h,w, new_val)
    set_pixel(high_pass_filter,0,0,0, 0);
    set_pixel(high_pass_filter,0,0,1, -1);
    set_pixel(high_pass_filter,0,0,2, 0);
    set_pixel(high_pass_filter,0,1,0, -1);
    set_pixel(high_pass_filter,0,1,1, 4);
    set_pixel(high_pass_filter,0,1,2, -1);
    set_pixel(high_pass_filter,0,2,0, 0);
    set_pixel(high_pass_filter,0,2,1, -1);
    set_pixel(high_pass_filter,0,2,2, 0);

    return high_pass_filter;
}

image make_sharpen_filter()
{
    // TODO
    image sharpen = make_image(1,3,3);

    //set_pixel(image_________,c,h,w, new_val)
    set_pixel(sharpen,0,0,0, 0);
    set_pixel(sharpen,0,0,1, -1);
    set_pixel(sharpen,0,0,2, 0);
    set_pixel(sharpen,0,1,0, -1);
    set_pixel(sharpen,0,1,1, 5);
    set_pixel(sharpen,0,1,2, -1);
    set_pixel(sharpen,0,2,0, 0);
    set_pixel(sharpen,0,2,1, -1);
    set_pixel(sharpen,0,2,2, 0);

    return sharpen;
}

image make_emboss_filter()
{
    image emboss = make_image(1,3,3);

    //set_pixel(image_________,c,h,w, new_val)
    set_pixel(emboss,0,0,0, -2);
    set_pixel(emboss,0,0,1, -1);
    set_pixel(emboss,0,0,2, 0);
    set_pixel(emboss,0,1,0, -1);
    set_pixel(emboss,0,1,1, 1);
    set_pixel(emboss,0,1,2, 1);
    set_pixel(emboss,0,2,0, 0);
    set_pixel(emboss,0,2,1, 1);
    set_pixel(emboss,0,2,2, 2);

    return emboss;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: We should use preserve when using the sharpen and emboss filters and we shouldn't use it for the high-pass filter. Sharpening and embossing are stylistic layers to add to an image, therefore we wouldn't want to throw away color, but sharpening is for feature extraction, for which we don't care about color as much.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: We probably want to smooth the sharpened image a bit. To be honest I don't know.

image make_gaussian_filter(float sigma)
{
    // How big should the filter be?
    int filter_size = (int)(6 * sigma) + 1;

    if ((filter_size + 1) % 2) {
        filter_size++;
    }

    printf("Hello there, this code is running :) \n");
    image gauss_filter = make_image(1,filter_size,filter_size);
    int half = filter_size / 2;
    int x,y;
    float G_xy = 0.f;

    for (int h = 0; h < filter_size; h++) {
        for (int w = 0; w < filter_size; w++) {
            x = h - half;
            y = w - half;

            G_xy = (1 / (TWOPI * sigma * sigma)) * (float)exp(-((x*x) + (y*y)) / (2 * sigma * sigma));
            set_pixel(gauss_filter,0,h,w,G_xy);
        }
    }

    l1_normalize(gauss_filter);

    return gauss_filter;
}

image add_image(image a, image b)
{
    assert(a.c == b.c);
    assert(a.h == b.h);
    assert(a.w == b.w);

    image added = make_image(a.c,a.h,a.w);

    for (int c = 0; c < a.c; c++) {
        for (int h = 0; h < a.h; h++) {
            for (int w = 0; w < a.w; w++) {
                set_pixel(added,c,h,w,get_pixel(a,c,h,w) + get_pixel(b,c,h,w));
            }
        }
    }

    l1_normalize(added);

    return added;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.c == b.c);
    assert(a.h == b.h);
    assert(a.w == b.w);

    image subbed = make_image(a.c,a.h,a.w);

    for (int c = 0; c < a.c; c++) {
        for (int h = 0; h < a.h; h++) {
            for (int w = 0; w < a.w; w++) {
                set_pixel(subbed,c,h,w,get_pixel(a,c,h,w) - get_pixel(b,c,h,w));
            }
        }
    }

    l1_normalize(subbed);

    return subbed;
}

image make_gx_filter()
{
    image gx_filter = make_image(1,3,3);

    //set_pixel(image__,c,h,w, new_val)
    set_pixel(gx_filter,0,0,0, -1);
    set_pixel(gx_filter,0,0,1, 0);
    set_pixel(gx_filter,0,0,2, 1);
    set_pixel(gx_filter,0,1,0, -2);
    set_pixel(gx_filter,0,1,1, 0);
    set_pixel(gx_filter,0,1,2, 2);
    set_pixel(gx_filter,0,2,0, -1);
    set_pixel(gx_filter,0,2,1, 0);
    set_pixel(gx_filter,0,2,2, 1);

    return gx_filter;
}

image make_gy_filter()
{
    image gy_filter = make_image(1,3,3);

    //set_pixel(image_________,c,h,w, new_val)
    set_pixel(gy_filter,0,0,0, -1);
    set_pixel(gy_filter,0,0,1, -2);
    set_pixel(gy_filter,0,0,2, -1);
    set_pixel(gy_filter,0,1,0, 0);
    set_pixel(gy_filter,0,1,1, 0);
    set_pixel(gy_filter,0,1,2, 0);
    set_pixel(gy_filter,0,2,0, 1);
    set_pixel(gy_filter,0,2,1, 2);
    set_pixel(gy_filter,0,2,2, 1);

    return gy_filter;
}

void feature_normalize(image im)
{
    // TODO

    float min = 0.f;
    float max = 0.f;

    float val;

    for (int c = 0; c < im.c; c++) {
        for (int h = 0; h < im.h; h++) {
            for (int w = 0; w < im.w; w++) {
                val = get_pixel(im,c,h,w);
                if (val > max) {
                    max = val;
                } else if (val < min) {
                    min = val;
                }
            }
        }
    }

    float range = max - min;

    for (int c = 0; c < im.c; c++) {
        for (int h = 0; h < im.h; h++) {
            for (int w = 0; w < im.w; w++) {
                val = get_pixel(im,c,h,w);

                set_pixel(im,c,h,w,val - min);
            }
        }
    }

    if (range > 0) {
        for (int c = 0; c < im.c; c++) {
            for (int h = 0; h < im.h; h++) {
                for (int w = 0; w < im.w; w++) {
                    val = get_pixel(im,c,h,w);
                    set_pixel(im,c,h,w,(val / range));
                }
            }
        }
    }

}

image *sobel_image(image im)
{
    // TODO
    image Gx = convolve_image(im, make_gx_filter(),0);
    image Gy = convolve_image(im, make_gy_filter(),0);

    // image to_return[2];

    image *to_return = (image*)calloc(2, sizeof(image));

    to_return[0] = make_image(1,im.h,im.w);
    to_return[1] = make_image(1,im.h,im.w);

    float gx,gy,mag,dir;

    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            gx = get_pixel(Gx,0,h,w);
            gy = get_pixel(Gy,0,h,w);

            mag = (float)sqrt((gx*gx) + (gy*gy));
            dir = (float)atan2(gy,gx);

            set_pixel(to_return[0],0,h,w,mag);
            set_pixel(to_return[1],0,h,w,dir);
        }
    }

    feature_normalize(to_return[0]);
    feature_normalize(to_return[1]);

    return to_return;
}

image colorize_sobel(image im)
{
    // TODO
    image *sobel_images = sobel_image(im);
    image colorized = make_image(3,im.h,im.w);

    float mag,dir;

    for (int h = 0; h < im.h; h++) {
        for (int w = 0; w < im.w; w++) {
            mag = get_pixel(sobel_images[0],0,h,w);
            dir = get_pixel(sobel_images[1],0,h,w);

            set_pixel(colorized,0,h,2,dir);
            set_pixel(colorized,1,h,2,mag);
        }
    }
    hsv_to_rgb(colorized);

    free(sobel_images);

    return colorized;
}
