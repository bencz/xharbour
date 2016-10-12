#ifndef _PBT_H
#define _PBT_H

/* Virtual Power Management Device definitions */

#define PBT_APMQUERYSUSPEND  0x0000
#define PBT_APMQUERYSTANDBY  0x0001
#define PBT_APMQUERYSUSPENDFAILED  0x0002
#define PBT_APMQUERYSTANDBYFAILED  0x0003
#define PBT_APMSUSPEND  0x0004
#define PBT_APMSTANDBY  0x0005
#define PBT_APMRESUMECRITICAL  0x0006
#define PBT_APMRESUMESUSPEND  0x0007
#define PBT_APMRESUMESTANDBY  0x0008
#define PBT_APMBATTERYLOW  0x0009
#define PBT_APMPOWERSTATUSCHANGE  0x000A
#define PBT_APMOEMEVENT  0x000B
#define PBT_APMRESUMEAUTOMATIC  0x0012

#define PBTF_APMRESUMEFROMFAILURE  0x00000001

#endif /* _PBT_H */
