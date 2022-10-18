/* Author: Adil Kasim
 * Date Created: Mon. Sep. 09, 2022
 * Last Edited: Mon. Oct. 17, 2022
 * */

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*  VERSION CONTROL */
#define VERSION 2.0

/* WINDOWS STUFF */
#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

/* DEFINES */
#define MAX_CHARS 255
#define SUCCESS 0
#define FAILURE -1

/* Colors */
#define RED       "\033[1;91m"
#define GREEN     "\033[1;92m"
#define YELLOW    "\033[1;93m"
#define MAGENTA   "\033[1;95m"
#define COLOR_END "\33[0m"

/* Defined Functions */
FILE *popen(const char *command, const char *mode);

int pclose(FILE *stream);

size_t strnlen(const char *s, size_t maxlen);

#undef strlcpy
size_t strlcpy(char *dst, const char *src, size_t maxlen) {
    const size_t srclen = strlen(src);
    if (srclen + 1 < maxlen) {
        memcpy(dst, src, srclen + 1);
    } else if (maxlen != 0) {
        memcpy(dst, src, maxlen - 1);
        dst[maxlen - 1] = '\0';
    }
    return srclen;
}

#undef strlcat
size_t strlcat(char *dst, const char *src, size_t maxlen) {
    const size_t srclen = strlen(src);
    const size_t dstlen = strnlen(dst, maxlen);
    if (dstlen == maxlen) {
        return maxlen + srclen;
    }
    if (srclen < maxlen-dstlen) {
        memcpy(dst+dstlen, src, srclen + 1);
    } else {
        memcpy(dst+dstlen, src, maxlen-dstlen - 1);
        dst[maxlen - 1] = '\0';
    }
    return dstlen + srclen;
}

/* FUNCTIONS */
int make() {
    if (access("Makefile", F_OK) == 0) {
        system("make");
        return SUCCESS;
    }
    return FAILURE;
}

int get_first(int num) {
    while(num >= 10) {
        num /= 10;
    }
    return num;
}

void edit_cmp(int test_num, char *cmp) {
    /* EDIT CMP CMDS*/
    if (test_num < 10) {
        cmp[28] = (test_num + '0');
    } else {
        cmp[27] = (get_first(test_num) + '0');
        cmp[28] = ((test_num % 10) + '0');
    }
}

int run_diff(int test_num, char *cmp, FILE *shell, char*opt) {
    int err = 0;
    /* RUN DIFF CHECKER */
    shell = popen(cmp, "r");
    if (shell == NULL) { /* SHELL UNABLE TO OPEN */
        printf("%s%s%s\n", MAGENTA, "ERROR: UNABLE TO COMPARE", COLOR_END); /* ERROR CODE HERE */
        return FAILURE;
    } else {
        while (fgets(opt, MAX_CHARS, shell) != NULL) { /* IF THE TEST FAILS */
            err++;
            printf("%s%s%s", RED, opt, COLOR_END);
        }
        if (err > 0) { /* IF THE TEST FAILS */
            printf("%s%s%s%d%s%s%s", RED  , "<---------- Test: ", COLOR_END,
                   test_num, RED  , " failed ---------->\n", COLOR_END);
            err = 0;
        } else { /* IF THE TEST PASSES */
            printf("%s%s%s%d%s%s%s", GREEN, "<---------- Test: ", COLOR_END,
                   test_num, GREEN, " passed ---------->\n", COLOR_END);
        }
    }
    pclose(shell);

    return SUCCESS;
}

int run_test(int test_num, char *run, char *cmp, FILE *shell, char *opt) {
    /* RUN TEST COMMAND*/
    system(run);

    /* RUN DIFF CHECKER */
    if (run_diff(test_num, cmp, shell, opt) == FAILURE) {
        printf("%s%s%s\n", MAGENTA, "ERROR: diff CHECK COULD NOT BE RUN", COLOR_END); /* ERROR CODE HERE */
        return FAILURE;
    }

    return SUCCESS;
}

int in_test(int test_num, char *run, char *cmp, FILE *shell, char *opt) {
    /* EDIT RUN AND CMP CMDS*/
    if (test_num < 10) {
        run[15] = (test_num + '0'); /* RUN */
    } else {
        run[14] = (get_first(test_num) + '0'); /*  RUN */
        run[15] = ((test_num % 10) + '0'); /* RUN */
    }
    edit_cmp(test_num, cmp);

    if (run_test(test_num, run, cmp, shell, opt) == FAILURE) {
        printf("%s%s%s\n", MAGENTA, "ERROR: TEST COULD NOT BE RUN", COLOR_END); /* ERROR CODE HERE */
        return FAILURE;
    }

    return SUCCESS;
}

int c_test(int test_num, char *run, char *cmp, FILE *shell, char *opt) {
    if (test_num < 10) {
        run[11] = (test_num + '0'); /* RUN */
    } else {
        run[10] = (get_first(test_num) + '0'); /*  RUN */
        run[11] = ((test_num % 10) + '0'); /* RUN */
    }
    edit_cmp(test_num, cmp);

    if (run_test(test_num, run, cmp, shell, opt) == FAILURE) {
        printf("%s%s%s\n", MAGENTA, "ERROR: TEST COULD NOT BE RUN", COLOR_END); /* ERROR CODE HERE */
        return FAILURE;
    }

    return SUCCESS;
}

int c_make_test(int test_num, char *run, char *cmp, FILE *shell, char *opt) {
    /* EDIT RUN AND CMP CMDS*/
    if (test_num < 10) {
        run[7] = (test_num + '0'); /* RUN */
    } else {
        run[6] = (get_first(test_num) + '0'); /*  RUN */
        run[7] = ((test_num % 10) + '0'); /* RUN */
    }
    edit_cmp(test_num, cmp);

    if (run_test(test_num, run, cmp, shell, opt) == FAILURE) {
        printf("%s%s%s\n", MAGENTA, "ERROR: TEST COULD NOT BE RUN", COLOR_END); /* ERROR CODE HERE */
        return FAILURE;
    }

    return SUCCESS;
}

/* MAIN */
int main(int argc, char *argv[]) {
    /* VARIABLES */
    char cmp_test[] = "diff -U 0 -b results public00.output";
    char num_tester[] = "public00.output";
    char opt[MAX_CHARS + 1], run[MAX_CHARS + 1];
    int i, num_tests, num_right = 0;
    FILE *shell = NULL;

    opt[MAX_CHARS] = '\0';
    run[MAX_CHARS] = '\0';

    /* CHECK NUMBER OF ARGUMENTS */
    if (argc == 4) {
        /* Check if File to Test Exists */
        if (access(argv[1], F_OK) != 0) {
            printf("%s%s%s\n", MAGENTA, "ERROR: FILE NAME INCORRECT", COLOR_END); /* ERROR CODE HERE */
            return 0;
        }
        if (atoi(argv[3]) > 0 && atoi(argv[3]) < 100) {
            num_tests = atoi(argv[3]);

            if (num_tests < 10) {
                num_tester[7] = (num_tests + '0');
            } else {
                num_tester[6] = ((num_tests % 10) + '0');
                num_tester[7] = (get_first(num_tests) + '0');
            }

            if (access(num_tester, F_OK) != 0) {
                printf("%s%s%s\n", MAGENTA, "ERROR: INVALID NUMBER OF TESTS", COLOR_END); /* ERROR CODE HERE */
                return 0;
            }
        } else {
            printf("%s%s%s\n", MAGENTA, "ERROR: INVALID NUMBER OF TESTS", COLOR_END); /* ERROR CODE HERE */
            return 0;
        }
        /* If Tests Are '.in' Files */
        if (strcmp(argv[2], "in") == 0) {
            if (access("a.out", F_OK) != 0) { /* 'a.out' FILE Does Not Exist */
                printf("%s%s%s\n", MAGENTA, "ERROR: NO 'a.out' FILE FOUND", COLOR_END); /* ERROR CODE HERE */
                return 0;
            } else { /* 'a.out' FILE Exists */
                strlcpy(run, "a.out < public00.in > results", 30);
                for (i = 1; i < num_tests + 1; i++) {
                    if (in_test(i, run, cmp_test, shell, opt) == FAILURE) {
                        printf("%s%s%s\n", MAGENTA, "ERROR: TEST NOT RUN", COLOR_END); /* ERROR CODE HERE */
                        return 0;
                    } else {
                        num_right++;
                    }
                }
            }
            /* If Tests are '.c' Files */
        } else if (strcmp(argv[2], "c") == 0) {
            if (make() == SUCCESS) { /* Makefile Exits */
                strlcpy(run, "public00 > results", 19);
                for (i = 1; i < num_tests + 1; i++) {
                    if (c_make_test(i, run, cmp_test, shell, opt) == FAILURE) {
                        printf("%s%s%s\n", MAGENTA, "ERROR: TEST NOT RUN", COLOR_END); /* ERROR CODE HERE */
                        return 0;
                    } else {
                        num_right++;
                    }
                }
            } else { /* Makefile Does Not Exist */
                strlcpy(run, "gcc public00.c ", 16);
                strlcat(run, argv[1], strlen(argv[1]));
                strlcat(run, "; a.out > results", 18);
                for (i = 1; i < num_tests + 1; i++) {
                    if (c_test(i, run, cmp_test, shell, opt) == FAILURE) {
                        printf("%s%s%s\n", MAGENTA, "ERROR: TEST NOT RUN", COLOR_END); /* ERROR CODE HERE */
                        return 0;
                    } else {
                        num_right++;
                    }
                }
            }
        } else {
            printf("%s%s%s\n", MAGENTA, "ERROR: INVALID TEST TYPE", COLOR_END); /* ERROR CODE HERE */
            return 0;
        }
    } else {
        printf("%s%s%s\n", MAGENTA, "INCORRECT NUMBER OF ARGUMENTS", COLOR_END); /* ERROR CODE HERE */
    }

    if (num_right == 0) {
        printf("%d%s%s%s%d%s%s%s\n", num_right, RED, " out of ", COLOR_END,
               num_tests, RED, " tests passed", COLOR_END);
    } else if (num_right > 0 && num_right < num_tests) {
        printf("%d%s%s%s%d%s%s%s\n", num_right, YELLOW, " out of ", COLOR_END,
               num_tests, YELLOW, " tests passed", COLOR_END);
    } else {
        printf("%d%s%s%s%d%s%s%s\n", num_right, GREEN, " out of ", COLOR_END,
               num_tests, GREEN, " tests passed", COLOR_END);
    }

    return 0;
}