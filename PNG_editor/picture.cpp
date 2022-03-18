#include "picture.h"
#include <QDebug>


int Image::read_png_file(const char *filename){
    FILE *fp = fopen(filename, "rb");

    Bitmap.png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!Bitmap.png){
        fclose(fp);
        return -1;
    }

    Bitmap.info = png_create_info_struct(Bitmap.png);
    if(!Bitmap.info){
        fclose(fp);
        return -2;
    }
    if(setjmp(png_jmpbuf(Bitmap.png))){
        fclose(fp);
        return -3;
    }

    png_init_io(Bitmap.png, fp);
    png_read_info(Bitmap.png, Bitmap.info);
    Bitmap.width = png_get_image_width(Bitmap.png, Bitmap.info);
    Bitmap.height = png_get_image_height(Bitmap.png, Bitmap.info);
    Bitmap.color_type = png_get_color_type(Bitmap.png, Bitmap.info);
    Bitmap.bit_depth  = png_get_bit_depth(Bitmap.png, Bitmap.info);
    Bitmap.row_pointers = static_cast<png_bytep*>(malloc(sizeof(png_bytep) * Bitmap.height));

    for(int y = 0; y < Bitmap.height; y++) {
        Bitmap.row_pointers[y] = static_cast<png_byte*>(malloc(png_get_rowbytes(Bitmap.png, Bitmap.info)));
    }

    png_read_image(Bitmap.png, Bitmap.row_pointers);
    Bitmap.Pixels = static_cast<Pixel_table**>(malloc(sizeof(Pixel_table *) * Bitmap.height));

    for (int i = 0; i < Bitmap.height; i++) {
        Bitmap.Pixels[i] = static_cast<Pixel_table*>(calloc(sizeof(Pixel_table), Bitmap.width));
    }

    for(int y = 0; y < Bitmap.height; y++){
        png_bytep row = Bitmap.row_pointers[y];
        for(int x = 0; x < Bitmap.width; x++){
            png_bytep px = &(row[x * 4]);
            Bitmap.Pixels[y][x].red = uint8_t(px[0]);
            Bitmap.Pixels[y][x].green = uint8_t(px[1]);
            Bitmap.Pixels[y][x].blue = uint8_t(px[2]);
            Bitmap.Pixels[y][x].alpha = uint8_t(px[3]);
          //  if( Bitmap.Pixels[y][x].red == Bitmap.Pixels[y][x].green == Bitmap.Pixels[y][x].blue == 0){
          //      Bitmap.Pixels[y][x].alpha = 255;
          //  }
        }
    }
    qDebug() << Bitmap.Pixels[51][51].red << Bitmap.Pixels[51][51].green << Bitmap.Pixels[51][51].blue;
    fclose(fp);
    return 0;
}

QPoint Image::get_size(){
    QPoint begin;
    begin.setX(Bitmap.width);
    begin.setY(Bitmap.height);
    return begin;
}

int Image::save_png_file(const char *filename){
    FILE *fp = fopen(filename, "wb");
    if(!fp)
        return -1;

    Bitmap.png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!Bitmap.png){
        fclose(fp);
        return -2;
    }
    Bitmap.info = png_create_info_struct(Bitmap.png);
    if (!Bitmap.info){
        fclose(fp);
        return -3;
    }
    if (setjmp(png_jmpbuf(Bitmap.png))){
        fclose(fp);
        return -4;
    }
    png_init_io(Bitmap.png, fp);
    if (setjmp(png_jmpbuf(Bitmap.png))){
        fclose(fp);
        return -5;
    }

    png_set_IHDR(
                Bitmap.png,
                Bitmap.info,
                static_cast<unsigned int>(Bitmap.width),
                static_cast<unsigned int>(Bitmap.height),
                Bitmap.bit_depth,
                Bitmap.color_type,
                PNG_INTERLACE_NONE,
                PNG_COMPRESSION_TYPE_BASE,
                PNG_FILTER_TYPE_BASE);
    png_write_info(Bitmap.png, Bitmap.info);

    for(int y = 0; y < Bitmap.height; y++) {
        png_bytep row = Bitmap.row_pointers[y];
        for(int x = 0; x < Bitmap.width; x++) {
            png_bytep px = &(row[x * 4]);
            px[0] = png_byte(Bitmap.Pixels[y][x].red);
            px[1] = png_byte(Bitmap.Pixels[y][x].green);
            px[2] = png_byte(Bitmap.Pixels[y][x].blue);
            px[3] = png_byte(Bitmap.Pixels[y][x].alpha);
        }
    }

    if (setjmp(png_jmpbuf(Bitmap.png))){
        fclose(fp);
        return -6;
    }
    png_write_image(Bitmap.png, Bitmap.row_pointers);
    if (setjmp(png_jmpbuf(Bitmap.png))){
        fclose(fp);
        return -7;
    }

    png_write_end(Bitmap.png, nullptr);
    fclose(fp);
    return 0;
}

void Image::close_png_file(){
    for(int y = 0; y < Bitmap.height; y++) {
        free(Bitmap.row_pointers[y]);
        free(Bitmap.Pixels[y]);
    }
    free(Bitmap.row_pointers);
    free(Bitmap.Pixels);
}

QPixmap Image::get_pixmap(bool alpha_button){
    QImage *image = new QImage(Bitmap.width, Bitmap.height, QImage::Format_ARGB32);
    QColor pixel;
    int devination = 0;
    if(!alpha_button)
        devination = 128;
    for (int y = 0; y < Bitmap.height; y++) {
        for (int x = 0; x < Bitmap.width;x++) {
            pixel.setRed(Bitmap.Pixels[y][x].red);
            pixel.setGreen(Bitmap.Pixels[y][x].green);
            pixel.setBlue(Bitmap.Pixels[y][x].blue);
            if(Bitmap.Pixels[y][x].alpha == 0)
                pixel.setAlpha(devination);
            else
                pixel.setAlpha(Bitmap.Pixels[y][x].alpha);
            image->setPixel(x, y, pixel.rgba());
        }
    }
    return QPixmap::fromImage(*image);
}

void Image::painter(int image_width, int image_height, int x, int y, QColor line_color){
    if(0 <= x && 0 <= y && x < image_width && y < image_height){
        Bitmap.Pixels[y][x].red = line_color.red();
        Bitmap.Pixels[y][x].green = line_color.green();
        Bitmap.Pixels[y][x].blue = line_color.blue();
        Bitmap.Pixels[y][x].alpha = 255;
    }
}

void Image::painter(int x_min, int Y_min, int image_width, int image_height, int x, int y, QColor line_color){
    if(x_min <= x && Y_min <= y && x < image_width && y < image_height){
        Bitmap.Pixels[y][x].red = line_color.red();
        Bitmap.Pixels[y][x].green = line_color.green();
        Bitmap.Pixels[y][x].blue = line_color.blue();
        Bitmap.Pixels[y][x].alpha = 255;
    }
}

void Image::drawLine(int x_min, int Y_min, int image_width, int image_height, int x1, int y1, int x2, int y2, QColor line_color) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    painter(x_min, Y_min, image_width, image_height, x2, y2, line_color);
    while(x1 != x2 || y1 != y2)
   {
        painter(x_min, Y_min, image_width, image_height, x1, y1, line_color);
        const int error2 = error * 2;
        //
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }

}

void Image::circle_painter(int image_width, int image_height, int X1, int Y1, int R, QColor line_color){
    int y = R;
    int x = 0;
    int delta = 1 - 2 * R;
    int error = 0;
    while(y >= 0){
        painter(image_width, image_height, X1 + x, Y1 + y, line_color);
        painter(image_width, image_height, X1 + x, Y1 - y, line_color);
        painter(image_width, image_height, X1 - x, Y1 + y, line_color);
        painter(image_width, image_height, X1 - x, Y1 - y, line_color);
        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0)){
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0)){
            delta -= 2 * --y + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }
}

void Image::circle_func(int image_width, int image_height, QPoint first_point, QPoint second_point, int width, QColor line_color, bool background, QColor Background_color){
    int R = sqrt(pow(second_point.x()-first_point.x(), 2) + pow(second_point.y()-first_point.y(), 2)) / 2;
    int X1 = ((second_point.x()-first_point.x()) / 2) + first_point.x();
    int Y1 = ((second_point.y()-first_point.y()) / 2) + first_point.y();
    int devination = 0;
    int min = R;
    bool negative = true;
    while(width != 0){
        if(!negative && R - devination <= 0){
            circle_painter(image_width, image_height, X1, Y1, R - devination, line_color);
            min = R - devination;
            negative = true;
            width--;
        }
        else{
            circle_painter(image_width, image_height, X1, Y1, R + devination, line_color);
            negative = false;
            devination++;
            width--;
        }
    }
    min--;
    if(background){
        while(min != 0){
            circle_painter(image_width, image_height, X1, Y1, min, Background_color);
            min--;
        }
    }
}

void Image::RGB_func(int color, int value){
    switch (color) {
    case 1:
        for(int y = 0; y < Bitmap.height; y++){
            for(int x = 0; x < Bitmap.width; x++){
                Bitmap.Pixels[y][x].red = value;
            }
        }
        break;
    case 2:
        for(int y = 0; y < Bitmap.height; y++){
            for(int x = 0; x < Bitmap.width; x++){
                Bitmap.Pixels[y][x].green = value;
            }
        }
        break;
    case 3:
        for(int y = 0; y < Bitmap.height; y++){
            for(int x = 0; x < Bitmap.width; x++){
                Bitmap.Pixels[y][x].blue = value;
            }
        }
        break;
    }
}

void Image::split_func(int x_parts, int y_parts, int width, QColor color){
    if (!color.isValid())
        color.setRgb(0, 0, 0);

    int y_n = 1;
    int x_n = 1;
    int y_part = (Bitmap.height - (width * (y_parts - 1)))/ y_parts;
    int x_part = (Bitmap.width - (width * (x_parts - 1)))/ x_parts;
    if(x_part <= 0 || y_part <= 0){
        for(int y = 0; y < Bitmap.height; y++){
            for(int x = 0; x < Bitmap.width; x++){
                Bitmap.Pixels[y][x].red = uint8_t(color.red());
                Bitmap.Pixels[y][x].green = uint8_t(color.green());
                Bitmap.Pixels[y][x].blue = uint8_t(color.blue());
                Bitmap.Pixels[y][x].alpha = uint8_t(255);
            }
        }
        return;
    }


    for(int y = 0; y < Bitmap.height; y++){

        if(y > (y_part * y_n) + (width * y_n))
            y_n++;
        if(y_n != y_parts && y > (y_part * y_n) + (width * (y_n - 1)) && y <= (y_part * y_n) + (width * y_n)){
            for(int x = 0; x < Bitmap.width; x++){
                Bitmap.Pixels[y][x].red = uint8_t(color.red());
                Bitmap.Pixels[y][x].green = uint8_t(color.green());
                Bitmap.Pixels[y][x].blue = uint8_t(color.blue());
                Bitmap.Pixels[y][x].alpha = uint8_t(255);
            }
            continue;
        }

        for(int x = 0; x < Bitmap.width; x++){
            if(x > (x_part * x_n) + (width * x_n))
                x_n++;
            if(x_n != x_parts && x > (x_part * x_n) + (width * (x_n - 1)) && x <= (x_part * x_n) + (width * x_n)){
                Bitmap.Pixels[y][x].red = uint8_t(color.red());
                Bitmap.Pixels[y][x].green = uint8_t(color.green());
                Bitmap.Pixels[y][x].blue = uint8_t(color.blue());
                Bitmap.Pixels[y][x].alpha = uint8_t(255);
            }
        }
        x_n = 1;
    }

}

void Image::square_func(int image_width, int image_height, QPoint C, QPoint A, int width, QColor line_color, bool background, QColor Background_color){
    int side = A.x()-C.x();
    if(!(A.y() >= C.y())){
        C.setY(C.y()-side);
    }
    int width_half = width / 2;
    if(width == 1)
        width_half = 1;
    if(width >= side){
        for(int y = C.y()-(width_half); y < C.y()+side+(width_half); y++)
            for(int x = C.x()-(width_half); x < C.x()+side+(width_half); x++)
                painter(image_width, image_height, x, y, line_color);
        return;
    }
    for(int y = C.y()-width_half+1; y < C.y()+side+width_half; y++){
        if((C.y()-width_half <= y && y <= C.y()+width_half) || ((C.y()- width_half + side <= y && y <= C.y()+width_half+side))){
            for(int x = C.x()-width_half+1; x < C.x()+side+width_half; x++)
                painter(image_width, image_height, x, y, line_color);
            continue;
        }
        for(int x = C.x()-width_half+1; x < C.x()+side+width_half; x++){
            if((C.x()-width_half <= x && x <= C.x()+width_half) || ((C.x()-width_half + side <= x && x <= C.x()+width_half+side))){
                painter(image_width, image_height, x, y, line_color);
            }
            if(background)
                if(C.x()+width_half < x && x < C.x()-width_half + side){
                    painter(image_width, image_height, x, y, Background_color);
            }
        }
    }
    int devination = 0;
    bool negative = true;
    while(width != 0){
        if(!negative){
            drawLine(0, 0, image_width, image_height, C.x()-width_half + devination, C.y()-width_half, C.x()+side+width_half,  C.y()+side+width_half - devination, line_color);
            drawLine(0, 0, image_width, image_height, C.x()-width_half + devination, C.y()+side+width_half, C.x()+side+width_half,  C.y()-width_half + devination, line_color);
            negative = true;
            width--;
        }
        else{
            drawLine(0, 0, image_width, image_height, C.x()-width_half , C.y()-width_half + devination, C.x()+side+width_half - devination,  C.y()+side+width_half, line_color);
            drawLine(0, 0, image_width, image_height, C.x()-width_half, C.y()+side+width_half - devination, C.x()+side+width_half - devination,  C.y()-width_half, line_color);
            negative = false;
            devination++;
            width--;
        }
    }
}

void Image::white_func(int image_width, int image_height, QColor line_color){
    qDebug() << line_color;
    for(int y = 0; y < Bitmap.height; y++){
        for(int x = 0; x < Bitmap.width; x++){
            if(Bitmap.Pixels[y][x].red != 255 || Bitmap.Pixels[y][x].green != 255 || Bitmap.Pixels[y][x].blue != 255){
                if(y - 1 >= 0){
                    if(Bitmap.Pixels[y-1][x].red == Bitmap.Pixels[y-1][x].green && Bitmap.Pixels[y-1][x].green == Bitmap.Pixels[y-1][x].blue && Bitmap.Pixels[y-1][x].blue ==  255){
                        painter(image_width, image_height, x, y, line_color);
                        continue;
                    }
                }
                if(y + 1 < image_height){
                    if(Bitmap.Pixels[y+1][x].red == Bitmap.Pixels[y+1][x].green && Bitmap.Pixels[y+1][x].green == Bitmap.Pixels[y+1][x].blue && Bitmap.Pixels[y+1][x].blue ==  255){
                        painter(image_width, image_height, x, y, line_color);
                        continue;
                    }
                }
                if(x - 1 >= 0){
                    if(Bitmap.Pixels[y][x-1].red == Bitmap.Pixels[y][x-1].green && Bitmap.Pixels[y][x-1].green == Bitmap.Pixels[y][x-1].blue && Bitmap.Pixels[y][x-1].blue ==  255){
                        painter(image_width, image_height, x, y, line_color);
                        continue;
                    }
                }
                if(x + 1 < image_width){
                    if(Bitmap.Pixels[y][x+1].red == Bitmap.Pixels[y][x+1].green && Bitmap.Pixels[y][x+1].green == Bitmap.Pixels[y][x+1].blue && Bitmap.Pixels[y][x+1].blue ==  255){
                        painter(image_width, image_height, x, y, line_color);
                        continue;
                    }
                }
            }
        }
    }
}

