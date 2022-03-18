#ifndef PICTURE_H
#define PICTURE_H

#include <png.h>
#include <QPixmap>
#include <QPoint>
#include <math.h>

class Image{
    typedef struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
    } Pixel_table;
    typedef struct {
        int number_of_passes;
        int width;
        int height;
        png_structp png;
        png_infop info;
        png_byte color_type;
        png_byte bit_depth;
        png_bytep *row_pointers;
        Pixel_table **Pixels;
    } Bitmap_t;

public:
    Bitmap_t Bitmap;
    int read_png_file(const char *filename);
    QPoint get_size();
    int save_png_file(const char *filename);
    void close_png_file();
    QPixmap get_pixmap(bool alpha_button);
    void painter(int image_width, int image_height, int x, int y, QColor line_color);
    void painter(int x_min, int Y_min, int image_width, int image_height, int x, int y, QColor line_color);
    void drawLine(int x_min, int Y_min, int image_width, int image_height, int x1, int y1, int x2, int y2, QColor line_color);
    void circle_painter(int image_width, int image_height, int X1, int Y1, int R, QColor line_color);
    void circle_func(int image_width, int image_height, QPoint first_point, QPoint second_point, int width, QColor line_color, bool background, QColor Background_color);
    void RGB_func(int color, int value);
    void split_func(int x, int y, int width, QColor color);
    void square_func(int image_width, int image_height, QPoint first_point, QPoint second_point, int width, QColor line_color, bool background, QColor Background_color);
    void white_func(int image_width, int image_height, QColor line_color);
};

#endif // PICTURE_H
