/*
    Тест BPSW на простоту чисел
    Источник: http://e-maxx.ru/algo/bpsw
*/

#ifndef BPSW_HPP_INCLUDED
#define BPSW_HPP_INCLUDED

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>

namespace BPSW {

    //! Модуль 64-битного числа
    long long abs (long long n) {
        return n < 0 ? -n : n;
    }

    unsigned long long abs (unsigned long long n) {
        return n;
    }

    //! Возвращает true, если n четное
    template <class T>
    bool even (const T & n) {
        // return n % 2 == 0;
        return (n & 1) == 0;
    }

    //! Делит число на 2
    template <class T>
    void bisect (T & n) {
        // n /= 2;
        n >>= 1;
    }

    //! Умножает число на 2
    template <class T>
    void redouble (T & n) {
        // n *= 2;
        n <<= 1;
    }

    //! Возвращает true, если n - точный квадрат простого числа
    template <class T>
    bool perfect_square (const T & n) {
        T sq = (T) ceil (sqrt ((double)n));
        return sq*sq == n;
    }

    //! Вычисляет корень из числа, округляя его вниз
    template <class T>
    T sq_root (const T & n) {
        return (T) floor (sqrt ((double) n));
    }

    //! Возвращает количество бит в числе (т.е. минимальное количество бит, которыми можно представить данное число)
    template <class T>
    unsigned bits_in_number (T n) {
        if (n == 0)
            return 1;
        unsigned result = 0;
        while (n) {
            bisect (n);
            ++result;
        }
        return result;
    }

    //! Возвращает значение k-го бита числа (биты нумеруются с нуля)
    template <class T>
    bool test_bit (const T & n, unsigned k) {
        return (n & (T(1) << k)) != 0;
    }

    //! Умножает a *= b (mod n)
    template <class T>
    void mulmod (T & a, T b, const T & n) {
        // наивная версия, годится только для длинной арифметики
        a *= b;
        a %= n;
    }

    template <>
    void mulmod (int & a, int b, const int & n) {
        a = int( (((long long)a) * b) % n );
    }

    template <>
    void mulmod (unsigned & a, unsigned b, const unsigned & n) {
        a = unsigned( (((unsigned long long)a) * b) % n );
    }

    template <>
    void mulmod (unsigned long long & a, unsigned long long b, const unsigned long long & n) {
        // сложная версия, основанная на бинарном разложении произведения в сумму
        if (a >= n)
            a %= n;
        if (b >= n)
            b %= n;
        unsigned long long res = 0;
        while (b)
            if (!even (b))
            {
                res += a;
                while (res >= n)
                    res -= n;
                --b;
            }
            else
            {
                redouble (a);
                while (a >= n)
                    a -= n;
                bisect (b);
            }
        a = res;
    }

    template <>
    void mulmod (long long & a, long long b, const long long & n) {
        bool neg = false;
        if (a < 0)
        {
            neg = !neg;
            a = -a;
        }
        if (b < 0)
        {
            neg = !neg;
            b = -b;
        }
        unsigned long long aa = a;
        mulmod<unsigned long long> (aa, (unsigned long long)b, (unsigned long long)n);
        a = (long long)aa * (neg ? -1 : 1);
    }


    //! Вычисляет a^k (mod n). Использует бинарное возведение в степень
    template <class T, class T2>
    T powmod (T a, T2 k, const T & n) {
        T res = 1;
        while (k)
            if (!even (k))
            {
                mulmod (res, a, n);
                --k;
            }
            else
            {
                mulmod (a, a, n);
                bisect (k);
            }
        return res;
    }

    //! Переводит число n в форму q*2^p
    template <class T>
    void transform_num (T n, T & p, T & q) {
        T p_res = 0;
        while (even (n))
        {
            ++p_res;
            bisect (n);
        }
        p = p_res;
        q = n;
    }

    //! Алгоритм Евклида
    template <class T, class T2>
    T gcd (const T & a, const T2 & b) {
        return (a == 0) ? b : gcd (b % a, a);
    }

    //! Вычисляет jacobi(a,b)
    template <class T>
    T jacobi (T a, T b) {

    //#pragma warning (push)
    //#pragma warning (disable: 4146)

        if (a == 0)
            return 0;
        if (a == 1)
            return 1;

        if (a < 0)
            if ((b & 2) == 0)
                return jacobi (-a, b);
            else
                return - jacobi (-a, b);

        T e, a1;
        transform_num (a, e, a1);

        T s;
        if (even (e) || (b & 7) == 1 || (b & 7) == 7)
            s = 1;
        else
            s = -1;
        if ((b & 3) == 3 && (a1 & 3) == 3)
            s = -s;
        if (a1 == 1)
            return s;
        return s * jacobi (b % a1, a1);

    //#pragma warning (pop)

    }

    //! Вычисляет pi(b) первых простых чисел. Возвращает ссылку на вектор с простыми (в векторе может оказаться больше простых, чем надо) и в pi - pi(b)
    template <class T, class T2>
    const std::vector<T> & get_primes (const T & b, T2 & pi) {

        static std::vector<T> primes;
        static T counted_b;

        // если результат уже был вычислен ранее, возвращаем его, иначе довычисляем простые
        if (counted_b >= b)
            pi = T2 (std::upper_bound (primes.begin(), primes.end(), b) - primes.begin());
        else
        {

            // число 2 обрабатываем отдельно
            if (counted_b == 0)
            {
                primes.push_back (2);
                counted_b = 2;
            }

            // теперь обрабатываем все нечётные, пока не наберём нужное количество простых
            T first = counted_b == 2 ? 3 : primes.back()+2;
            for (T cur=first; cur<=b; ++++cur)
            {
                bool cur_is_prime = true;
                for (typename std::vector<T>::const_iterator iter = primes.begin(), end = primes.end(); iter != end; ++iter)
                {
                    const T & div = *iter;
                    if (div * div > cur)
                        break;
                    if (cur % div == 0)
                    {
                        cur_is_prime = false;
                        break;
                    }
                }
                if (cur_is_prime)
                    primes.push_back (cur);
            }

            counted_b = b;
            pi = (T2) primes.size();

        }

        return primes;

    }

    //! Тривиальная проверка n на простоту, перебираются все делители до m. Результат: 1 - если n точно простое, p - его найденный делитель, 0 - если неизвестно, является ли n простым или нет
    template <class T, class T2>
    T2 prime_div_trivial (const T & n, T2 m)
    {

        // сначала проверяем тривиальные случаи
        if (n == 2 || n == 3)
            return 1;
        if (n < 2)
            return 0;
        if (even (n))
            return 2;

        // генерируем простые от 3 до m
        T2 pi;
        const std::vector<T2> & primes = get_primes (m, pi);

        // делим на все простые
        for (typename std::vector<T2>::const_iterator iter=primes.begin(), end=primes.end();
            iter!=end && *iter <= m; ++iter) {
            const T2 & div = *iter;
            if (div * div > n)
                break;
            else
                if (n % div == 0)
                    return div;
        }

        if (n < m*m)
            return 1;
        return 0;

    }

    //! Усиленный алгоритм Миллера-Рабина проверки n на простоту по основанию b
    template <class T, class T2>
    bool miller_rabin (T n, T2 b) {

        // сначала проверяем тривиальные случаи
        if (n == 2)
            return true;
        if (n < 2 || even (n))
            return false;

        // проверяем, что n и b взаимно просты (иначе это приведет к ошибке)
        // если они не взаимно просты, то либо n не просто, либо нужно увеличить b
        if (b < 2)
            b = 2;
        for (T g; (g = gcd (n, b)) != 1; ++b)
            if (n > g)
                return false;

        // разлагаем n-1 = q*2^p
        T n_1 = n;
        --n_1;
        T p, q;
        transform_num (n_1, p, q);

        // вычисляем b^q mod n, если оно равно 1 или n-1, то n, вероятно, простое
        T rem = powmod (T(b), q, n);
        if (rem == 1 || rem == n_1)
            return true;

        // теперь вычисляем b^2q, b^4q, ... , b^((n-1)/2)
        // если какое-либо из них равно n-1, то n, вероятно, простое
        for (T i=1; i<p; i++)
        {
            mulmod (rem, rem, n);
            if (rem == n_1)
                return true;
        }

        return false;

    }

    //! Усиленный алгоритм Лукаса-Селфриджа проверки n на простоту. Используется усиленный алгоритм Лукаса с параметрами Селфриджа. Работает только с знаковыми типами!!! Второй параметр unused не используется, он только дает тип
    template <class T, class T2>
    bool lucas_selfridge (const T & n, T2 unused) {

        // сначала проверяем тривиальные случаи
        if (n == 2)
            return true;
        if (n < 2 || even (n))
            return false;

        // проверяем, что n не является точным квадратом, иначе алгоритм даст ошибку
        if (perfect_square (n))
            return false;

        // алгоритм Селфриджа: находим первое число d такое, что:
        // jacobi(d,n)=-1 и оно принадлежит ряду { 5,-7,9,-11,13,... }
        T2 dd;
        for (T2 d_abs = 5, d_sign = 1; ; d_sign = -d_sign, ++++d_abs)
        {
            dd = d_abs * d_sign;
            T g = gcd (n, d_abs);
            if (1 < g && g < n)
                // нашли делитель - d_abs
                return false;
            if (jacobi (T(dd), n) == -1)
                break;
        }

        // параметры Селфриджа
        T2
            p = 1,
            q = (p*p - dd) / 4;

        // разлагаем n+1 = d*2^s
        T n_1 = n;
        ++n_1;
        T s, d;
        transform_num (n_1, s, d);

        // алгоритм Лукаса
        T
            u = 1,
            v = p,
            u2m = 1,
            v2m = p,
            qm = q,
            qm2 = q*2,
            qkd = q;
        for (unsigned bit = 1, bits = bits_in_number(d); bit < bits; bit++) {
            mulmod (u2m, v2m, n);
            mulmod (v2m, v2m, n);
            while (v2m < qm2)
                v2m += n;
            v2m -= qm2;
            mulmod (qm, qm, n);
            qm2 = qm;
            redouble (qm2);
            if (test_bit (d, bit))
            {
                T t1, t2;
                t1 = u2m;
                mulmod (t1, v, n);
                t2 = v2m;
                mulmod (t2, u, n);

                T t3, t4;
                t3 = v2m;
                mulmod (t3, v, n);
                t4 = u2m;
                mulmod (t4, u, n);
                mulmod (t4, (T)dd, n);

                u = t1 + t2;
                if (!even (u))
                    u += n;
                bisect (u);
                u %= n;

                v = t3 + t4;
                if (!even (v))
                    v += n;
                bisect (v);
                v %= n;
                mulmod (qkd, qm, n);
            }
        }

        // точно простое (или псевдо-простое)
        if (u == 0 || v == 0)
            return true;

        // вычисляем оставшиеся члены
        T qkd2 = qkd;
        redouble (qkd2);
        for (T2 r = 1; r < s; ++r) {
            mulmod (v, v, n);
            v -= qkd2;
            if (v < 0) v += n;
            if (v < 0) v += n;
            if (v >= n) v -= n;
            if (v >= n) v -= n;
            if (v == 0)
                return true;
            if (r < s-1)
            {
                mulmod (qkd, qkd, n);
                qkd2 = qkd;
                redouble (qkd2);
            }
        }

        return false;

    }

    //! Алгоритм Бэйли-Померанс-Селфридж-Вагстафф (BPSW) проверки n на простоту
    template <class T>
    bool baillie_pomerance_selfridge_wagstaff (T n) {
        // перебираем тривиальные делители до 1000
        int div = prime_div_trivial (n, 1000);
        if (div == 1)
            return true;
        if (div > 1)
            return false;

        // тест Миллера-Рабина по основанию 2
        if (!miller_rabin (n, 2))
            return false;

        // усиленный тест Лукаса-Селфриджа
        return lucas_selfridge (n, 0);

    }

    //! Алгоритм Бэйли-Померанс-Селфридж-Вагстафф (BPSW) проверки n на простоту
    template <class T>
    bool isprime (T n) {
        return baillie_pomerance_selfridge_wagstaff (n);
    }

}

#endif // BPSW_HPP_INCLUDED
