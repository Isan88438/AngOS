#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MATH_H__
#define __MATH_H__

#include <stdint.h>

extern int isinf(double x);

extern double cos(double x);
extern double sin(double x);
extern double tan(double x);
extern double acos(double x);
extern double asin(double x);
extern double atan(double x);
extern double atan2(double y, double x);

extern float cosf(float x);
extern float sinf(float x);
extern float tanf(float x);
extern float acosf(float x);
extern float asinf(float x);
extern float atanf(float x);
extern float atan2f(float y, float x);

extern double modf(double x, double* intpart);
extern double log(double x);
extern double log10(double x);

extern double pow(double base, double exponent);
extern double sqrt(double x);
extern float sqrtf(float x);

extern double ceil(double x);
extern double floor(double x);
extern double fmod(double numer, double denom);

extern double trunc(double x);
extern float truncf(float x);
extern long double truncl(long double x);

extern double round(double x);
extern float roundf(float x);
extern long double roundl(long double x);

extern int min(int a, int b);
extern double fmin(double x, double y);
extern float fminf(float x, float y);
extern long double fminl(long double x, long double y);

extern int max(int a, int b);
extern double fmax(double x, double y);
extern float fmaxf(float x, float y);
extern long double fmaxl(long double x, long double y);

extern double fabs(double x);
extern float fabsf(float x);

extern float absf(float x);
extern long double absld(long double x);

extern float sinhf(float x);
extern float coshf(float x);
extern float tanhf(float x);
extern float asinhf(float x);
extern float acoshf(float x);
extern float atanhf(float x);
extern float powf(float base, float exponent);
extern float expf(float x);
extern float exp2f(float x);
extern float logf(float x);
extern float log2f(float x);
extern float floorf(float x);
extern float nearbyintf(float x);
extern float ceilf(float x);
extern int isnan(float x);

extern double ldexp(double x, int exp);
extern float ldexpf(float x, int exp);

extern float fmodf(float x, float y);

#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define NAN         (0.0 / 0.0)

#endif // __MATH_H__

#ifdef __cplusplus
}
#endif
