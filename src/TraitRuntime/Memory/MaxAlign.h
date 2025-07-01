#ifndef MAXALIGN_H
#define MAXALIGN_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(_MSC_VER)
    #define TRM_MAX_ALIGN sizeof(max_align_t)
#else
    typedef union {
        long long ll;
        long double ld;
        void* p;
    } trm_max_align_t;
#define TRM_MAX_ALIGN sizeof(trm_max_align_t)
#endif

#endif //MAXALIGN_H
