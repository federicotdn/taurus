#include "include/words.h"

const char *signs_words[SIGNS_WORDS_COUNT] = {
    "aries", "tauro", "geminis", "cancer", "leo", "virgo", "libra", "escorpio", "sagitario",
    "capricornio", "acuario", "piscis",
    "ARIES", "TAURO", "GEMINIS", "CANCER", "LEO", "VIRGO", "LIBRA", "ESCORPIO", "SAGITARIO",
    "CAPRICORNIO", "ACUARIO", "PISCIS",
};

const char *stars_words[STARS_WORDS_COUNT] = {
    "SOL", "A.CENTAURI", "BARNARD", "LUHMAN16", "WISE0855", "WOLF359", "LALANDE", "SIRIO", 
    "LUYTEN", "ROSS154", "ROSS248", "RAN", "LACAILLE", "ROSS128", "AQUARII", "PROCYON", "CYGNI",
    "STRUVE", "INDI", "DX.CANCRI", "TAU.CETI"
};

const char *errors_words[ERRORS_WORDS_COUNT] = {
    "ERROR", "FATAL ERR", "FAILURE", "ERR", "PANIC", "404", "NULL", "STOP", "RESET",
    "CRASH", "BAD ALLOC"
};

const char *cmds_words[CMDS_WORDS_COUNT] = {
    "", "./tauro.exe", "RUN", "STAR... OK", "SUN...  OK", "MOON... OK", "VENUS DATA:",
    "EPOCH J2000", "92 BAR", "9.2MPA", "DATA LOADED", "TOTAL 32KB", "EXIT 0"
};

const char *event_words[EVENT_WORDS_COUNT] = {
    "FIESTA", "ALCOHOL", "CUMBIA", "TAURO", "ANARCOAFECCION", "TECHNO", "SABADO", "13 MAYO" 
};