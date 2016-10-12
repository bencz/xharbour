#ifndef _ACCCTRL_H
#define _ACCCTRL_H

/* Windows Access Control API definitions */

#include <wtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#if __POCC__ >= 290
#pragma warn(push)
#pragma warn(disable:2198)  /* Nameless field is not standard */
#endif

#define NO_INHERITANCE  0x0
#define SUB_OBJECTS_ONLY_INHERIT  0x1
#define SUB_CONTAINERS_ONLY_INHERIT  0x2
#define SUB_CONTAINERS_AND_OBJECTS_INHERIT  0x3
#define INHERIT_NO_PROPAGATE  0x4
#define INHERIT_ONLY  0x8
#define INHERITED_ACCESS_ENTRY  0x10

#define INHERITED_PARENT  0x10000000
#define INHERITED_GRANDPARENT  0x20000000

#define ACCCTRL_DEFAULT_PROVIDERA  "Windows NT Access Provider"
#define ACCCTRL_DEFAULT_PROVIDERW  L"Windows NT Access Provider"

#define AccFree LocalFree

#define TRUSTEE_ACCESS_ALLOWED  0x00000001L
#define TRUSTEE_ACCESS_READ  0x00000002L
#define TRUSTEE_ACCESS_WRITE  0x00000004L
#define TRUSTEE_ACCESS_EXPLICIT  0x00000001L
#define TRUSTEE_ACCESS_READ_WRITE  (TRUSTEE_ACCESS_READ|TRUSTEE_ACCESS_WRITE)
#define TRUSTEE_ACCESS_ALL  0xFFFFFFFFL

#define ACTRL_RESERVED  0x00000000
#define ACTRL_PERM_1  0x00000001
#define ACTRL_PERM_2  0x00000002
#define ACTRL_PERM_3  0x00000004
#define ACTRL_PERM_4  0x00000008
#define ACTRL_PERM_5  0x00000010
#define ACTRL_PERM_6  0x00000020
#define ACTRL_PERM_7  0x00000040
#define ACTRL_PERM_8  0x00000080
#define ACTRL_PERM_9  0x00000100
#define ACTRL_PERM_10  0x00000200
#define ACTRL_PERM_11  0x00000400
#define ACTRL_PERM_12  0x00000800
#define ACTRL_PERM_13  0x00001000
#define ACTRL_PERM_14  0x00002000
#define ACTRL_PERM_15  0x00004000
#define ACTRL_PERM_16  0x00008000
#define ACTRL_PERM_17  0x00010000
#define ACTRL_PERM_18  0x00020000
#define ACTRL_PERM_19  0x00040000
#define ACTRL_PERM_20  0x00080000

#define ACTRL_ACCESS_ALLOWED  0x00000001
#define ACTRL_ACCESS_DENIED  0x00000002
#define ACTRL_AUDIT_SUCCESS  0x00000004
#define ACTRL_AUDIT_FAILURE  0x00000008

#define ACTRL_ACCESS_PROTECTED  0x00000001

#define ACTRL_SYSTEM_ACCESS  0x04000000
#define ACTRL_DELETE  0x08000000
#define ACTRL_READ_CONTROL  0x10000000
#define ACTRL_CHANGE_ACCESS  0x20000000
#define ACTRL_CHANGE_OWNER  0x40000000
#define ACTRL_SYNCHRONIZE  0x80000000
#define ACTRL_STD_RIGHTS_ALL  0xf8000000
#define ACTRL_STD_RIGHT_REQUIRED  (ACTRL_STD_RIGHTS_ALL&~ACTRL_SYNCHRONIZE)

#ifndef _DS_CONTROL_BITS_DEFINED_
#define _DS_CONTROL_BITS_DEFINED_
#define ACTRL_DS_OPEN  ACTRL_RESERVED
#define ACTRL_DS_CREATE_CHILD  ACTRL_PERM_1
#define ACTRL_DS_DELETE_CHILD  ACTRL_PERM_2
#define ACTRL_DS_LIST  ACTRL_PERM_3
#define ACTRL_DS_SELF  ACTRL_PERM_4
#define ACTRL_DS_READ_PROP  ACTRL_PERM_5
#define ACTRL_DS_WRITE_PROP  ACTRL_PERM_6
#define ACTRL_DS_DELETE_TREE  ACTRL_PERM_7
#define ACTRL_DS_LIST_OBJECT  ACTRL_PERM_8
#define ACTRL_DS_CONTROL_ACCESS  ACTRL_PERM_9
#endif /* _DS_CONTROL_BITS_DEFINED_ */

#define ACTRL_FILE_READ  ACTRL_PERM_1
#define ACTRL_FILE_WRITE  ACTRL_PERM_2
#define ACTRL_FILE_APPEND  ACTRL_PERM_3
#define ACTRL_FILE_READ_PROP  ACTRL_PERM_4
#define ACTRL_FILE_WRITE_PROP  ACTRL_PERM_5
#define ACTRL_FILE_EXECUTE  ACTRL_PERM_6
#define ACTRL_FILE_READ_ATTRIB  ACTRL_PERM_8
#define ACTRL_FILE_WRITE_ATTRIB  ACTRL_PERM_9
#define ACTRL_FILE_CREATE_PIPE  ACTRL_PERM_10
#define ACTRL_DIR_LIST  ACTRL_PERM_1
#define ACTRL_DIR_CREATE_OBJECT  ACTRL_PERM_2
#define ACTRL_DIR_CREATE_CHILD  ACTRL_PERM_3
#define ACTRL_DIR_DELETE_CHILD  ACTRL_PERM_7
#define ACTRL_DIR_TRAVERSE  ACTRL_PERM_6
#define ACTRL_KERNEL_TERMINATE  ACTRL_PERM_1
#define ACTRL_KERNEL_THREAD  ACTRL_PERM_2
#define ACTRL_KERNEL_VM  ACTRL_PERM_3
#define ACTRL_KERNEL_VM_READ  ACTRL_PERM_4
#define ACTRL_KERNEL_VM_WRITE  ACTRL_PERM_5
#define ACTRL_KERNEL_DUP_HANDLE  ACTRL_PERM_6
#define ACTRL_KERNEL_PROCESS  ACTRL_PERM_7
#define ACTRL_KERNEL_SET_INFO  ACTRL_PERM_8
#define ACTRL_KERNEL_GET_INFO  ACTRL_PERM_9
#define ACTRL_KERNEL_CONTROL  ACTRL_PERM_10
#define ACTRL_KERNEL_ALERT  ACTRL_PERM_11
#define ACTRL_KERNEL_GET_CONTEXT  ACTRL_PERM_12
#define ACTRL_KERNEL_SET_CONTEXT  ACTRL_PERM_13
#define ACTRL_KERNEL_TOKEN  ACTRL_PERM_14
#define ACTRL_KERNEL_IMPERSONATE  ACTRL_PERM_15
#define ACTRL_KERNEL_DIMPERSONATE  ACTRL_PERM_16
#define ACTRL_PRINT_SADMIN  ACTRL_PERM_1
#define ACTRL_PRINT_SLIST  ACTRL_PERM_2
#define ACTRL_PRINT_PADMIN  ACTRL_PERM_3
#define ACTRL_PRINT_PUSE  ACTRL_PERM_4
#define ACTRL_PRINT_JADMIN  ACTRL_PERM_5
#define ACTRL_SVC_GET_INFO  ACTRL_PERM_1
#define ACTRL_SVC_SET_INFO  ACTRL_PERM_2
#define ACTRL_SVC_STATUS  ACTRL_PERM_3
#define ACTRL_SVC_LIST  ACTRL_PERM_4
#define ACTRL_SVC_START  ACTRL_PERM_5
#define ACTRL_SVC_STOP  ACTRL_PERM_6
#define ACTRL_SVC_PAUSE  ACTRL_PERM_7
#define ACTRL_SVC_INTERROGATE  ACTRL_PERM_8
#define ACTRL_SVC_UCONTROL  ACTRL_PERM_9
#define ACTRL_REG_QUERY  ACTRL_PERM_1
#define ACTRL_REG_SET  ACTRL_PERM_2
#define ACTRL_REG_CREATE_CHILD  ACTRL_PERM_3
#define ACTRL_REG_LIST  ACTRL_PERM_4
#define ACTRL_REG_NOTIFY  ACTRL_PERM_5
#define ACTRL_REG_LINK  ACTRL_PERM_6
#define ACTRL_WIN_CLIPBRD  ACTRL_PERM_1
#define ACTRL_WIN_GLOBAL_ATOMS  ACTRL_PERM_2
#define ACTRL_WIN_CREATE  ACTRL_PERM_3
#define ACTRL_WIN_LIST_DESK  ACTRL_PERM_4
#define ACTRL_WIN_LIST  ACTRL_PERM_5
#define ACTRL_WIN_READ_ATTRIBS  ACTRL_PERM_6
#define ACTRL_WIN_WRITE_ATTRIBS  ACTRL_PERM_7
#define ACTRL_WIN_SCREEN  ACTRL_PERM_8
#define ACTRL_WIN_EXIT  ACTRL_PERM_9

#define ACTRL_ACCESS_NO_OPTIONS  0x00000000
#define ACTRL_ACCESS_SUPPORTS_OBJECT_ENTRIES  0x00000001

typedef enum _SE_OBJECT_TYPE {
    SE_UNKNOWN_OBJECT_TYPE = 0,
    SE_FILE_OBJECT,
    SE_SERVICE,
    SE_PRINTER,
    SE_REGISTRY_KEY,
    SE_LMSHARE,
    SE_KERNEL_OBJECT,
    SE_WINDOW_OBJECT,
    SE_DS_OBJECT,
    SE_DS_OBJECT_ALL,
    SE_PROVIDER_DEFINED_OBJECT,
    SE_WMIGUID_OBJECT
} SE_OBJECT_TYPE;

typedef enum _TRUSTEE_TYPE {
    TRUSTEE_IS_UNKNOWN,
    TRUSTEE_IS_USER,
    TRUSTEE_IS_GROUP,
    TRUSTEE_IS_DOMAIN,
    TRUSTEE_IS_ALIAS,
    TRUSTEE_IS_WELL_KNOWN_GROUP,
    TRUSTEE_IS_DELETED,
    TRUSTEE_IS_INVALID,
    TRUSTEE_IS_COMPUTER
} TRUSTEE_TYPE;

typedef enum _TRUSTEE_FORM {
    TRUSTEE_IS_SID,
    TRUSTEE_IS_NAME,
    TRUSTEE_BAD_FORM,
    TRUSTEE_IS_OBJECTS_AND_SID,
    TRUSTEE_IS_OBJECTS_AND_NAME
} TRUSTEE_FORM;

typedef enum _MULTIPLE_TRUSTEE_OPERATION {
    NO_MULTIPLE_TRUSTEE,
    TRUSTEE_IS_IMPERSONATE,
} MULTIPLE_TRUSTEE_OPERATION;

typedef struct _OBJECTS_AND_SID {
    DWORD ObjectsPresent;
    GUID ObjectTypeGuid;
    GUID InheritedObjectTypeGuid;
    SID *pSid;
} OBJECTS_AND_SID, *POBJECTS_AND_SID;

typedef struct _OBJECTS_AND_NAME_A {
    DWORD ObjectsPresent;
    SE_OBJECT_TYPE ObjectType;
    LPSTR ObjectTypeName;
    LPSTR InheritedObjectTypeName;
    LPSTR ptstrName;
} OBJECTS_AND_NAME_A, *POBJECTS_AND_NAME_A;

typedef struct _OBJECTS_AND_NAME_W {
    DWORD ObjectsPresent;
    SE_OBJECT_TYPE ObjectType;
    LPWSTR ObjectTypeName;
    LPWSTR InheritedObjectTypeName;
    LPWSTR ptstrName;
} OBJECTS_AND_NAME_W, *POBJECTS_AND_NAME_W;

typedef struct _TRUSTEE_A {
    struct _TRUSTEE_A *pMultipleTrustee;
    MULTIPLE_TRUSTEE_OPERATION MultipleTrusteeOperation;
    TRUSTEE_FORM TrusteeForm;
    TRUSTEE_TYPE TrusteeType;
    LPSTR ptstrName;
} TRUSTEE_A, *PTRUSTEE_A, TRUSTEEA, *PTRUSTEEA;

typedef struct _TRUSTEE_W {
    struct _TRUSTEE_W *pMultipleTrustee;
    MULTIPLE_TRUSTEE_OPERATION MultipleTrusteeOperation;
    TRUSTEE_FORM TrusteeForm;
    TRUSTEE_TYPE TrusteeType;
    LPWSTR                      ptstrName;
} TRUSTEE_W, *PTRUSTEE_W, TRUSTEEW, *PTRUSTEEW;

typedef enum _ACCESS_MODE {
    NOT_USED_ACCESS = 0,
    GRANT_ACCESS,
    SET_ACCESS,
    DENY_ACCESS,
    REVOKE_ACCESS,
    SET_AUDIT_SUCCESS,
    SET_AUDIT_FAILURE
} ACCESS_MODE;

typedef struct _EXPLICIT_ACCESS_A {
    DWORD grfAccessPermissions;
    ACCESS_MODE grfAccessMode;
    DWORD grfInheritance;
    TRUSTEE_A Trustee;
} EXPLICIT_ACCESS_A, *PEXPLICIT_ACCESS_A, EXPLICIT_ACCESSA, *PEXPLICIT_ACCESSA;

typedef struct _EXPLICIT_ACCESS_W {
    DWORD grfAccessPermissions;
    ACCESS_MODE grfAccessMode;
    DWORD grfInheritance;
    TRUSTEE_W Trustee;
} EXPLICIT_ACCESS_W, *PEXPLICIT_ACCESS_W, EXPLICIT_ACCESSW, *PEXPLICIT_ACCESSW;

typedef ULONG ACCESS_RIGHTS, *PACCESS_RIGHTS;
typedef ULONG INHERIT_FLAGS, *PINHERIT_FLAGS;

typedef struct _ACTRL_ACCESS_ENTRYA {
    TRUSTEE_A Trustee;
    ULONG fAccessFlags;
    ACCESS_RIGHTS Access;
    ACCESS_RIGHTS ProvSpecificAccess;
    INHERIT_FLAGS Inheritance;
    LPSTR lpInheritProperty;
} ACTRL_ACCESS_ENTRYA, *PACTRL_ACCESS_ENTRYA;

typedef struct _ACTRL_ACCESS_ENTRYW {
    TRUSTEE_W Trustee;
    ULONG fAccessFlags;
    ACCESS_RIGHTS Access;
    ACCESS_RIGHTS ProvSpecificAccess;
    INHERIT_FLAGS Inheritance;
    LPWSTR lpInheritProperty;
} ACTRL_ACCESS_ENTRYW, *PACTRL_ACCESS_ENTRYW;

typedef struct _ACTRL_ACCESS_ENTRY_LISTA {
    ULONG cEntries;
    ACTRL_ACCESS_ENTRYA *pAccessList;
} ACTRL_ACCESS_ENTRY_LISTA, *PACTRL_ACCESS_ENTRY_LISTA;

typedef struct _ACTRL_ACCESS_ENTRY_LISTW {
    ULONG cEntries;
    ACTRL_ACCESS_ENTRYW *pAccessList;
} ACTRL_ACCESS_ENTRY_LISTW, *PACTRL_ACCESS_ENTRY_LISTW;

typedef struct _ACTRL_PROPERTY_ENTRYA {
    LPSTR lpProperty;
    PACTRL_ACCESS_ENTRY_LISTA pAccessEntryList;
    ULONG fListFlags;
} ACTRL_PROPERTY_ENTRYA, *PACTRL_PROPERTY_ENTRYA;

typedef struct _ACTRL_PROPERTY_ENTRYW {
    LPWSTR lpProperty;
    PACTRL_ACCESS_ENTRY_LISTW pAccessEntryList;
    ULONG fListFlags;
} ACTRL_PROPERTY_ENTRYW, *PACTRL_PROPERTY_ENTRYW;

typedef struct _ACTRL_ALISTA {
    ULONG cEntries;
    PACTRL_PROPERTY_ENTRYA pPropertyAccessList;
} ACTRL_ACCESSA, *PACTRL_ACCESSA, ACTRL_AUDITA, *PACTRL_AUDITA;

typedef struct _ACTRL_ALISTW {
    ULONG cEntries;
    PACTRL_PROPERTY_ENTRYW pPropertyAccessList;
} ACTRL_ACCESSW, *PACTRL_ACCESSW, ACTRL_AUDITW, *PACTRL_AUDITW;

typedef struct _TRUSTEE_ACCESSA {
    LPSTR lpProperty;
    ACCESS_RIGHTS Access;
    ULONG fAccessFlags;
    ULONG fReturnedAccess;
} TRUSTEE_ACCESSA, *PTRUSTEE_ACCESSA;

typedef struct _TRUSTEE_ACCESSW {
    LPWSTR lpProperty;
    ACCESS_RIGHTS Access;
    ULONG fAccessFlags;
    ULONG fReturnedAccess;
} TRUSTEE_ACCESSW, *PTRUSTEE_ACCESSW;

typedef struct _ACTRL_OVERLAPPED {
    union {
        PVOID Provider;
        ULONG Reserved1;
    };
    ULONG Reserved2;
    HANDLE hEvent;
} ACTRL_OVERLAPPED, *PACTRL_OVERLAPPED;

typedef struct _ACTRL_ACCESS_INFOA {
    ULONG fAccessPermission;
    LPSTR lpAccessPermissionName;
} ACTRL_ACCESS_INFOA, *PACTRL_ACCESS_INFOA;

typedef struct _ACTRL_ACCESS_INFOW {
    ULONG fAccessPermission;
    LPWSTR lpAccessPermissionName;
} ACTRL_ACCESS_INFOW, *PACTRL_ACCESS_INFOW;

typedef struct _ACTRL_CONTROL_INFOA {
    LPSTR lpControlId;
    LPSTR lpControlName;
} ACTRL_CONTROL_INFOA, *PACTRL_CONTROL_INFOA;

typedef struct _ACTRL_CONTROL_INFOW {
    LPWSTR lpControlId;
    LPWSTR lpControlName;
} ACTRL_CONTROL_INFOW, *PACTRL_CONTROL_INFOW;

#ifdef UNICODE
typedef OBJECTS_AND_NAME_W OBJECTS_AND_NAME_;
typedef POBJECTS_AND_NAME_W POBJECTS_AND_NAME_;
typedef TRUSTEE_W TRUSTEE_;
typedef PTRUSTEE_W PTRUSTEE_;
typedef TRUSTEEW TRUSTEE;
typedef PTRUSTEEW PTRUSTEE;
typedef EXPLICIT_ACCESS_W EXPLICIT_ACCESS_;
typedef PEXPLICIT_ACCESS_W PEXPLICIT_ACCESS_;
typedef EXPLICIT_ACCESSW EXPLICIT_ACCESS;
typedef PEXPLICIT_ACCESSW PEXPLICIT_ACCESS;
typedef ACTRL_ACCESS_ENTRYW ACTRL_ACCESS_ENTRY;
typedef PACTRL_ACCESS_ENTRYW PACTRL_ACCESS_ENTRY;
typedef ACTRL_ACCESS_ENTRY_LISTW ACTRL_ACCESS_ENTRY_LIST;
typedef PACTRL_ACCESS_ENTRY_LISTW PACTRL_ACCESS_ENTRY_LIST;
typedef ACTRL_PROPERTY_ENTRYW ACTRL_PROPERTY_ENTRY;
typedef PACTRL_PROPERTY_ENTRYW PACTRL_PROPERTY_ENTRY;
typedef ACTRL_ACCESSW ACTRL_ACCESS;
typedef PACTRL_ACCESSW PACTRL_ACCESS;
typedef ACTRL_AUDITW ACTRL_AUDIT;
typedef PACTRL_AUDITW PACTRL_AUDIT;
typedef TRUSTEE_ACCESSW TRUSTEE_ACCESS;
typedef PTRUSTEE_ACCESSW PTRUSTEE_ACCESS;
typedef ACTRL_ACCESS_INFOW ACTRL_ACCESS_INFO;
typedef PACTRL_ACCESS_INFOW PACTRL_ACCESS_INFO;
typedef ACTRL_CONTROL_INFOW ACTRL_CONTROL_INFO;
typedef PACTRL_CONTROL_INFOW PACTRL_CONTROL_INFO;
#define ACCCTRL_DEFAULT_PROVIDER ACCCTRL_DEFAULT_PROVIDERW
#else
typedef OBJECTS_AND_NAME_A OBJECTS_AND_NAME_;
typedef POBJECTS_AND_NAME_A POBJECTS_AND_NAME_;
typedef TRUSTEE_A TRUSTEE_;
typedef PTRUSTEE_A PTRUSTEE_;
typedef TRUSTEEA TRUSTEE;
typedef PTRUSTEEA PTRUSTEE;
typedef EXPLICIT_ACCESS_A EXPLICIT_ACCESS_;
typedef PEXPLICIT_ACCESS_A PEXPLICIT_ACCESS_;
typedef EXPLICIT_ACCESSA EXPLICIT_ACCESS;
typedef PEXPLICIT_ACCESSA PEXPLICIT_ACCESS;
typedef ACTRL_ACCESS_ENTRYA ACTRL_ACCESS_ENTRY;
typedef PACTRL_ACCESS_ENTRYA PACTRL_ACCESS_ENTRY;
typedef ACTRL_ACCESS_ENTRY_LISTA ACTRL_ACCESS_ENTRY_LIST;
typedef PACTRL_ACCESS_ENTRY_LISTA PACTRL_ACCESS_ENTRY_LIST;
typedef ACTRL_PROPERTY_ENTRYA ACTRL_PROPERTY_ENTRY;
typedef PACTRL_PROPERTY_ENTRYA PACTRL_PROPERTY_ENTRY;
typedef ACTRL_ACCESSA ACTRL_ACCESS;
typedef PACTRL_ACCESSA PACTRL_ACCESS;
typedef ACTRL_AUDITA ACTRL_AUDIT;
typedef PACTRL_AUDITA PACTRL_AUDIT;
typedef TRUSTEE_ACCESSA TRUSTEE_ACCESS;
typedef PTRUSTEE_ACCESSA PTRUSTEE_ACCESS;
typedef ACTRL_ACCESS_INFOA ACTRL_ACCESS_INFO;
typedef PACTRL_ACCESS_INFOA PACTRL_ACCESS_INFO;
typedef ACTRL_CONTROL_INFOA ACTRL_CONTROL_INFO;
typedef PACTRL_CONTROL_INFOA PACTRL_CONTROL_INFO;
#define ACCCTRL_DEFAULT_PROVIDER ACCCTRL_DEFAULT_PROVIDERA
#endif /* UNICODE */

#if __POCC__ >= 290
#pragma warn(pop)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _ACCCTRL_H */

