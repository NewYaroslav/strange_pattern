#include <iostream>
#include "xwave.h"
#include "BPSW.hpp"
#include "cmath"
#include "dir.h"

using namespace std;

void generateSound(char* data, unsigned long len_data, long long offset_x, long long offset_y, long long w, long long h);

int main() {
    cout << "simple sound!" << endl;
    const int bits_per_sample = 16; // Количество бит в сэмпле. Так называемая “глубина” или точность звучания.
    const int sample_rate = 16000;  // Частота дискретизации.
    const int num_channels = 1;     // Количество каналов.

    int len_sound = 300;
    const int w = 1; const int h = 14;
    long long x = 999 + 100*2000, y = 999;
    long long dx = 500;

    xwave_wave_file example; // создадим структуру файла wav

    // инициализируем структуру
    xwave_init_wave_file(&example, "example.wav", sample_rate, bits_per_sample, num_channels);
    // создадим файл
    xwave_create_wave_file(&example);

    double period[h] = {1.0/523.25, 1.0/587.32, 1.0/659.26, 1.0/698.46, 1.0/784.00, 1.0/880.00, 1.0/987.75,
                        1.0/1046.50, 1.0/1174.60, 1.0/1318.50, 1.0/1396.90, 1.0/1568.00, 1.0/1720.00, 1.0/1975.50};

    double ampl[h];

    char note_data[h];
    char last_note_data[h];
    const int data_block_len = 8000;
    short data_block[data_block_len];

    for(int i = 0; i < sample_rate * len_sound / data_block_len; ++i) {
        generateSound(note_data, h, x, y, w, h);
        for(int n = 0; n < h; ++n) {
            ampl[n] = 0.0;
            if(note_data[n] != last_note_data[n]) {
                if(note_data[n] == 0x01) {
                    ampl[n] = 0.8;
                }
            }
            last_note_data[n] = note_data[n];
        }
        xwave_get_multiple_damped_impulses_mono(data_block, sample_rate, bits_per_sample, period, ampl, h, data_block_len);
        xwave_write_data_block_wave_file(&example, data_block, data_block_len);
        x += dx;
        //y += dx;
    }
    // сохраним изменения в файле и закроем его
    xwave_close_wave_file(&example);

    return 0;
}

void generateSound(char* data, unsigned long len_data, long long offset_x, long long offset_y, long long w, long long h) {
    for(long long i = 0; i < w; ++i) { // по оси x
        for(long long j = 0; j < h; ++j) { // по оси y
            long long gx = offset_x + i;
            long long gy = offset_y - j;
            long long temp = 0;
            temp = gy ^ (long long)std::sqrt(std::abs(gx));
            //temp = gy ^ gx;
            if(BPSW::isprime(std::abs(temp))) {
                data[j] = 0x01;
            } else {
                data[j] = 0;
            }
        }
    }
}
