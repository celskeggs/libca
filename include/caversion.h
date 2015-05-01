#ifndef LIBCA_VERSION

#define LIBCA_MAJOR 0
#define LIBCA_MINOR 1
#define LIBCA_PATCH 0
#define _LC_CCT(ma,mi,pt) #ma "." #mi "." #pt
#define LIBCA_VERSION _LC_CCT(LIBCA_MAJOR, LIBCA_MINOR, LIBCA_PATCH)
#undef _LC_CCT

#endif
