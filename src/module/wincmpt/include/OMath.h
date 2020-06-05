#ifndef __OMATH_H__
#define __OMATH_H__

#define omath_PI		(3.14159265358979)
#define omath_2PI		(6.28318530717958)
#define omath_hPI		(1.57079632679489)

#define omath_abs(x)	(((x)<0)?-((x)):(x))
#define omath_max(x,y)	(((x)>(y))?(x):(y))
#define omath_min(x,y)	(((x)<(y))?(x):(y))

#ifdef __cplusplus
extern "C" {
#endif

double omath_sqrt(double x);
double omath_pow(double base, double ex);

double omath_sin(double x);
double omath_cos(double x);
double omath_tan(double x);

double omath_asin(double x);
double omath_acos(double x);
double omath_atan(double x);
double omath_atan2(double y, double x);

int omath_gcd(int a, int b);
int omath_lcm(int a, int b);

#ifdef __cplusplus
}
#endif

#endif
