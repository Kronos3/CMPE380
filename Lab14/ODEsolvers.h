/*  ===========================================================================
      ODEsolvers.h - Professor header file for ODEsolvers module 
      Version: 1.02
      Juan C. Cockburn, Nov. 2012
      R. Repka      11/11/2015 - Improved prototypes
      R. Repka      12/13/2015 - Added disturbance time an value, restructured
      R. Repka      11/20/2017 - Added simple tank simulation
      R. Repka      06/23/2020 - Added spring and pendulum simulations
    =========================================================================== */
#ifndef _ODESOLVERS_H_
#define _ODESOLVERS_H_

#include <stdlib.h>
#include <math.h>

#define  NX (2)     /* Number of simulator state variables */

typedef struct {
    int neq;      /* Number of simulation equations               */
    double h;     /* time step                                    */
    double damp;  /* dampening value, if used                     */
} simParm;

/*  ===========================================================================
    These are function pointer prototypes.  The function pointed to will contain:
    
    A function which defines the right-hand side of a  system of differential equations
    in state-space form.  This function must be of the form
    
 =========================================================================== */
typedef void (*FunArgs) (const simParm *sim, double t, double *x, double *dx);
typedef void (*Solver) (const simParm *sim,  double t0, double *x0, FunArgs f);

/* Other function prototypes */
void eu(const simParm *sim,  double t0, double *x0, FunArgs f);
void rk2(const simParm *sim, double t0, double *x0, FunArgs f);
void rk3(const simParm *sim, double t0, double *x0, FunArgs f);
void rk4(const simParm *sim, double t0, double *x0, FunArgs f);
void tank(const simParm *sim, double t, double *x, double *dx);
void springMass(const simParm *sim, double t, double *x, double *dx);

#endif /* _ODESOLVERS_H_ */
