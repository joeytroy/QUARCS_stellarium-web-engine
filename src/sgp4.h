/* Stellarium Web Engine - Copyright (c) 2022 - Stellarium Labs SRL
 *
 * This program is licensed under the terms of the GNU AGPL v3, or
 * alternatively under a commercial licence.
 *
 * The terms of the AGPL v3 license can be found in the main directory of this
 * repository.
 */

/*
 * C bindings to the SGP4 functions for artificial satellites position
 * computation so that we can use them from C or other environments.
 */

#ifndef SGP4_H
#define SGP4_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/* SGP4 satellite record structure. */
typedef struct sgp4_elsetrec sgp4_elsetrec_t;

/*
 * Function: sgp4_twoline2rv
 * Convert Two-Line Element (TLE) data to an sgp4_elsetrec_t structure.
 *
 * Parameters:
 *  str1        - First line of the TLE (char array of 130 characters).
 *  str2        - Second line of the TLE (char array of 130 characters).
 *  typerun     - Type of run ('v', 'c', or 'm').
 *  typeinput   - Type of input ('m', 'e', or 'd').
 *  opsmode     - Operation mode ('a' for AFSPC, 'i' for improved).
 *  startmfe    - Pointer to start minute from epoch.
 *  stopmfe     - Pointer to stop minute from epoch.
 *  deltamin    - Pointer to time step in minutes.
 *
 * Returns:
 *  Pointer to a dynamically allocated sgp4_elsetrec_t structure.
 */
sgp4_elsetrec_t *sgp4_twoline2rv(
    const char str1[130], const char str2[130],
    char typerun, char typeinput, char opsmode,
    double *startmfe, double *stopmfe, double *deltamin
);

/*
 * Function: sgp4
 * Propagate a satellite's position and velocity at a given time.
 *
 * Parameters:
 *  satrec      - Pointer to an sgp4_elsetrec_t structure.
 *  utc_mjd     - UTC time in Modified Julian Date format.
 *  r           - Output array for the satellite's position (3 elements, km).
 *  v           - Output array for the satellite's velocity (3 elements, km/s).
 *
 * Returns:
 *  Error code:
 *    0 - No error.
 *    1 - Mean elements, ecc >= 1.0 or ecc < -0.001 or a < 0.95 ER.
 *    2 - Mean motion less than 0.0.
 *    3 - Perturbed elements, ecc < 0.0 or ecc > 1.0.
 *    4 - Semi-latus rectum < 0.0.
 *    5 - Epoch elements are sub-orbital.
 *    6 - Satellite has decayed.
 */
int sgp4(sgp4_elsetrec_t *satrec, double utc_mjd, double r[3], double v[3]);

/*
 * Function: sgp4_get_satepoch
 * Retrieve the reference epoch of a satellite in UTC Modified Julian Date (MJD).
 *
 * Parameters:
 *  satrec      - Pointer to an sgp4_elsetrec_t structure.
 *
 * Returns:
 *  Reference epoch in UTC MJD.
 */
double sgp4_get_satepoch(const sgp4_elsetrec_t *satrec);

/*
 * Function: sgp4_get_perigree_height
 * Compute the perigree height in kilometers for a given satellite orbit.
 *
 * Parameters:
 *  satrec      - Pointer to an sgp4_elsetrec_t structure.
 *
 * Returns:
 *  Perigree height in kilometers.
 */
double sgp4_get_perigree_height(const sgp4_elsetrec_t *satrec);

#ifdef __cplusplus
}
#endif

#endif /* SGP4_H */
