#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <stdlib.h>
#include "include/str_to_float.h"

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

str2float_errno str2float(float *out, char *s) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2FLOAT_INCONVERTIBLE;
    errno = 0;
    double d = strtod(s, &end);
    /* Both checks are needed because FLT_MAX == DBL_MAX is possible. */
    if (d > FLT_MAX || (errno == ERANGE && d == DBL_MAX))
        return STR2FLOAT_OVERFLOW;
    if (d < FLT_MIN || (errno == ERANGE && d == DBL_MIN))
        return STR2FLOAT_UNDERFLOW;
    if (*end != '\0')
        return STR2FLOAT_INCONVERTIBLE;
    *out = (float)d;
    return STR2FLOAT_SUCCESS;
}