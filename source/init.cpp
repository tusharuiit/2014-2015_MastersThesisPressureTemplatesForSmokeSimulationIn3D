#include "helper.h"
#include "init.h"

int read_parameters( const char *szFileName,       /* name of the file */
                    Real *Re,                /* reynolds number   */
                    Real *UI,                /* velocity x-direction */
                    Real *VI,                /* velocity y-direction */
                    Real *PI,                /* pressure */
                    Real *GX,                /* gravitation x-direction */
                    Real *GY,                /* gravitation y-direction */
                    Real *t_end,             /* end time */
                    Real *xlength,           /* length of the domain x-dir.*/
                    Real *ylength,           /* length of the domain y-dir.*/
                    Real *dt,                /* time step */
                    Real *dx,                /* length of a cell x-dir. */
                    Real *dy,                /* length of a cell y-dir. */
                    int  *imax,                /* number of cells x-direction*/
                    int  *jmax,                /* number of cells y-direction*/
                    Real *alpha,             /* uppwind differencing factor*/
                    Real *omg,               /* relaxation factor */
                    Real *tau,               /* safety factor for time step*/
                    int  *itermax,             /* max. number of iterations  */
		                               /* for pressure per time step */
                    Real *eps,               /* accuracy bound for pressure*/
		    Real *dt_value)           /* time for output */
{
   READ_Real( szFileName, *xlength );
   READ_Real( szFileName, *ylength );

   READ_Real( szFileName, *Re    );
   READ_Real( szFileName, *t_end );
   READ_Real( szFileName, *dt    );

   READ_INT   ( szFileName, *imax );
   READ_INT   ( szFileName, *jmax );

   READ_Real( szFileName, *omg   );
   READ_Real( szFileName, *eps   );
   READ_Real( szFileName, *tau   );
   READ_Real( szFileName, *alpha );

   READ_INT   ( szFileName, *itermax );
   READ_Real( szFileName, *dt_value );

   READ_Real( szFileName, *UI );
   READ_Real( szFileName, *VI );
   READ_Real( szFileName, *GX );
   READ_Real( szFileName, *GY );
   READ_Real( szFileName, *PI );

   *dx = *xlength / (Real)(*imax);
   *dy = *ylength / (Real)(*jmax);

   return 1;
}

/**
 * The arrays U,V and P are initialized to the constant values UI, VI and PI on
 * the whole domain.
 */
void init_uvp(
  Real UI,
  Real VI,
  Real PI,
  int imax,
  int jmax,
  Real **U,
  Real **V,
  Real **P
)
{
    /** asign the initial values to whole domain */
    init_matrix(U, 0, imax, 0, jmax + 1, UI);
    init_matrix(P, 0, imax + 1, 0, jmax + 1, PI);
    init_matrix(V, 0, imax + 1, 0, jmax, VI);

}


