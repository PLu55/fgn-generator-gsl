/*  Copyright Ton Dieker                                                    */
/*  Centre of Mathematics and Computer Science (CWI) Amsterdam              */
/*  April 2002                                                              */

/*  ton@cwi.nl                                                              */

/*  Modified to be used with GSL (instead of Netlib libraries) by           */
/*  Aleksejus Kononovicius                                                  */

/*  Modified  by                                                            */
/*  Peter Lundén p@apal.se                                                  */

typedef struct fgn_plan_S fgn_plan;

extern void fgn(fgn_plan* plan, double L, double *output);
extern void fbm(fgn_plan* plan, double L, double *output);
extern void fgn_make_plan(int n, long seed, double H, fgn_plan** plan);
extern void fgn_free(fgn_plan* plan);
