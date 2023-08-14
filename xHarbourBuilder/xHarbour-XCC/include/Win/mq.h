#ifndef _MQ_H
#define _MQ_H

/* Message Queue API definitions */

#ifndef __transact_h__
typedef void ITransaction;
#endif

#define PRLT  (0)
#define PRLE  (1)
#define PRGT  (2)
#define PRGE  (3)
#define PREQ  (4)
#define PRNE  (5)

#define QUERY_SORTASCEND  (0)
#define QUERY_SORTDESCEND  (1)

#define MQ_MAX_Q_NAME_LEN  124
#define MQ_MAX_Q_LABEL_LEN  124
#define MQ_MAX_MSG_LABEL_LEN  250

#define MQ_RECEIVE_ACCESS  0x00000001
#define MQ_SEND_ACCESS  0x00000002
#define MQ_PEEK_ACCESS  0x00000020

#define MQ_DENY_NONE  0x00000000
#define MQ_DENY_RECEIVE_SHARE  0x00000001

#define MQ_ACTION_RECEIVE  0x00000000
#define MQ_ACTION_PEEK_CURRENT  0x80000000
#define MQ_ACTION_PEEK_NEXT  0x80000001

#define MQ_NO_TRANSACTION  NULL
#define MQ_MTS_TRANSACTION  (ITransaction *)1
#define MQ_XA_TRANSACTION  (ITransaction *)2
#define MQ_SINGLE_MESSAGE  (ITransaction *)3

#define MQ_MIN_PRIORITY  0
#define MQ_MAX_PRIORITY  7

#define PROPID_M_BASE  0
#define PROPID_M_CLASS  (PROPID_M_BASE+1)
#define PROPID_M_MSGID  (PROPID_M_BASE+2)
#define PROPID_M_CORRELATIONID  (PROPID_M_BASE+3)
#define PROPID_M_PRIORITY  (PROPID_M_BASE+4)
#define PROPID_M_DELIVERY  (PROPID_M_BASE+5)
#define PROPID_M_ACKNOWLEDGE  (PROPID_M_BASE+6)
#define PROPID_M_JOURNAL  (PROPID_M_BASE+7)
#define PROPID_M_APPSPECIFIC  (PROPID_M_BASE+8)
#define PROPID_M_BODY  (PROPID_M_BASE+9)
#define PROPID_M_BODY_SIZE  (PROPID_M_BASE+10)
#define PROPID_M_LABEL  (PROPID_M_BASE+11)
#define PROPID_M_LABEL_LEN  (PROPID_M_BASE+12)
#define PROPID_M_TIME_TO_REACH_QUEUE (PROPID_M_BASE+13)
#define PROPID_M_TIME_TO_BE_RECEIVED (PROPID_M_BASE+14)
#define PROPID_M_RESP_QUEUE  (PROPID_M_BASE+15)
#define PROPID_M_RESP_QUEUE_LEN  (PROPID_M_BASE+16)
#define PROPID_M_ADMIN_QUEUE  (PROPID_M_BASE+17)
#define PROPID_M_ADMIN_QUEUE_LEN  (PROPID_M_BASE+18)
#define PROPID_M_VERSION  (PROPID_M_BASE+19)
#define PROPID_M_SENDERID  (PROPID_M_BASE+20)
#define PROPID_M_SENDERID_LEN  (PROPID_M_BASE+21)
#define PROPID_M_SENDERID_TYPE  (PROPID_M_BASE+22)
#define PROPID_M_PRIV_LEVEL  (PROPID_M_BASE+23)
#define PROPID_M_AUTH_LEVEL  (PROPID_M_BASE+24)
#define PROPID_M_AUTHENTICATED  (PROPID_M_BASE+25)
#define PROPID_M_HASH_ALG  (PROPID_M_BASE+26)
#define PROPID_M_ENCRYPTION_ALG  (PROPID_M_BASE+27)
#define PROPID_M_SENDER_CERT  (PROPID_M_BASE+28)
#define PROPID_M_SENDER_CERT_LEN  (PROPID_M_BASE+29)
#define PROPID_M_SRC_MACHINE_ID  (PROPID_M_BASE+30)
#define PROPID_M_SENTTIME  (PROPID_M_BASE+31)
#define PROPID_M_ARRIVEDTIME  (PROPID_M_BASE+32)
#define PROPID_M_DEST_QUEUE  (PROPID_M_BASE+33)
#define PROPID_M_DEST_QUEUE_LEN  (PROPID_M_BASE+34)
#define PROPID_M_EXTENSION  (PROPID_M_BASE+35)
#define PROPID_M_EXTENSION_LEN  (PROPID_M_BASE+36)
#define PROPID_M_SECURITY_CONTEXT  (PROPID_M_BASE+37)
#define PROPID_M_CONNECTOR_TYPE  (PROPID_M_BASE+38)
#define PROPID_M_XACT_STATUS_QUEUE  (PROPID_M_BASE+39)
#define PROPID_M_XACT_STATUS_QUEUE_LEN (PROPID_M_BASE+40)
#define PROPID_M_TRACE  (PROPID_M_BASE+41)
#define PROPID_M_BODY_TYPE  (PROPID_M_BASE+42)
#define PROPID_M_DEST_SYMM_KEY  (PROPID_M_BASE+43)
#define PROPID_M_DEST_SYMM_KEY_LEN  (PROPID_M_BASE+44)
#define PROPID_M_SIGNATURE  (PROPID_M_BASE+45)
#define PROPID_M_SIGNATURE_LEN  (PROPID_M_BASE+46)
#define PROPID_M_PROV_TYPE  (PROPID_M_BASE+47)
#define PROPID_M_PROV_NAME  (PROPID_M_BASE+48)
#define PROPID_M_PROV_NAME_LEN  (PROPID_M_BASE+49)
#define PROPID_M_FIRST_IN_XACT  (PROPID_M_BASE+50)
#define PROPID_M_LAST_IN_XACT  (PROPID_M_BASE+51)
#define PROPID_M_XACTID  (PROPID_M_BASE+52)
#define PROPID_M_AUTHENTICATED_EX  (PROPID_M_BASE+53)

#define PROPID_M_MSGID_SIZE  20
#define PROPID_M_CORRELATIONID_SIZE 20
#define PROPID_M_XACTID_SIZE  20

#define MQCLASS_CODE(s,r,code) ((USHORT)(((s) << 15) | ((r) << 14) | (code)))
#define MQCLASS_NACK(c)  ((c) & 0x8000)
#define MQCLASS_RECEIVE(c)  ((c) & 0x4000)

#define MQMSG_CLASS_NORMAL  MQCLASS_CODE(0,0,0x00)
#define MQMSG_CLASS_REPORT  MQCLASS_CODE(0,0,0x01)
#define MQMSG_CLASS_ACK_REACH_QUEUE  MQCLASS_CODE(0,0,0x02)
#define MQMSG_CLASS_ACK_RECEIVE  MQCLASS_CODE(0,1,0x00)
#define MQMSG_CLASS_NACK_BAD_DST_Q  MQCLASS_CODE(1,0,0x00)
#define MQMSG_CLASS_NACK_PURGED  MQCLASS_CODE(1,0,0x01)
#define MQMSG_CLASS_NACK_REACH_QUEUE_TIMEOUT  MQCLASS_CODE(1,0,0x02)
#define MQMSG_CLASS_NACK_Q_EXCEED_QUOTA  MQCLASS_CODE(1,0,0x03)
#define MQMSG_CLASS_NACK_ACCESS_DENIED  MQCLASS_CODE(1,0,0x04)
#define MQMSG_CLASS_NACK_HOP_COUNT_EXCEEDED  MQCLASS_CODE(1,0,0x05)
#define MQMSG_CLASS_NACK_BAD_SIGNATURE  MQCLASS_CODE(1,0,0x06)
#define MQMSG_CLASS_NACK_BAD_ENCRYPTION  MQCLASS_CODE(1,0,0x07)
#define MQMSG_CLASS_NACK_COULD_NOT_ENCRYPT  MQCLASS_CODE(1,0,0x08)
#define MQMSG_CLASS_NACK_NOT_TRANSACTIONAL_Q  MQCLASS_CODE(1,0,0x09)
#define MQMSG_CLASS_NACK_NOT_TRANSACTIONAL_MSG  MQCLASS_CODE(1,0,0x0A)
#define MQMSG_CLASS_NACK_UNSUPPORTED_CRYPTO_PROVIDER  MQCLASS_CODE(1,0,0x0B)
#define MQMSG_CLASS_NACK_Q_DELETED  MQCLASS_CODE(1,1,0x00)
#define MQMSG_CLASS_NACK_Q_PURGED  MQCLASS_CODE(1,1,0x01)
#define MQMSG_CLASS_NACK_RECEIVE_TIMEOUT  MQCLASS_CODE(1,1,0x02)
#define MQMSG_CLASS_NACK_RECEIVE_TIMEOUT_AT_SENDER  MQCLASS_CODE(1,1,0x03)

#define MQMSG_ACKNOWLEDGMENT_NONE  0x00

#define MQMSG_ACKNOWLEDGMENT_POS_ARRIVAL  0x01
#define MQMSG_ACKNOWLEDGMENT_POS_RECEIVE  0x02
#define MQMSG_ACKNOWLEDGMENT_NEG_ARRIVAL  0x04
#define MQMSG_ACKNOWLEDGMENT_NEG_RECEIVE  0x08

#define MQMSG_ACKNOWLEDGMENT_NACK_REACH_QUEUE ((UCHAR)(MQMSG_ACKNOWLEDGMENT_NEG_ARRIVAL))
#define MQMSG_ACKNOWLEDGMENT_FULL_REACH_QUEUE ((UCHAR)(MQMSG_ACKNOWLEDGMENT_NEG_ARRIVAL|MQMSG_ACKNOWLEDGMENT_POS_ARRIVAL))
#define MQMSG_ACKNOWLEDGMENT_NACK_RECEIVE ((UCHAR)(MQMSG_ACKNOWLEDGMENT_NEG_ARRIVAL|MQMSG_ACKNOWLEDGMENT_NEG_RECEIVE))
#define MQMSG_ACKNOWLEDGMENT_FULL_RECEIVE ((UCHAR)(MQMSG_ACKNOWLEDGMENT_NEG_ARRIVAL|MQMSG_ACKNOWLEDGMENT_NEG_RECEIVE|MQMSG_ACKNOWLEDGMENT_POS_RECEIVE))

#define MQMSG_DELIVERY_EXPRESS  0
#define MQMSG_DELIVERY_RECOVERABLE  1

#define MQMSG_JOURNAL_NONE  0
#define MQMSG_DEADLETTER  1
#define MQMSG_JOURNAL  2

#define MQMSG_TRACE_NONE  0
#define MQMSG_SEND_ROUTE_TO_REPORT_QUEUE  1

#define MQMSG_SENDERID_TYPE_NONE  0
#define MQMSG_SENDERID_TYPE_SID  1

#define MQMSG_PRIV_LEVEL_NONE  0
#define MQMSG_PRIV_LEVEL_BODY  0x01
#define MQMSG_PRIV_LEVEL_BODY_BASE  0x01
#define MQMSG_PRIV_LEVEL_BODY_ENHANCED  0x03

#define MQMSG_AUTH_LEVEL_NONE  0
#define MQMSG_AUTH_LEVEL_ALWAYS  1
#define MQMSG_AUTH_LEVEL_MSMQ10  2
#define MQMSG_AUTH_LEVEL_MSMQ20  4

#define MQMSG_AUTHENTICATION_NOT_REQUESTED  0
#define MQMSG_AUTHENTICATION_REQUESTED  1
#define MQMSG_AUTHENTICATION_REQUESTED_EX  3

#define MQMSG_NOT_FIRST_IN_XACT  0
#define MQMSG_FIRST_IN_XACT  1

#define MQMSG_NOT_LAST_IN_XACT  0
#define MQMSG_LAST_IN_XACT  1

#define PROPID_Q_BASE  100
#define PROPID_Q_INSTANCE  (PROPID_Q_BASE+1)
#define PROPID_Q_TYPE  (PROPID_Q_BASE+2)
#define PROPID_Q_PATHNAME  (PROPID_Q_BASE+3)
#define PROPID_Q_JOURNAL  (PROPID_Q_BASE+4)
#define PROPID_Q_QUOTA  (PROPID_Q_BASE+5)
#define PROPID_Q_BASEPRIORITY  (PROPID_Q_BASE+6)
#define PROPID_Q_JOURNAL_QUOTA  (PROPID_Q_BASE+7)
#define PROPID_Q_LABEL  (PROPID_Q_BASE+8)
#define PROPID_Q_CREATE_TIME  (PROPID_Q_BASE+9)
#define PROPID_Q_MODIFY_TIME  (PROPID_Q_BASE+10)
#define PROPID_Q_AUTHENTICATE  (PROPID_Q_BASE+11)
#define PROPID_Q_PRIV_LEVEL  (PROPID_Q_BASE+12)
#define PROPID_Q_TRANSACTION  (PROPID_Q_BASE+13)
#define PROPID_Q_PATHNAME_DNS  (PROPID_Q_BASE+24)

#define MQ_JOURNAL_NONE  (unsigned char)0
#define MQ_JOURNAL  (unsigned char)1

#define MQ_QTYPE_REPORT {0x55ee8f32,0xcce9,0x11cf,{0xb1,0x8,0x0,0x20,0xaf,0xd6,0x1c,0xe9}}
#define MQ_QTYPE_TEST  {0x55ee8f33,0xcce9,0x11cf,{0xb1,0x8,0x0,0x20,0xaf,0xd6,0x1c,0xe9}}

#define MQ_TRANSACTIONAL_NONE  (unsigned char)0
#define MQ_TRANSACTIONAL  (unsigned char)1

#define MQ_AUTHENTICATE_NONE  (unsigned char)0
#define MQ_AUTHENTICATE  (unsigned char)1

#define MQ_PRIV_LEVEL_NONE  (unsigned long)0
#define MQ_PRIV_LEVEL_OPTIONAL  (unsigned long)1
#define MQ_PRIV_LEVEL_BODY  (unsigned long)2

#define PROPID_QM_BASE 200

#define PROPID_QM_SITE_ID  (PROPID_QM_BASE+1)
#define PROPID_QM_MACHINE_ID  (PROPID_QM_BASE+2)
#define PROPID_QM_PATHNAME  (PROPID_QM_BASE+3)
#define PROPID_QM_CONNECTION  (PROPID_QM_BASE+4)
#define PROPID_QM_ENCRYPTION_PK  (PROPID_QM_BASE+5)
#define PROPID_QM_ENCRYPTION_PK_BASE  (PROPID_QM_BASE+31)
#define PROPID_QM_ENCRYPTION_PK_ENHANCED  (PROPID_QM_BASE+32)
#define PROPID_QM_PATHNAME_DNS  (PROPID_QM_BASE+33)

#define LONG_LIVED  0xfffffffe

#define PROPID_PC_BASE 5800

#define PROPID_PC_VERSION  (PROPID_PC_BASE+1)
#define PROPID_PC_DS_ENABLED  (PROPID_PC_BASE+2)

#define MQ_OK  0L

#ifndef FACILITY_MSMQ
#define FACILITY_MSMQ  0x0E
#endif

#define MQ_ERROR  0xC00E0001L
#define MQ_ERROR_PROPERTY  0xC00E0002L
#define MQ_ERROR_QUEUE_NOT_FOUND  0xC00E0003L
#define MQ_ERROR_QUEUE_EXISTS  0xC00E0005L
#define MQ_ERROR_INVALID_PARAMETER  0xC00E0006L
#define MQ_ERROR_INVALID_HANDLE  0xC00E0007L
#define MQ_ERROR_OPERATION_CANCELLED  0xC00E0008L
#define MQ_ERROR_SHARING_VIOLATION  0xC00E0009L
#define MQ_ERROR_SERVICE_NOT_AVAILABLE  0xC00E000BL
#define MQ_ERROR_MACHINE_NOT_FOUND  0xC00E000DL
#define MQ_ERROR_ILLEGAL_SORT  0xC00E0010L
#define MQ_ERROR_ILLEGAL_USER  0xC00E0011L
#define MQ_ERROR_NO_DS  0xC00E0013L
#define MQ_ERROR_ILLEGAL_QUEUE_PATHNAME  0xC00E0014L
#define MQ_ERROR_ILLEGAL_PROPERTY_VALUE  0xC00E0018L
#define MQ_ERROR_ILLEGAL_PROPERTY_VT  0xC00E0019L
#define MQ_ERROR_BUFFER_OVERFLOW  0xC00E001AL
#define MQ_ERROR_IO_TIMEOUT  0xC00E001BL
#define MQ_ERROR_ILLEGAL_CURSOR_ACTION  0xC00E001CL
#define MQ_ERROR_MESSAGE_ALREADY_RECEIVED 0xC00E001DL
#define MQ_ERROR_ILLEGAL_FORMATNAME  0xC00E001EL
#define MQ_ERROR_FORMATNAME_BUFFER_TOO_SMALL 0xC00E001FL
#define MQ_ERROR_UNSUPPORTED_FORMATNAME_OPERATION 0xC00E0020L
#define MQ_ERROR_ILLEGAL_SECURITY_DESCRIPTOR 0xC00E0021L
#define MQ_ERROR_SENDERID_BUFFER_TOO_SMALL 0xC00E0022L
#define MQ_ERROR_SECURITY_DESCRIPTOR_TOO_SMALL 0xC00E0023L
#define MQ_ERROR_CANNOT_IMPERSONATE_CLIENT 0xC00E0024L
#define MQ_ERROR_ACCESS_DENIED  0xC00E0025L
#define MQ_ERROR_PRIVILEGE_NOT_HELD  0xC00E0026L
#define MQ_ERROR_INSUFFICIENT_RESOURCES  0xC00E0027L
#define MQ_ERROR_USER_BUFFER_TOO_SMALL  0xC00E0028L
#define MQ_ERROR_MESSAGE_STORAGE_FAILED  0xC00E002AL
#define MQ_ERROR_SENDER_CERT_BUFFER_TOO_SMALL 0xC00E002BL
#define MQ_ERROR_INVALID_CERTIFICATE  0xC00E002CL
#define MQ_ERROR_CORRUPTED_INTERNAL_CERTIFICATE 0xC00E002DL
#define MQ_ERROR_NO_INTERNAL_USER_CERT  0xC00E002FL
#define MQ_ERROR_CORRUPTED_SECURITY_DATA 0xC00E0030L
#define MQ_ERROR_CORRUPTED_PERSONAL_CERT_STORE 0xC00E0031L
#define MQ_ERROR_COMPUTER_DOES_NOT_SUPPORT_ENCRYPTION 0xC00E0033L
#define MQ_ERROR_BAD_SECURITY_CONTEXT  0xC00E0035L
#define MQ_ERROR_COULD_NOT_GET_USER_SID  0xC00E0036L
#define MQ_ERROR_COULD_NOT_GET_ACCOUNT_INFO 0xC00E0037L
#define MQ_ERROR_ILLEGAL_MQCOLUMNS  0xC00E0038L
#define MQ_ERROR_ILLEGAL_PROPID  0xC00E0039L
#define MQ_ERROR_ILLEGAL_RELATION  0xC00E003AL
#define MQ_ERROR_ILLEGAL_PROPERTY_SIZE  0xC00E003BL
#define MQ_ERROR_ILLEGAL_RESTRICTION_PROPID 0xC00E003CL
#define MQ_ERROR_ILLEGAL_MQQUEUEPROPS  0xC00E003DL
#define MQ_ERROR_PROPERTY_NOTALLOWED  0xC00E003EL
#define MQ_ERROR_INSUFFICIENT_PROPERTIES 0xC00E003FL
#define MQ_ERROR_MACHINE_EXISTS  0xC00E0040L
#define MQ_ERROR_ILLEGAL_MQQMPROPS  0xC00E0041L
#define MQ_ERROR_DS_IS_FULL  0xC00E0042L
#define MQ_ERROR_DS_ERROR  0xC00E0043L
#define MQ_ERROR_INVALID_OWNER  0xC00E0044L
#define MQ_ERROR_UNSUPPORTED_ACCESS_MODE 0xC00E0045L
#define MQ_ERROR_RESULT_BUFFER_TOO_SMALL 0xC00E0046L
#define MQ_ERROR_DELETE_CN_IN_USE  0xC00E0048L
#define MQ_ERROR_NO_RESPONSE_FROM_OBJECT_SERVER 0xC00E0049L
#define MQ_ERROR_OBJECT_SERVER_NOT_AVAILABLE 0xC00E004AL
#define MQ_ERROR_QUEUE_NOT_AVAILABLE  0xC00E004BL
#define MQ_ERROR_DTC_CONNECT  0xC00E004CL
#define MQ_ERROR_TRANSACTION_IMPORT  0xC00E004EL
#define MQ_ERROR_TRANSACTION_USAGE  0xC00E0050L
#define MQ_ERROR_TRANSACTION_SEQUENCE  0xC00E0051L
#define MQ_ERROR_MISSING_CONNECTOR_TYPE  0xC00E0055L
#define MQ_ERROR_STALE_HANDLE  0xC00E0056L
#define MQ_ERROR_TRANSACTION_ENLIST  0xC00E0058L
#define MQ_ERROR_QUEUE_DELETED  0xC00E005AL
#define MQ_ERROR_ILLEGAL_CONTEXT  0xC00E005BL
#define MQ_ERROR_ILLEGAL_SORT_PROPID  0xC00E005CL
#define MQ_ERROR_LABEL_TOO_LONG  0xC00E005DL
#define MQ_ERROR_LABEL_BUFFER_TOO_SMALL  0xC00E005EL
#define MQ_ERROR_MQIS_SERVER_EMPTY  0xC00E005FL
#define MQ_ERROR_MQIS_READONLY_MODE  0xC00E0060L
#define MQ_ERROR_SYMM_KEY_BUFFER_TOO_SMALL 0xC00E0061L
#define MQ_ERROR_SIGNATURE_BUFFER_TOO_SMALL 0xC00E0062L
#define MQ_ERROR_PROV_NAME_BUFFER_TOO_SMALL 0xC00E0063L
#define MQ_ERROR_ILLEGAL_OPERATION  0xC00E0064L
#define MQ_ERROR_WRITE_NOT_ALLOWED  0xC00E0065L
#define MQ_ERROR_WKS_CANT_SERVE_CLIENT  0xC00E0066L
#define MQ_ERROR_DEPEND_WKS_LICENSE_OVERFLOW 0xC00E0067L
#define MQ_CORRUPTED_QUEUE_WAS_DELETED  0xC00E0068L
#define MQ_ERROR_REMOTE_MACHINE_NOT_AVAILABLE 0xC00E0069L
#define MQ_ERROR_UNSUPPORTED_OPERATION  0xC00E006AL
#define MQ_ERROR_ENCRYPTION_PROVIDER_NOT_SUPPORTED 0xC00E006BL
#define MQ_ERROR_CANNOT_SET_CRYPTO_SEC_DESCR 0xC00E006CL
#define MQ_ERROR_CERTIFICATE_NOT_PROVIDED 0xC00E006DL
#define MQ_ERROR_Q_DNS_PROPERTY_NOT_SUPPORTED 0xC00E006EL
#define MQ_ERROR_CANNOT_CREATE_CERT_STORE 0xC00E006FL
#define MQ_ERROR_CANNOT_OPEN_CERT_STORE  0xC00E0070L
#define MQ_ERROR_ILLEGAL_ENTERPRISE_OPERATION 0xC00E0071L
#define MQ_ERROR_CANNOT_GRANT_ADD_GUID  0xC00E0072L
#define MQ_ERROR_CANNOT_LOAD_MSMQOCM  0xC00E0073L
#define MQ_ERROR_NO_ENTRY_POINT_MSMQOCM  0xC00E0074L
#define MQ_ERROR_NO_MSMQ_SERVERS_ON_DC  0xC00E0075L
#define MQ_ERROR_CANNOT_JOIN_DOMAIN  0xC00E0076L
#define MQ_ERROR_CANNOT_CREATE_ON_GC  0xC00E0077L
#define MQ_ERROR_GUID_NOT_MATCHING  0xC00E0078L
#define MQ_ERROR_PUBLIC_KEY_NOT_FOUND  0xC00E0079L
#define MQ_ERROR_PUBLIC_KEY_DOES_NOT_EXIST 0xC00E007AL
#define MQ_ERROR_ILLEGAL_MQPRIVATEPROPS  0xC00E007BL
#define MQ_ERROR_NO_GC_IN_DOMAIN  0xC00E007CL
#define MQ_ERROR_NO_MSMQ_SERVERS_ON_GC  0xC00E007DL
#define MQ_ERROR_CANNOT_GET_DN  0xC00E007EL
#define MQ_ERROR_CANNOT_HASH_DATA_EX  0xC00E007FL
#define MQ_ERROR_CANNOT_SIGN_DATA_EX  0xC00E0080L
#define MQ_ERROR_CANNOT_CREATE_HASH_EX  0xC00E0081L
#define MQ_ERROR_FAIL_VERIFY_SIGNATURE_EX 0xC00E0082L

#define MQ_INFORMATION_PROPERTY  0x400E0001L
#define MQ_INFORMATION_ILLEGAL_PROPERTY  0x400E0002L
#define MQ_INFORMATION_PROPERTY_IGNORED  0x400E0003L
#define MQ_INFORMATION_UNSUPPORTED_PROPERTY 0x400E0004L
#define MQ_INFORMATION_DUPLICATE_PROPERTY 0x400E0005L
#define MQ_INFORMATION_OPERATION_PENDING 0x400E0006L
#define MQ_INFORMATION_FORMATNAME_BUFFER_TOO_SMALL 0x400E0009L
#define MQ_INFORMATION_INTERNAL_USER_CERT_EXIST 0x400E000AL
#define MQ_INFORMATION_OWNER_IGNORED  0x400E000BL

#define MQCERT_REGISTER_ALWAYS  0x01
#define MQCERT_REGISTER_IF_NOT_EXIST  0x02

#define MQSEC_DELETE_MESSAGE  0x1
#define MQSEC_PEEK_MESSAGE  0x2
#define MQSEC_WRITE_MESSAGE  0x4
#define MQSEC_DELETE_JOURNAL_MESSAGE  0x8
#define MQSEC_SET_QUEUE_PROPERTIES  0x10
#define MQSEC_GET_QUEUE_PROPERTIES  0x20
#define MQSEC_DELETE_QUEUE  DELETE
#define MQSEC_GET_QUEUE_PERMISSIONS  READ_CONTROL
#define MQSEC_CHANGE_QUEUE_PERMISSIONS  WRITE_DAC
#define MQSEC_TAKE_QUEUE_OWNERSHIP  WRITE_OWNER

#define MQSEC_RECEIVE_MESSAGE  (MQSEC_DELETE_MESSAGE|MQSEC_PEEK_MESSAGE)
#define MQSEC_RECEIVE_JOURNAL_MESSAGE  (MQSEC_DELETE_JOURNAL_MESSAGE|MQSEC_PEEK_MESSAGE)
#define MQSEC_QUEUE_GENERIC_READ  (MQSEC_GET_QUEUE_PROPERTIES|MQSEC_GET_QUEUE_PERMISSIONS|MQSEC_RECEIVE_MESSAGE|MQSEC_RECEIVE_JOURNAL_MESSAGE)
#define MQSEC_QUEUE_GENERIC_WRITE  (MQSEC_GET_QUEUE_PROPERTIES|MQSEC_GET_QUEUE_PERMISSIONS|MQSEC_WRITE_MESSAGE)
#define MQSEC_QUEUE_GENERIC_EXECUTE  0
#define MQSEC_QUEUE_GENERIC_ALL  (MQSEC_RECEIVE_MESSAGE|MQSEC_RECEIVE_JOURNAL_MESSAGE|MQSEC_WRITE_MESSAGE|MQSEC_SET_QUEUE_PROPERTIES|MQSEC_GET_QUEUE_PROPERTIES|MQSEC_DELETE_QUEUE|MQSEC_GET_QUEUE_PERMISSIONS|MQSEC_CHANGE_QUEUE_PERMISSIONS|MQSEC_TAKE_QUEUE_OWNERSHIP)

typedef struct tagPROPVARIANT tagMQPROPVARIANT;
typedef tagMQPROPVARIANT MQPROPVARIANT;

typedef struct tagMQPROPERTYRESTRICTION {
    ULONG rel;
    PROPID prop;
    MQPROPVARIANT prval;
} MQPROPERTYRESTRICTION;

typedef struct tagMQRESTRICTION {
    ULONG cRes;
    MQPROPERTYRESTRICTION *paPropRes;
} MQRESTRICTION;

typedef struct tagMQCOLUMNSET {
    ULONG cCol;
    PROPID *aCol;
} MQCOLUMNSET;

typedef struct tagMQSORTKEY {
    PROPID propColumn;
    ULONG dwOrder;
} MQSORTKEY;

typedef struct tagMQSORTSET {
    ULONG cCol;
    MQSORTKEY *aCol;
} MQSORTSET;

typedef HANDLE QUEUEHANDLE;

typedef PROPID MSGPROPID;
typedef PROPID QUEUEPROPID;
typedef PROPID QMPROPID;

typedef struct tagMQMSGPROPS {
    DWORD cProp;
    MSGPROPID *aPropID;
    MQPROPVARIANT *aPropVar;
    HRESULT *aStatus;
} MQMSGPROPS;

typedef struct tagMQQUEUEPROPS {
    DWORD cProp;
    QUEUEPROPID *aPropID;
    MQPROPVARIANT *aPropVar;
    HRESULT *aStatus;
} MQQUEUEPROPS;

typedef struct tagMQQMPROPS {
    DWORD cProp;
    QMPROPID *aPropID;
    MQPROPVARIANT *aPropVar;
    HRESULT *aStatus;
} MQQMPROPS;

typedef struct tagMQPRIVATEPROPS {
    DWORD cProp;
    QMPROPID *aPropID;
    MQPROPVARIANT *aPropVar;
    HRESULT *aStatus;
} MQPRIVATEPROPS;

#ifdef __cplusplus
extern "C" {
#endif

typedef VOID (APIENTRY *PMQRECEIVECALLBACK)(HRESULT,QUEUEHANDLE,DWORD,DWORD,MQMSGPROPS*,LPOVERLAPPED,HANDLE);
typedef HRESULT (APIENTRY *MQRegisterCertificate_ROUTINE)(DWORD,PVOID,DWORD);

HRESULT APIENTRY MQCreateQueue(PSECURITY_DESCRIPTOR,MQQUEUEPROPS*,LPWSTR,LPDWORD);
HRESULT APIENTRY MQDeleteQueue(LPCWSTR);
HRESULT APIENTRY MQLocateBegin(LPCWSTR,MQRESTRICTION*,MQCOLUMNSET*,MQSORTSET*,PHANDLE);
HRESULT APIENTRY MQLocateNext(HANDLE,DWORD*,MQPROPVARIANT []);
HRESULT APIENTRY MQLocateEnd(HANDLE);
HRESULT APIENTRY MQOpenQueue(LPCWSTR,DWORD,DWORD,QUEUEHANDLE*);
HRESULT APIENTRY MQSendMessage(QUEUEHANDLE,MQMSGPROPS*,ITransaction*);
HRESULT APIENTRY MQReceiveMessage(QUEUEHANDLE,DWORD,DWORD,MQMSGPROPS*,LPOVERLAPPED,PMQRECEIVECALLBACK,HANDLE,ITransaction*);
HRESULT APIENTRY MQCreateCursor(QUEUEHANDLE,PHANDLE);
HRESULT APIENTRY MQCloseCursor(HANDLE);
HRESULT APIENTRY MQCloseQueue(HANDLE);
HRESULT APIENTRY MQSetQueueProperties(LPCWSTR,MQQUEUEPROPS*);
HRESULT APIENTRY MQGetQueueProperties(LPCWSTR,MQQUEUEPROPS*);
HRESULT APIENTRY MQGetQueueSecurity(LPCWSTR,SECURITY_INFORMATION,PSECURITY_DESCRIPTOR,DWORD,LPDWORD);
HRESULT APIENTRY MQSetQueueSecurity(LPCWSTR,SECURITY_INFORMATION,PSECURITY_DESCRIPTOR);
HRESULT APIENTRY MQPathNameToFormatName(LPCWSTR,LPWSTR,LPDWORD);
HRESULT APIENTRY MQHandleToFormatName(QUEUEHANDLE,LPWSTR,LPDWORD);
HRESULT APIENTRY MQInstanceToFormatName(GUID*,LPWSTR,LPDWORD);
void APIENTRY MQFreeMemory(PVOID);
HRESULT APIENTRY MQGetMachineProperties(LPCWSTR,const GUID*,MQQMPROPS*);
HRESULT APIENTRY MQGetSecurityContext(PVOID,DWORD,HANDLE*);
void APIENTRY MQFreeSecurityContext(HANDLE);
HRESULT APIENTRY MQRegisterCertificate(DWORD,PVOID,DWORD);
HRESULT APIENTRY MQBeginTransaction(ITransaction**);
HRESULT APIENTRY MQGetOverlappedResult(LPOVERLAPPED);
HRESULT APIENTRY MQGetPrivateComputerInformation(LPCWSTR,MQPRIVATEPROPS*);

#ifdef __cplusplus
}
#endif

#endif /* _MQ_H */
