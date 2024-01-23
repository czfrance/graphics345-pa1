/*
 *  Copyright 2024 <me>
 */

#include "starter_canvas.h"
# include <stdio.h>

void MyCanvas::clear(const GColor& color) {
    for (int y = 0; y < fDevice.height(); ++y) {
        for (int x = 0; x < fDevice.width(); ++x) {
            //this is an address
            GPixel* p = fDevice.getAddr(x, y);
            GPixel newP = GPixel_PackARGB(GRoundToInt(color.a * 255), GRoundToInt(color.r * color.a * 255), GRoundToInt(color.g * color.a * 255), GRoundToInt(color.b * color.a * 255));
            //this is a dereference, this means you can put stuff in it
            *p = newP;
        }
    }
}

void MyCanvas::fillRect(const GRect& rect, const GColor& color) {
    //find the actual bounds
    GRect r = GRect::LTRB(std::max(GRoundToInt(rect.left), 0), std::max(GRoundToInt(rect.top), 0),
                       std::min(GRoundToInt(rect.right), fDevice.width()), std::min(GRoundToInt(rect.bottom), fDevice.height()));

    int top = GRoundToInt(r.top);
    int bottom = GRoundToInt(r.bottom);
    int left = GRoundToInt(r.left);
    int right = GRoundToInt(r.right);
    
    if (color.a == 0.0f) {
        ;
    }

    else if (color.a == 1.0f) {
        GPixel newP = GPixel_PackARGB(GRoundToInt(color.a * 255), GRoundToInt(color.r * 255), 
        GRoundToInt(color.g * 255), GRoundToInt(color.b * 255));

        for (int y = top; y < bottom; ++y) {
            // printf("(%d, %d)\n", left, y);
            // GPixel* curr = fDevice.getAddr(left, y);
            for (int x = left; x < right; ++x) {
                // printf("(%d, %d)\n", x, y);
                GPixel* curr = fDevice.getAddr(x, y);
                *curr = newP;
                // curr++;
                //just need to comput the first address, and then go to the next one after
            }
        }
    }

    else {
        //turn color into premult
        int sr_pm = GRoundToInt(color.r * color.a * 255);
        int sg_pm = GRoundToInt(color.g * color.a * 255);
        int sb_pm = GRoundToInt(color.b * color.a * 255);
        int sa_pm = GRoundToInt(color.a * 255);

        //calculate the blend
        for (int y = top; y < bottom; ++y) {
            for (int x = left; x < right; ++x) {
                GPixel* curr = fDevice.getAddr(x, y);

                int da = GPixel_GetA(*curr);
                int dr = GPixel_GetR(*curr);
                int dg = GPixel_GetG(*curr);
                int db = GPixel_GetB(*curr);

                //use the equation
                int fr = sr_pm + ((((255-sa_pm)*dr) + 128) * 257 >> 16);
                int fg = sg_pm + ((((255-sa_pm)*dg) + 128) * 257 >> 16);
                int fb = sb_pm + ((((255-sa_pm)*db) + 128) * 257 >> 16);
                int fa = sa_pm + ((((255-sa_pm)*da) + 128) * 257 >> 16);

                //put the value back into the pixel itself
                GPixel newP = GPixel_PackARGB(fa, fr, fg, fb);
                //this is a dereference, this means you can put stuff in it
                *curr = newP;
            }
        }
    }
    
}

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap& device) {
    return std::unique_ptr<GCanvas>(new MyCanvas(device));
}

std::string GDrawSomething(GCanvas* canvas, GISize dim) {
    // as fancy as you like
    // ...
    // canvas->clear(...);
    // canvas->fillRect(...);

    //size of canvas: 256x256

    //make the background
    GRect rect = GRect::XYWH(0, -10, dim.width, 30);

    int delta = 8;
    for (int i = 0; i < 200; i+=2) {
        float r = i / 200.0f;
        float g = std::abs(std::cos(i/40.0f));
        float b = std::abs(std::sin(i/50.0f));
        GColor color = {r, g, b, 0.3f};
        canvas->fillRect(rect, color);
        rect = rect.offset(0, delta);
    }


    GRect rect2 = GRect::XYWH(dim.width / 8, dim.height/4, 20, 20);

    int delta2 = 5;
    for (int out = 0; out < dim.width/2; out+=delta2) {
        for (int i = 0; i < dim.width/2; i+=delta2) {
            float r = std::abs(std::sin(i/50.0f));
            float g = std::abs(std::sin(i/100.0f));
            float b = i / dim.width/2;
            GColor color = {r, g, b, 0.5f};
            canvas->fillRect(rect2, color);
            rect2 = rect2.offset(delta2, 0);
        }
        rect2 = rect2.offset(-dim.width/2, delta2);
    }

    GRect rect3 = GRect::XYWH(dim.width-(dim.width / 2.5), dim.height/2.5, 10, 10);

    int delta3 = 2;
    for (int out = 0; out < dim.width/4; out+=delta3) {
        for (int i = dim.width/4; i > 0; i-=delta3) {
            float r = std::abs(std::sin(i/80.0f));
            float g = i / dim.width/2;
            float b = std::abs(std::sin(i/30.0f));
            GColor color = {r, g, b, 0.5f};
            canvas->fillRect(rect3, color);
            rect3 = rect3.offset(-delta3, 0);
        }
        rect3 = rect3.offset(dim.width/4, delta3);
    }

    return "my drawing";
}
