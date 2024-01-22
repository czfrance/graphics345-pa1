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

    //calculate the blend
    for (int y = GRoundToInt(r.top); y < GRoundToInt(r.bottom); ++y) {
        for (int x = GRoundToInt(r.left); x < GRoundToInt(r.right); ++x) {
            //TODO

            // 1. check for alpha = 0 and alpha = 255 first!!!
            // else:
            GPixel* curr = fDevice.getAddr(x, y);

            // if (color.a == 0.0) {
            //     ;
            // }

            // else if (color.a == 1.0) {
            //     GPixel newP = GPixel_PackARGB(GRoundToInt(color.a * 255), GRoundToInt(color.r * 255), 
            //     GRoundToInt(color.g * 255), GRoundToInt(color.b * 255));
            //     *curr = newP;
            // }

            else {
                int da = GPixel_GetA(*curr);
                int dr = GPixel_GetR(*curr);
                int dg = GPixel_GetG(*curr);
                int db = GPixel_GetB(*curr);

                //turn color into premult
                int sr_pm = GRoundToInt(color.r * color.a * 255);
                int sg_pm = GRoundToInt(color.g * color.a * 255);
                int sb_pm = GRoundToInt(color.b * color.a * 255);
                int sa_pm = GRoundToInt(color.a * 255);

                //use the equation
                int fr = sr_pm + (((255-sa_pm)*dr)/255);
                int fg = sg_pm + (((255-sa_pm)*dg)/255);
                int fb = sb_pm + (((255-sa_pm)*db)/255);
                int fa = sa_pm + (((255-sa_pm)*da)/255);

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

    return "tears in rain";
}
