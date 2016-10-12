#ifndef _LMJOIN_H
#define _LMJOIN_H

/* NetSetup API definitions */

#ifdef __cplusplus
extern "C" {
#endif

#define NETSETUP_JOIN_DOMAIN  0x00000001
#define NETSETUP_ACCT_CREATE  0x00000002
#define NETSETUP_ACCT_DELETE  0x00000004
#define NETSETUP_WIN9X_UPGRADE  0x00000010
#define NETSETUP_DOMAIN_JOIN_IF_JOINED  0x00000020
#define NETSETUP_JOIN_UNSECURE  0x00000040
#define NETSETUP_INSTALL_INVOCATION  0x00040000

typedef enum _NETSETUP_NAME_TYPE {
    NetSetupUnknown = 0,
    NetSetupMachine,
    NetSetupWorkgroup,
    NetSetupDomain,
    NetSetupNonExistentDomain,
#if (_WIN32_WINNT >= 0x0500)
    NetSetupDnsMachine
#endif
} NETSETUP_NAME_TYPE, *PNETSETUP_NAME_TYPE;

typedef enum _NETSETUP_JOIN_STATUS {
    NetSetupUnknownStatus = 0,
    NetSetupUnjoined,
    NetSetupWorkgroupName,
    NetSetupDomainName
} NETSETUP_JOIN_STATUS, *PNETSETUP_JOIN_STATUS;

NET_API_STATUS NET_API_FUNCTION NetJoinDomain(LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,DWORD);
NET_API_STATUS NET_API_FUNCTION NetUnjoinDomain(LPCWSTR,LPCWSTR,LPCWSTR,DWORD);
NET_API_STATUS NET_API_FUNCTION NetRenameMachineInDomain(LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,DWORD);
NET_API_STATUS NET_API_FUNCTION NetValidateName(LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,NETSETUP_NAME_TYPE);
NET_API_STATUS NET_API_FUNCTION NetGetJoinInformation(LPCWSTR,LPWSTR*,PNETSETUP_JOIN_STATUS);
NET_API_STATUS NET_API_FUNCTION NetGetJoinableOUs(LPCWSTR,LPCWSTR,LPCWSTR,LPCWSTR,DWORD*,LPWSTR**);

#ifdef __cplusplus
}
#endif

#endif /* _LMJOIN_H */
