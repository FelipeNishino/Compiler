#ifndef STR2FLOAT_H
#define STR2FLOAT_H

/* Convert string s to float out.
 *
 * @param[out] out The converted float. Cannot be NULL.
 *
 * @param[in] s Input string to be converted.
 *
 *     The format is the same as strtod,
 *     except that the following are inconvertible:
 *
 *     - empty string
 *     - leading whitespace
 *     - any trailing characters that are not part of the number
 *
 *     Cannot be NULL.
 *
 * @return Indicates if the operation succeeded, or why it failed.
 */

typedef enum {
    STR2FLOAT_SUCCESS,
    STR2FLOAT_OVERFLOW,
    STR2FLOAT_UNDERFLOW,
    STR2FLOAT_INCONVERTIBLE
} str2float_errno;

str2float_errno str2float(float *out, char *s);

#endif //STR2FLOAT_H