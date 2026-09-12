#include <stdint.h> 
#include <math.h>



#ifndef NAN
#define NAN (0.0 / 0.0) // Define NAN if it's not already defined
#endif


int isinf(double x) {
    union {
        double d;
        uint64_t u;
    } u;
    u.d = x;
    uint64_t exponent = (u.u >> 52) & 0x7FF;
    uint64_t mantissa = u.u & 0x000FFFFFFFFFFFFFULL;
    return (exponent == 0x7FF && mantissa == 0);
}

// Trigonometric functions
double cos(double x) {
    // Using Taylor series approximation for cosine
    double result = 1.0;
    double term = 1.0;
    for (int i = 1; i <= 10; i++) {
        term *= -x * x / ((2 * i) * (2 * i - 1));
        result += term;
    }
    return result;
}

double sin(double x) {
    // Using Taylor series approximation for sine
    double result = x;
    double term = x;
    for (int i = 1; i <= 10; i++) {
        term *= -x * x / ((2 * i) * (2 * i + 1));
        result += term;
    }
    return result;
}

double tan(double x) {
    return sin(x) / cos(x);
}

// Decompose x into fractional and integer parts
double modf(double x, double* intpart) {
    *intpart = (long long)x;
    return x - *intpart;
}

// Logarithmic functions
double log(double x) {
    if (x <= 0) return NAN; // Logarithm of non-positive number is undefined
    // Using Newton's method for natural logarithm
    double result = 0.0;
    while (x >= 2.0) {
        x /= 2.0;
        result += 0.69314718056; // ln(2)
    }
    while (x < 1.0) {
        x *= 2.0;
        result -= 0.69314718056; // ln(2)
    }
    x -= 1.0;
    double term = x;
    result += term;
    for (int i = 2; i <= 10; i++) {
        term *= -x;
        result += term / i;
    }
    return result;
}

double log10(double x) {
    return log(x) / 2.30258509299; // ln(10)
}

// Power function
double pow(double base, double exponent) {
    if (exponent == 0) return 1.0;
    if (base == 0) return 0.0;
    double result = 1.0;
    for (int i = 0; i < (int)exponent; i++) {
        result *= base;
    }
    return result;
}

// Square root function
double sqrt(double x) {
    if (x < 0) return NAN; // Square root of negative number is undefined
    double guess = x / 2.0;
    for (int i = 0; i < 10; i++) {
        guess = 0.5 * (guess + x / guess);
    }
    return guess;
}

// Rounding functions
double ceil(double x) {
    if (x == (long long)x) return x;
    return x > 0 ? (long long)x + 1 : (long long)x;
}

double floor(double x) {
    if (x == (long long)x) return x;
    return x > 0 ? (long long)x : (long long)x - 1;
}

double fmod(double numer, double denom) {
    if (denom == 0) return NAN; // Division by zero
    return numer - (long long)(numer / denom) * denom;
}

double trunc(double x) {
    return (long long)x;
}

float truncf(float x) {
    return (long long)x;
}

long double truncl(long double x) {
    return (long long)x;
}

double round(double x) {
    return x >= 0 ? (long long)(x + 0.5) : (long long)(x - 0.5);
}

float roundf(float x) {
    return x >= 0 ? (long long)(x + 0.5f) : (long long)(x - 0.5f);
}

long double roundl(long double x) {
    return x >= 0 ? (long long)(x + 0.5L) : (long long)(x - 0.5L);
}

// Minimum, Maximum, and difference functions
int min(int a, int b) {
    return a < b ? a : b;
}
double fmin(double x, double y) {
    return x < y ? x : y;
}

float fminf(float x, float y) {
    return x < y ? x : y;
}

long double fminl(long double x, long double y) {
    return x < y ? x : y;
}

int max(int a, int b) {
    return a > b ? a : b;
}

double fmax(double x, double y) {
    return x > y ? x : y;
}

float fmaxf(float x, float y) {
    return x > y ? x : y;
}

long double fmaxl(long double x, long double y) {
    return x > y ? x : y;
}

// Absolute value functions
double fabs(double x) {
    return x < 0 ? -x : x;
}

float fabsf(float x) {
    return x < 0 ? -x : x;
}

int abs(int x) {
    return x < 0 ? -x : x;
}

float absf(float x) {
    return x < 0 ? -x : x;
}

long double absld(long double x) {
    return x < 0 ? -x : x;
}


double acos(double x) {
    if (x < -1.0 || x > 1.0) return NAN;
    return atan2(sqrt(1.0 - x*x), x);
}

double atan2(double y, double x) {
    if (x > 0) return atan(y / x);
    if (x < 0) {
        if (y >= 0) return atan(y / x) + M_PI;
        return atan(y / x) - M_PI;
    }
    if (y > 0) return M_PI_2;
    if (y < 0) return -M_PI_2;
    return NAN;
}

double atan(double x) {
    if (x > 1.0) return M_PI_2 - atan(1.0 / x);
    if (x < -1.0) return -M_PI_2 - atan(1.0 / x);
    
    double result = x;
    double term = x;
    double x_sq = x * x;
    for (int i = 1; i < 20; i++) {
        term *= -x_sq;
        result += term / (2 * i + 1);
    }
    return result;
}

float cosf(float x) {
    float result = 1.0f;
    float term = 1.0f;
    for (int i = 1; i <= 10; i++) {
        term *= -x * x / ((2 * i) * (2 * i - 1));
        result += term;
    }
    return result;
}

float sinf(float x) {
    float result = x;
    float term = x;
    for (int i = 1; i <= 10; i++) {
        term *= -x * x / ((2 * i) * (2 * i + 1));
        result += term;
    }
    return result;
}

float tanf(float x) {
    return sinf(x) / cosf(x);
}

float acosf(float x) {
    if (x < -1.0f || x > 1.0f) return NAN;
    return atan2f(sqrtf(1.0f - x*x), x);
}

float atan2f(float y, float x) {
    if (x > 0) return atanf(y / x);
    if (x < 0) {
        if (y >= 0) return atanf(y / x) + (float)M_PI;
        return atanf(y / x) - (float)M_PI;
    }
    if (y > 0) return (float)M_PI_2;
    if (y < 0) return -(float)M_PI_2;
    return NAN;
}

float atanf(float x) {
    if (x > 1.0f) return (float)M_PI_2 - atanf(1.0f / x);
    if (x < -1.0f) return -(float)M_PI_2 - atanf(1.0f / x);
    
    float result = x;
    float term = x;
    float x_sq = x * x;
    for (int i = 1; i < 20; i++) {
        term *= -x_sq;
        result += term / (2 * i + 1);
    }
    return result;
}

float sqrtf(float x) {
    if (x < 0) return NAN;
    float guess = x / 2.0f;
    for (int i = 0; i < 10; i++) {
        guess = 0.5f * (guess + x / guess);
    }
    return guess;
}


// Hyperbolic Functions
float sinhf(float x) {
    float ex = expf(x);
    float e_x = expf(-x);
    return (ex - e_x) / 2.0f;
}

float coshf(float x) {
    float ex = expf(x);
    float e_x = expf(-x);
    return (ex + e_x) / 2.0f;
}

float tanhf(float x) {
    float ex = expf(x);
    float e_x = expf(-x);
    return (ex - e_x) / (ex + e_x);
}

// Inverse Hyperbolic Functions
float asinhf(float x) {
    return logf(x + sqrtf(x*x + 1.0f));
}

float acoshf(float x) {
    if (x < 1.0f) return NAN;
    return logf(x + sqrtf(x*x - 1.0f));
}

float atanhf(float x) {
    if (x <= -1.0f || x >= 1.0f) return NAN;
    return 0.5f * logf((1.0f + x)/(1.0f - x));
}

// Power and Exponential Functions
float powf(float base, float exponent) {
    if (base == 0.0f) {
        if (exponent <= 0.0f) return NAN;
        return 0.0f;
    }
    if (exponent == 0.0f) return 1.0f;
    return expf(exponent * logf(base));
}

float expf(float x) {
    if (x == 0.0f) return 1.0f;

    int is_negative = 0;
    if (x < 0.0f) {
        is_negative = 1;
        x = -x;
    }

    const float ln2 = 0.69314718056f;
    int k = (int)(x / ln2);
    float r = x - (float)k * ln2;

    float term = 1.0f;
    float sum = term;
    for (int i = 1; i <= 10; i++) {
        term *= r / (float)i;
        sum += term;
    }

    float two_k = 1.0f;
    if (k > 0) {
        for (int i = 0; i < k; i++) two_k *= 2.0f;
    } else if (k < 0) {
        for (int i = 0; i > k; i--) two_k /= 2.0f;
    }

    sum *= two_k;
    return is_negative ? 1.0f / sum : sum;
}

float exp2f(float x) {
    return expf(x * 0.69314718056f);
}

// Logarithmic Functions
float logf(float x) {
    if (x <= 0.0f) return NAN;

    int exponent = 0;
    while (x >= 2.0f) { x /= 2.0f; exponent++; }
    while (x < 1.0f) { x *= 2.0f; exponent--; }

    x -= 1.0f;

    float term = x, result = term;
    for (int i = 2; i <= 10; i++) {
        term *= -x;
        result += term / (float)i;
    }

    return result + exponent * 0.69314718056f;
}

float log2f(float x) {
    return logf(x) / 0.69314718056f;
}

// Rounding Functions
float floorf(float x) {
    if (x >= 0.0f) return (int)x;
    float int_part = (int)x;
    return (x == int_part) ? int_part : int_part - 1.0f;
}

float nearbyintf(float x) {
    return (x >= 0.0f) ? (int)(x + 0.5f) : (int)(x - 0.5f);
}

float ceilf(float x) {
    if (x == (int)x) return x;
    return (x > 0.0f) ? (int)x + 1.0f : (int)x;
}

// NaN Check
int isnan(float x) {
    return x != x;
}

// Implementation of ldexp and ldexpf
double ldexp(double x, int exp) {
    if (x == 0.0 || isinf(x) || isnan(x)) {
        return x;
    }

    union {
        double d;
        uint64_t u;
    } u;
    u.d = x;

    uint64_t sign = u.u & 0x8000000000000000ULL;
    int64_t exponent = (u.u >> 52) & 0x7FF;
    uint64_t mantissa = u.u & 0x000FFFFFFFFFFFFFULL;

    if (exponent == 0) {
        // Handle subnormal by multiplying with pow
        return x * pow(2.0, exp);
    }

    int64_t new_biased_exponent = exponent + exp;

    if (new_biased_exponent >= 0x7FF) {
        // Overflow to infinity
        u.u = sign | 0x7FF0000000000000ULL;
        return u.d;
    } else if (new_biased_exponent <= 0) {
        // Underflow to zero
        u.u = sign;
        return u.d;
    } else {
        u.u = sign | ((uint64_t)new_biased_exponent << 52) | mantissa;
        return u.d;
    }
}

float ldexpf(float x, int exp) {
    if (x == 0.0f) return x;

    union {
        float f;
        uint32_t u;
    } u;
    u.f = x;

    uint32_t sign = u.u & 0x80000000UL;
    uint32_t exponent = (u.u >> 23) & 0xFF;
    uint32_t mantissa = u.u & 0x007FFFFFUL;

    // Check for NaN or Inf
    if (exponent == 0xFF) {
        return x;
    }

    if (exponent == 0) {
        // Handle subnormal by multiplying with powf
        return x * powf(2.0f, exp);
    }

    int32_t new_biased_exponent = exponent + exp;

    if (new_biased_exponent >= 0xFF) {
        // Overflow to infinity
        u.u = sign | 0x7F800000UL;
        return u.f;
    } else if (new_biased_exponent <= 0) {
        // Underflow to zero
        u.u = sign;
        return u.f;
    } else {
        u.u = sign | ((uint32_t)new_biased_exponent << 23) | mantissa;
        return u.f;
    }
}

float fmodf(float x, float y) {
    if (y == 0.0f) {
        // Division by zero is undefined, return NaN
        return NAN;
    }

    // Handle infinity cases
    if (isinf(x) || isnan(x) || isnan(y)) {
        return NAN;
    }

    // Handle case where y is infinity
    if (isinf(y)) {
        return x;
    }

    // Compute the absolute values of x and y
    float abs_x = fabsf(x);
    float abs_y = fabsf(y);

    // If x is less than y, the remainder is x
    if (abs_x < abs_y) {
        return x;
    }

    // Repeatedly subtract y from x until x < y
    while (abs_x >= abs_y) {
        abs_x -= abs_y;
    }

    // Restore the sign of the result
    if (x < 0.0f) {
        abs_x = -abs_x;
    }

    return abs_x;
}