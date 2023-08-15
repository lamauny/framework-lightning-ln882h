#ifndef __PLATFORM_H_
#define __PLATFORM_H_

/*------   Version   ------*/
#define ZSDK_VERSION_MAJOR    0    /* for breaking interface changes  */
#define ZSDK_VERSION_MINOR    3    /* for new (non-breaking) interface capabilities */
#define ZSDK_VERSION_REVISION 2    /* for tweaks, bug-fixes, or development */
#define ZSDK_BUILD_NUMBER     0

#define ZSDK_VERSION_NUMBER (ZSDK_VERSION_MAJOR *100*100 + ZSDK_VERSION_MINOR *100 + ZSDK_VERSION_REVISION)

#define ZSDK_LIB_VERSION ZSDK_VERSION_MAJOR.ZSDK_VERSION_MINOR.ZSDK_VERSION_REVISION(ZSDK_BUILD_NUMBER)
#define ZSDK_QUOTE(str) #str
#define ZSDK_EXPAND_AND_QUOTE(str) ZSDK_QUOTE(str)
#define ZSDK_VERSION_STRING ZSDK_EXPAND_AND_QUOTE(ZSDK_LIB_VERSION)  /* requires v1.7.3+ */

#endif 
