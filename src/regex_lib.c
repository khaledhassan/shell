#include "regex_lib.h"


// REGEX code used was retreived from <http://www.lemoda.net/c/unix-regex/> and slightly modified


/* Compile the regular expression described by "regex_text" into
   "r". */

int compile_regex (regex_t * r, const char * regex_text)
{
    int status = regcomp (r, regex_text, REG_EXTENDED|REG_NEWLINE);
    if (status != 0) {
    char error_message[MAX_ERROR_MSG];
    regerror (status, r, error_message, MAX_ERROR_MSG);
        printf ("Regex error compiling '%s': %s\n",
                 regex_text, error_message);
        return 1;
    }
    return 0;
}

/*
  Match the string in "to_match" against the compiled regular
  expression in "r".
 */

int match_regex(regex_t * r, const char * to_match, char* first_result)
{
    /* "P" is a pointer into the string which points to the end of the
       previous match. */

    const char * p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = 10;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];

    while (1) {
        int i = 0;
        int nomatch = regexec (r, p, n_matches, m, 0);
        if (nomatch || (*p == '\0')) {
            return nomatch;
        }
        for (i = 0; i < n_matches; i++) {
            int start;
            int finish;
            if (m[i].rm_so == -1) {
                break;
            }
            start = m[i].rm_so + (p - to_match);
            finish = m[i].rm_eo + (p - to_match);
            if (i == 0) {
                sprintf(first_result, "%.*s", (finish - start), to_match + start);
            }
        }
        p += m[0].rm_eo;
    }
    return 0;
}

// Function used with permission of <http://creativeandcritical.net/str-replace-c>
char *repl_str(const char *str, const char *old, const char *new) {

    /* Adjust each of the below values to suit your needs. */

    /* Increment positions cache size initially by this number. */
    size_t cache_sz_inc = 16;
    /* Thereafter, each time capacity needs to be increased,
     * multiply the increment by this factor. */
    const size_t cache_sz_inc_factor = 2;
    /* But never increment capacity by more than this number. */
    const size_t cache_sz_inc_max = 1048576;

    char *pret, *ret = NULL;
    const char *pstr2, *pstr = str;
    size_t i, count = 0;
    ptrdiff_t *pos_cache = NULL;
    size_t cache_sz = 0;
    size_t cpylen, orglen, retlen, newlen, oldlen = strlen(old);

    /* Find all matches and cache their positions. */
    while ((pstr2 = strstr(pstr, old)) != NULL) {
        count++;

        /* Increase the cache size when necessary. */
        if (cache_sz < count) {
            cache_sz += cache_sz_inc;
            pos_cache = realloc(pos_cache, sizeof(*pos_cache) * cache_sz);
            if (pos_cache == NULL) {
                goto end_repl_str;
            }
            cache_sz_inc *= cache_sz_inc_factor;
            if (cache_sz_inc > cache_sz_inc_max) {
                cache_sz_inc = cache_sz_inc_max;
            }
        }

        pos_cache[count-1] = pstr2 - str;
        pstr = pstr2 + oldlen;
    }

    orglen = pstr - str + strlen(pstr);

    /* Allocate memory for the post-replacement string. */
    if (count > 0) {
        newlen = strlen(new);
        retlen = orglen + (newlen - oldlen) * count;
    } else  retlen = orglen;
    ret = malloc(retlen + 1);
    if (ret == NULL) {
        goto end_repl_str;
    }

    if (count == 0) {
        /* If no matches, then just duplicate the string. */
        strcpy(ret, str);
    } else {
        /* Otherwise, duplicate the string whilst performing
         * the replacements using the position cache. */
        pret = ret;
        memcpy(pret, str, pos_cache[0]);
        pret += pos_cache[0];
        for (i = 0; i < count; i++) {
            memcpy(pret, new, newlen);
            pret += newlen;
            pstr = str + pos_cache[i] + oldlen;
            cpylen = (i == count-1 ? orglen : pos_cache[i+1]) - pos_cache[i] - oldlen;
            memcpy(pret, pstr, cpylen);
            pret += cpylen;
        }
        ret[retlen] = '\0';
    }

    end_repl_str:
    /* Free the cache and return the post-replacement string,
     * which will be NULL in the event of an error. */
    free(pos_cache);
    return ret;
}
