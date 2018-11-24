#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "strange_patterns.hpp"
#include "dir.h"

using namespace std;

// Типы алгоритмов для создания узора
enum AlgorithmType {
        STRANGE_PATTERNS_MAIN = 0,
        STRANGE_PATTERNS_EQV,
        STRANGE_PATTERNS_IMP,
        STRANGE_PATTERNS_AND,
        STRANGE_PATTERNS_OR,
        STRANGE_PATTERNS_SQRT,
        STRANGE_PATTERNS_CONCATENATION_1,
        STRANGE_PATTERNS_CONCATENATION_2,
        STRANGE_PATTERNS_CONCATENATION_3,
        STRANGE_PATTERNS_CONCATENATION_4,
        STRANGE_PATTERNS_CRC64,
        STRANGE_PATTERNS_MAIN_RGB,
        STRANGE_PATTERNS_MAX
};

/* функция для создания изображения
 * image - изображение
 * offset_x - смещение по оси X
 * offset_y - смещение по оси Y
 * w - ширина изображения
 * h - высота изображения
 * algorithm_type - тип алгоритма
 * divider - делитель (не задействован)
 */
void generateImage(cv::Mat& image, long long offset_x, long long offset_y, long long w, long long h, int algorithm_type, long long divider);

int main() {
        std::string windowName = "2d_parrent";
        int algorithm_type = STRANGE_PATTERNS_MAIN; // тип алгоритма
        long w = 640, h = 480; // высота и ширина изображения
        long long x = 0, y = 0; // позиция в 2D пространстве
        long long divider = 0;
        long long dx = 1, dy = 1; // скорость смещения в пространстве
        long long ldx = 0, ldy = 0;
        bool isRun = false; // флаг автоматического перемещения в 2D пространстве

        crc64_generate_table();

        std::cout << "A - move left" << std::endl;
        std::cout << "D - move right" << std::endl;
        std::cout << "W - forward movement" << std::endl;
        std::cout << "S - backward movement" << std::endl;
        std::cout << "F - increase speed" << std::endl;
        std::cout << "B - to reset the speed of movement" << std::endl;
        std::cout << "R - enable constant movement" << std::endl;
        std::cout << "T - algorithm type" << std::endl;
        std::cout << "I - save image" << std::endl;
        std::cout << "V - start or stop video recording" << std::endl;

        std::cout << "enter the size of the image" << std::endl;
        std::cin >> w;
        std::cin >> h;
        //std::cout << "enter the type of algorithm (0 - 3)" << std::endl;
        //std::cin >> algorithm_type;

        bool isVideo = true; // флаг возможности записать видео
        bool isRecVideo = false; // флаг процесса записи видео
        mkdir("video");
        cv::VideoWriter writ("video/output.avi",CV_FOURCC('M','J','P','G'),
                24,
                cv::Size(w,
                h));
        if(!writ.isOpened())  {
                printf("Cannot open initialize output avi video!\n" );
                isVideo = false;
        }

        if(w < 10 || h < 10) return 0;

        cv::Mat image;
        generateImage(image, x, y, w, h, algorithm_type, divider);
        while(1) {
                cv::imshow(windowName, image);
                char symbol = cv::waitKey(50);
                if(symbol == 'A' || symbol == 'a') {
                        x -= dx;
                        ldx = -dx;
                        std::cout << x << " : " << y << std::endl;
                        generateImage(image, x, y, w, h, algorithm_type, divider);
                } else
                if(symbol == 'D' || symbol == 'd') {
                        x += dx;
                        ldx = dx;
                        std::cout << x << " : " << y << std::endl;
                        generateImage(image, x, y, w, h, algorithm_type, divider);
                } else
                if(symbol == 'S' || symbol == 's') {
                        y -= dy;
                        ldy = -dy;
                        std::cout << x << " : " << y << std::endl;
                        generateImage(image, x, y, w, h, algorithm_type, divider);
                } else
                if(symbol == 'W' || symbol == 'w') {
                        y += dy;
                        ldy = dy;
                        std::cout << x << " : " << y << std::endl;
                        generateImage(image, x, y, w, h, algorithm_type, divider);
                } else
                if(symbol == 'F' || symbol == 'f') {
                        if(dx < 1024*1024*1024) dx += dx * 1.5;
                        if(dy < 1024*1024*1024) dy += dy * 1.5;
                        if(ldx > 0) ldx = dx;
                        else if(ldx < 0) ldx = -dx;
                        if(ldy > 0) ldy = dy;
                        else if(ldy < 0) ldy = -dy;
                        std::cout << dx << " : " << dy << std::endl;
                } else
                if(symbol == 'B' || symbol == 'b') {
                        dx = 1;
                        dy = 1;
                        ldx = 0;
                        ldy = 0;
                } else
                if(symbol == 'R' || symbol == 'r') {
                        if(isRun == false) isRun = true;
                        else isRun = false;
                } else
                if(symbol == 'T' || symbol == 't') {
                        if(algorithm_type < STRANGE_PATTERNS_MAX) algorithm_type++;
                        else algorithm_type = 0;
                        generateImage(image, x, y, w, h, algorithm_type, divider);
                } else
                if(symbol == 'I' || symbol == 'i') {
                        mkdir("img");
                        std::string filename = "img/img_" + std::to_string(algorithm_type) + "_" + std::to_string(x) + "_" + std::to_string(y) + ".jpg";
                        cv::imwrite(filename, image);
                        std::cout << "save: " << filename << std::endl;
                } else
                if(isVideo && (symbol == 'V' || symbol == 'v')) {
                                if(isRecVideo == false) {
                                        isRecVideo = true;
                                std::cout << "start video recording" << std::endl;
                        } else if(isRecVideo == true) {
                                isRecVideo = false;
                                writ.release();
                                std::cout << "stop video recording" << std::endl;
                        }
                }

                if(isRun == true) {
                        std::cout << x << " : " << y << std::endl;
                        x += ldx;
                        y += ldy;
                        generateImage(image, x, y, w, h, algorithm_type, divider);
                }
                if(isRecVideo == true) {
                        writ.write(image);
                        std::cout << "video recording" << std::endl;
                }
        }
        return 0;
}

void fillImage(cv::Mat& output, long long offset_x, long long offset_y, long long w, long long h, bool (*f)(long long, long long)) {
        for(long long i = 0; i < w; ++i) {
                for(long long j = 0; j < h; ++j) {
                        cv::Point3_<uchar>* p = output.ptr<cv::Point3_<uchar> >(j,i);
                        if((*f)(offset_x + i, offset_y - j)) {
                                p->x = 255;
                                p->y = 120;
                                p->z = 0;
                        } else {
                                p->x = 0;
                                p->y = 0;
                                p->z = 0;
                        } // of
                } // for j
        } // for i
}

void generateImage(cv::Mat& image, long long offset_x, long long offset_y, long long w, long long h, int algorithm_type, long long divider) {
    if(divider <= 0) divider = 1;
    // создадим картинку
    cv::Mat output(cv::Size(w, h), CV_8UC3);
    cv::Scalar backgroundColor(0,0,0);
    output.setTo(backgroundColor);

        switch(algorithm_type) {
        case STRANGE_PATTERNS_MAIN:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_main);
                break;
        case STRANGE_PATTERNS_EQV:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_eqv);
                break;
        case STRANGE_PATTERNS_IMP:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_imp);
                break;
        case STRANGE_PATTERNS_AND:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_and);
                break;
        case STRANGE_PATTERNS_OR:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_or);
                break;
        case STRANGE_PATTERNS_SQRT:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_sqrt);
                break;
        case STRANGE_PATTERNS_CONCATENATION_1:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_concatenation_1);
                break;
        case STRANGE_PATTERNS_CONCATENATION_2:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_concatenation_2);
                break;
        case STRANGE_PATTERNS_CONCATENATION_3:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_concatenation_3);
                break;
        case STRANGE_PATTERNS_CONCATENATION_4:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_concatenation_4);
                break;
        case STRANGE_PATTERNS_CRC64:
                fillImage(output, offset_x, offset_y, w, h, calc_strange_patterns_crc64);
                break;
        case STRANGE_PATTERNS_MAIN_RGB:
                for(long long i = 0; i < w; ++i) {
                        for(long long j = 0; j < h; ++j) {
                                long long gx = offset_x + i;
                                long long gy = offset_y - j;
                                cv::Point3_<uchar>* p = output.ptr<cv::Point3_<uchar> >(j,i);
                                p->y = 0;
                                p->z = 0;
                                p->x = 0;
                                for(int c = 0; c < 8*3; ++c) {
                                        long long temp = gx ^ gy ^ c;
                                        if(BPSW::isprime(std::abs(temp)) == true) {
                                                if(c < 8) p->x = p->x | (1 << c);
                                                else
                                                if(c < 16) p->y = p->y | (1 << (c - 8));
                                                else
                                                if(c < 24) p->z = p->z | (1 << (c - 16));
                                        } // if
                                } // for c
                        } // for j
                } // for i
                break;
        }

        image = output.clone();
        output.release();
}

