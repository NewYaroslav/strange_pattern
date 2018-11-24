#include "crc64.hpp"

static const long long poly = 0xC96C5795D7870F42;

static long long crc64_table[256];

void crc64_generate_table() {
    for(int i=0; i<256; ++i) {
    	long long crc = i;
    	for(int j=0; j<8; ++j) {
            // is current coefficient set?
    		if(crc & 1) {
                // yes, then assume it gets zero'd (by implied x^64 coefficient of dividend)
                crc >>= 1;

                // and add rest of the divisor
                crc ^= poly;
            } else {
                // no? then move to next coefficient
                crc >>= 1;
            }
    	}
        crc64_table[i] = crc;
    }
}

long long calculate_crc64(long long crc, const unsigned char* stream, int n) {
    for(int i=0; i< n; ++i) {
        unsigned char index = stream[i] ^ crc;
        long long lookup = crc64_table[index];
        crc >>= 8;
        crc ^= lookup;
    }
    return crc;
}

long long calculate_crc64(long long x, long long y) {
    unsigned char buf[16];
    for(int i = 0; i < 8; ++i) {
        buf[i] = x >> i*8;
        buf[i+8] = y >> i*8;
    }
    return calculate_crc64(0, buf, 16);
}
