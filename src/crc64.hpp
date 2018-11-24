#ifndef CRC64_HPP_INCLUDED
#define CRC64_HPP_INCLUDED

void crc64_generate_table();

long long calculate_crc64(long long crc, const unsigned char* stream, int n);

long long calculate_crc64(long long x, long long y);

#endif // CRC64_HPP_INCLUDED
