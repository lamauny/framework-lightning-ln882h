#ifndef __OS_VER_H
#define __OS_VER_H


/*------   Version   ------*/
#define OS_VERSION_MAJOR    8    /* for breaking interface changes  */
#define OS_VERSION_MINOR    12    /* for new (non-breaking) interface capabilities */
#define OS_VERSION_REVISION 4    /* for tweaks, bug-fixes, or development */
#define OS_BUILD_NUMBER     0

#define OS_VERSION_NUMBER (OS_VERSION_MAJOR *100*100 + OS_VERSION_MINOR *100 + OS_VERSION_REVISION)

#define OS_LIB_VERSION OS_VERSION_MAJOR.OS_VERSION_MINOR.OS_VERSION_REVISION(OS_BUILD_NUMBER)
#define OS_QUOTE(str) #str
#define OS_EXPAND_AND_QUOTE(str) OS_QUOTE(str)
#define OS_VERSION_STRING OS_EXPAND_AND_QUOTE(OS_LIB_VERSION)  /* requires v1.7.3+ */


#endif
