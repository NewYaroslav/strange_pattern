#ifndef STRANGE_PATTERNS_HPP_INCLUDED
#define STRANGE_PATTERNS_HPP_INCLUDED

#include "BPSW.hpp"
#include "crc64.hpp"
#include "cmath"

/** \brief Операция конкантенации
 * \param a первое число
 * \param b второй число
 * \return результат конкантенации
 */
inline long long get_concatenation(long long a, long long b)
{
        for(long long i = 10; i < 1000000000000000000; i+=10) {
                if(b % i < 10) {
                        return a * i + abs(b);
                }
        }
        return a + b;
}

/** \brief Основной алгоритм для создания странного узора
 * \param x координаты по оси X
 * \param y координаты по оси Y
 * \return вернет true если пиксель закрашен
 */
inline bool calc_strange_patterns_main(long long x, long long y)
{
        long long temp = x ^ y;
        return BPSW::isprime(std::abs(temp));
}

/** \brief Алгоритм для создания странного узора на основе логической операции OR
 * Данный алгоритм дает узоры похожие на фрактальные треугольгики
 * \param x координаты по оси X
 * \param y координаты по оси Y
 * \return вернет true если пиксель закрашен
 */
inline bool calc_strange_patterns_or(long long x, long long y)
{
        long long temp = x | y;
        return BPSW::isprime(std::abs(temp));
}

/** \brief Алгоритм для создания странного узора на основе логической операции AND
 * Данный алгоритм дает узоры похожие на фрактальные треугольгики
 * \param x координаты по оси X
 * \param y координаты по оси Y
 * \return вернет true если пиксель закрашен
 */
inline bool calc_strange_patterns_and(long long x, long long y)
{
        long long temp = x & y;
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_sqrt(long long x, long long y)
{
        long long temp = y | (long long)std::sqrt(std::abs(x));
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_concatenation_1(long long x, long long y)
{
        long long temp = get_concatenation(x, y);
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_concatenation_2(long long x, long long y)
{
        long long temp = get_concatenation(x ^ y, x & y);
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_concatenation_3(long long x, long long y)
{
        long long temp = get_concatenation(x * y, x + y);
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_concatenation_4(long long x, long long y)
{
        long long temp = get_concatenation(get_concatenation(x ^ y, x | y), get_concatenation(x ^ y, x | y));
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_crc64(long long x, long long y)
{
        long long temp = calculate_crc64(x, y);
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_imp(long long x, long long y)
{
        long long temp = ~x | y;
        return BPSW::isprime(std::abs(temp));
}

inline bool calc_strange_patterns_eqv(long long x, long long y)
{
        long long temp = ~(x ^ y);
        return BPSW::isprime(std::abs(temp));
}

#endif // STRANGE_PATTERNS_HPP_INCLUDED
