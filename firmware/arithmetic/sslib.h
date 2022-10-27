/*		sslib.h		include file for sslib		*/
#ifndef _SSLIB
#define _SSLIB

#include	<math.h>

typedef		struct
			{
				double r;
				double i;
			}				Complex;

#ifndef	MAX_LONG
#define		MAX_LONG		 2147483647L
#endif
#ifndef	MIN_LONG
#define		MIN_LONG		-2147483648L
#endif

/*	complex.c	*/
Complex cadd(Complex a, Complex b);
Complex csub(Complex a, Complex b);
Complex cmul(Complex a, Complex b);
Complex cmul1(Complex a, double k);
Complex cdiv(Complex a, Complex b);
Complex cdiv1(Complex a, double k);
Complex cln(Complex a);
Complex clog10(Complex a);
Complex cexp(Complex a);
Complex cpow(Complex a, Complex b);
Complex cpowd(Complex a, double n);
Complex csin(Complex a);
Complex ccos(Complex a);
Complex ctan(Complex a);
Complex csqrt(Complex a);
Complex tocomplex(double x, double y);
Complex conj(Complex a);
Complex cfunc(Complex *a, int n, Complex x);
double cabslt(Complex a);
double carg(Complex a);
int ceq(Complex a, Complex b);
int cne(Complex a, Complex b);

/*	function.c	*/
double cbrt(double a);
double besj0(double x);
double besj1(double x);
double besy0(double x);
double besy1(double x);
double besi0(double x);
double besi1(double x);
double besk0(double x);
double besk1(double x);
double erfnc(double x);
//double gammaf(double x);
double legend(double x, int n);
double celi1(double k, double eps);
double celi2(double k, double eps);

/*	difint.c	*/
double cheb3(double a, double b);
double cheb4(double a, double b);
double cheb6(double a, double b);
double dgl3(double a, double b);
double dgl10(double a, double b);
double dgl20(double a, double b);
double dgl32(double a, double b);
double dgl48(double a, double b);
double dglg3(void);
double dglg5(void);
double dglg10(void);
double dgh10(void);
double dgh15(void);
double hardy(double xmin, double xmax, int n);
double lomberg(double xmin, double xmax, double eps);
double nc1(double xmin, double xmax, int n);
double nc2(double xmin, double xmax, int n);
double nc3(double xmin, double xmax, int n);
double nc4(double xmin, double xmax, int n);
double nc5(double xmin, double xmax, int n);
double nc6(double xmin, double xmax, int n);
double nc7(double xmin, double xmax, int n);
double nc8(double xmin, double xmax, int n);
double weddle(double xmin, double xmax, int n);
double _Normal(double a, double b, double x);
double difm1(double x, double h);
double difm2(double x, double h);
double difm3(double x, double h);
double difm4(double x, double h);
double difm5(double x, double h);
double difm6(double x, double h);
double difm7(double x, double h);
double difm8(double x, double h);

/*	bibun.c	*/
double diff2(double x, double h);
double diff3(double x, double h);
double diff4(double x, double h);
double diff5(double x, double h);
double difb2(double x, double h);
double difb3(double x, double h);
double difb4(double x, double h);
double difb5(double x, double h);

/*	simp.c	*/
double lagdif(double xd[], double yd[], int n, double xx);
double spldif(double xd[], double yd[], int n, double xx);
void subspl(double x[], double y[], int n, double h[], double sp[]);
double trap(double xx[], double yy[], int n);
double simpei(double y[], int n, double h);
double simpui(double xx[], double yy[], int n);
double splitg(double xx[], double yy[], int n);
double simpe2(double y[], int m, int n, double h1, double h2);

/*	poly.c	*/
void qurt(Complex a, Complex b, Complex c, Complex x[]);
void carda(double a[], Complex x[]);
double newton(double a[], int n, double eps, int iter);
void bairs(double a[], int n, double eps, int iter, Complex x[]);
int cnewton(Complex a[], Complex r[], int n, double eps, int iter);
int dka(Complex a[], Complex r[], int n, double eps, int iter);

/*	regula.c	*/
double regula(double xs, double xe, double h, double eps, int iter);

/*	linear.c	*/
void gausei(double a[], int l, int m, int iter, double eps, double x[]);
void gauss(double a[], int l, int m, int n, double eps);
void gaujor(double a[], int l, int m, int n, double eps);
int ludcmp(double aa[], double b[], double x[], double eps, int n);
int trdiam(double al[], double am[], double au[], double b[], int n, double x[]);
void cgauj(Complex a[], int l, int m, int n, double eps);

/*	matrix.c	*/
void madd(double a[], double b[], double c[], int la, int lb, int lc, int m, int n);
void msub(double a[], double b[], double c[], int la, int lb, int lc, int m, int n);
void mmul1(double a[], double b[], int la, int lb, int m);
void mmul2(double a[], double b[], double c[], int la, int lb, int lc, int m, int n, int k);
void mtra1(double a[], int l, int m, int n);
void mtra2(double a[], double b[], int la, int lb, int m, int n);
double minver(double a[], int l, int m, double eps);
void mmove(double a[], double b[], int la, int lb, int m, int n);
void mswap(double a[], double b[], int la, int lb, int m, int n);
int jacobi(double a[], double v[], int l, int m, int *nr, double eps);

/*	fft.c	*/
void fft1(double ar[], double ai[], int n, int iter, int flag);
void fft1x(Complex a[], int n, int iter, int flag);
void fft2(double ar[], double ai[], int n, int nmax, int flag);
void fft2x(Complex a[], int n, int nmax, int flag);

/*	interp.c	*/
void lstsq(double x[], double y[], int n, int m, double c[]);
double lagra(double xx[], double yy[], int n, double xi);
double splint(double xx[], double yy[], int n, double xi);
double polynomial(double a[], double x, int n);
void chebyshev(double min, double max, int n, double a[]);

/*	basstat.c	*/
void sorti1(int a[], int n);
void sorti2(int a[], int n);
void sortii1(int a[], int n, int jun[]);
void sortii2(int a[], int n, int jun[]);
void sortd1(double a[], int n);
void sortd2(double a[], int n);
void sortdi1(double a[], int n, int jun[]);
void sortdi2(double a[], int n, int jun[]);
int stat(double a[], int n, double *min, double *max, double *amp, double *sum, double *sum2, double *ave, double *gmean, double *hmean, double *amad, double *s, double *v, double *st, double *uv, double *squv, double *cv, double *sk, double *ku);
void swapi(int *a, int *b);
void swapd(double *a, double *b);
void swapc(char *a, char *b);
void swapx(Complex *a, Complex *b);

/*	dist.c	*/
double qnorm(double u);
double pnorm(double qn);
double qchi(double x2, int n);
double pchi(double qc, int n);
double qt(double t, int n);
void qtsub(double *q, int n, double w2, double w3, double t2);
double pt(double q, int n);
double ptsub(double q, int n);
double qf(double f, int n1, int n2);
double pf(double q, int n1, int n2);
double pfsub(double x, double y, double z);

/*	axis	*/
int dtor(double deg, double *rad);
int rtod(double rad, double *deg);
int dtop2(double xo, double yo, double *r, double *t);
int ptod2(double r, double t, double *x, double *y);
int dmov2(double xo, double yo, double xm, double ym, double *x, double *y);
int pmov2(double ro, double to, double xm, double ym, double *r, double *t);
int drot2(double xo, double yo, double t, double *x, double *y);
int prot2(double ro, double to, double trot, double *r, double *t);
int dtoc3(double dx, double dy, double dz, double *cr, double *ct, double *cz);
int ctod3(double cr, double ct, double cz, double *dx, double *dy, double *dz);
int dtop3(double dx, double dy, double dz, double *pr, double *pe, double *pp);
int ptod3(double pr, double pe, double pp, double *dx, double *dy, double *dz);
int ptoc3(double pr, double pe, double pp, double *cr, double *ct, double *cz);
int ctop3(double cr, double ct, double cz, double *pr, double *pe, double *pp);
int dpmov3(double xo, double yo, double zo, double dx, double dy, double dz,
	double *x, double *y, double *z);
int cpmov3(double cro, double cto, double czo, double dx, double dy, double dz,
	double *cr, double *ct, double *cz);
int ppmov3(double pro, double peo, double ppo, double dx, double dy, double dz,
	double *pr, double *pe, double *pp);
int drot3(double dxo, double dyo, double dzo, double phai, double thet,
	double psai, double *dx, double *dy, double *dz);
int crot3(double cro, double cto, double czo, double phai, double thet,
	double psai, double *cr, double *ct, double *cz);
int prot3(double pro, double peo, double ppo, double phai, double thet,
	double psai, double *pr, double *pe, double *pp);

/*	corr.c	*/
int corr(double x[], double y[], int n, double xbar, double ybar,
	double *uxy, double *r0);
int sreg(double x[], double y[], int n, double sx, double g,
	double *a, double *b, double *dyx, double *va, double *vb, double *al,
	double *au, double *bl, double *bu);

/*	vari.c	*/
int aov1(double x[], int n[], int l, int m, double al, double *sa, double *se,
	double *st, int *nua, int *nue, int *nut, double *va, double *ve,
	double *f0, double *f);
int aov2(double x[], int l, int m, int n, double al, double *sa, double *sb,
	double *se, double *st, int *nua, int *nub, int *nue, int *nut, double *va,
	double *vb, double *ve, double *f0a, double *f0b, double *fa, double *fb);

/*	test.c	*/
int mtst1(int n, double xbar, double pm, double pv, double al,
	int *sw, double *u0, double *u);
int mtst2(int n, double xbar, double pm, double uv, double al,
	int *sw, double *t0, double *t);
int mdtst1(int n1, int n2, double xbar1, double xbar2, double pv1, double pv2,
	double al, int *sw, double *u0, double *u);
int mdtst2(int n1, int n2, double xbar1, double xbar2, double s1, double s2,
	double al, int *sw, double *t0, double *t);
int mdtst3(int n1, int n2, double xbar1, double xbar2, double uv1, double uv2,
	double al, int *sw, double *t0, double *t);
int nbptst(int n, double ps, double pp, double al, int *sw, double *u0, double *u);
int fbptst(int n, double ps, double pp, double al, int *sw, double *f0, double *f);
int bpdtst(int n1, int n2, double ps1, double ps2, double al,
	int *sw, double *u0, double *u);
int vtst(int sw1, int n, double s, double v, double al,
	int *sw2, double *x20, double *x21, double *x22);
int vdtst(int n1, int n2, double uv1, double uv2, double al,
	int *sw, double *f0, double *f);
int cont22(int *sw, double a, double b, double c, double d, double al,
	double *x20, double *x2);
int contlm(int l, int m, double a[], double al, double ac[],
	double ar[], double *at, double ef[], double *x20, double *x2, int *sw);
int thomp(int n, double xk, double xbar, double v, double al,
	int *sw, double *t0, double *t);
int rttst(int n, double r0, double al, int *sw, double *t0, double *t);
int rptst(int n, double r0, double rp, double al, int *sw, double *u0, double *u);
int rptst2(int n1, int n2, double r01, double r02, double al,
	int *sw, double *u0, double *u);

/*	esti.c	*/
int mest1(int n, double xbar, double pv, double g, double *xl, double *xu);
int mest2(int n, double xbar, double squv, double g, double *xl, double *xu);
int mdest1(int n1, int n2, double xbar1, double xbar2, double pv1, double pv2,
	double g, double *xl, double *xu);
int mdest2(int n1, int n2, double xbar1, double xbar2, double v1, double v2,
	double g, double *xl, double *xu);
int vest(int sw, int n, double s, double g, double *xl, double *xu);
int vpest(int n1, int n2, double uv1, double uv2, double g, double *xl, double *xu);
int bpest(int n, double ps, double g, double *xl, double *xu);
int bpdest(int n1, int n2, double ps1, double ps2, double g, double *xl, double *xu);
int rest(int n, double r0, double g, double *xl, double *xu);

/*	rkg.c	*/
int rngkg(double x0, double y0, int n, double h, double y[]);
int hamng(double x0, double y0, int n, double h, double y[]);
int rngkgm(double x, double y[], double h, int multi, int n);

/*	etc.c	*/
double normal(double av, double st);
double rnd(void);
int poison(double av);
double bino(int m, int n);
void filt(double ts, double te, double *f, int nt, int n, int m, double *a1, double *b1, double a, double k1, double k2, int p);
double kaijo1(int n);
double kaijo2(int n);
double log_kai1(int n);
double log_kai2(int n);
void minmax_i(int a[], int n, int *min, int *max);
void minmax_d(double a[], int n, double *min, double *max);
void minmax_c(unsigned char *a[], int n, unsigned char **min, unsigned char **max);

extern double _f(double x);
extern double _fxy(double x, double y);
extern void _fmxy(double x, double y[], double dif[], int multi);


#endif

