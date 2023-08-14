/* This file is auto-generated from x86.dat by x86.pl - don't edit it */

/* This file is included by asm.h */

/* Instruction names */
enum {
	I_AAA,
	I_AAD,
	I_AAM,
	I_AAS,
	I_ADC,
	I_ADD,
	I_ADDPD,
	I_ADDPS,
	I_ADDSD,
	I_ADDSS,
	I_AND,
	I_ANDNPD,
	I_ANDNPS,
	I_ANDPD,
	I_ANDPS,
	I_ARPL,
	I_BOUND,
	I_BSF,
	I_BSR,
	I_BSWAP,
	I_BT,
	I_BTC,
	I_BTR,
	I_BTS,
	I_CALL,
	I_CBW,
	I_CDQ,
	I_CLC,
	I_CLD,
	I_CLFLUSH,
	I_CLI,
	I_CLTS,
	I_CMC,
	I_CMP,
	I_CMPEQPD,
	I_CMPEQPS,
	I_CMPEQSD,
	I_CMPEQSS,
	I_CMPLEPD,
	I_CMPLEPS,
	I_CMPLESD,
	I_CMPLESS,
	I_CMPLTPD,
	I_CMPLTPS,
	I_CMPLTSD,
	I_CMPLTSS,
	I_CMPNEQPD,
	I_CMPNEQPS,
	I_CMPNEQSD,
	I_CMPNEQSS,
	I_CMPNLEPD,
	I_CMPNLEPS,
	I_CMPNLESD,
	I_CMPNLESS,
	I_CMPNLTPD,
	I_CMPNLTPS,
	I_CMPNLTSD,
	I_CMPNLTSS,
	I_CMPORDPD,
	I_CMPORDPS,
	I_CMPORDSD,
	I_CMPORDSS,
	I_CMPPD,
	I_CMPPS,
	I_CMPSB,
	I_CMPSD,
	I_CMPSS,
	I_CMPSW,
	I_CMPUNORDPD,
	I_CMPUNORDPS,
	I_CMPUNORDSD,
	I_CMPUNORDSS,
	I_CMPXCHG,
	I_CMPXCHG486,
	I_CMPXCHG8B,
	I_COMISD,
	I_COMISS,
	I_CPUID,
	I_CVTDQ2PD,
	I_CVTDQ2PS,
	I_CVTPD2DQ,
	I_CVTPD2PI,
	I_CVTPD2PS,
	I_CVTPI2PD,
	I_CVTPI2PS,
	I_CVTPS2DQ,
	I_CVTPS2PD,
	I_CVTPS2PI,
	I_CVTSD2SI,
	I_CVTSD2SS,
	I_CVTSI2SD,
	I_CVTSI2SS,
	I_CVTSS2SD,
	I_CVTSS2SI,
	I_CVTTPD2DQ,
	I_CVTTPD2PI,
	I_CVTTPS2DQ,
	I_CVTTPS2PI,
	I_CVTTSD2SI,
	I_CVTTSS2SI,
	I_CWD,
	I_CWDE,
	I_DAA,
	I_DAS,
	I_DB,
	I_DD,
	I_DEC,
	I_DIV,
	I_DIVPD,
	I_DIVPS,
	I_DIVSD,
	I_DIVSS,
	I_DQ,
	I_DT,
	I_DW,
	I_EMMS,
	I_ENTER,
	I_EQU,
	I_F2XM1,
	I_FABS,
	I_FADD,
	I_FADDP,
	I_FBLD,
	I_FBSTP,
	I_FCHS,
	I_FCLEX,
	I_FCMOVB,
	I_FCMOVBE,
	I_FCMOVE,
	I_FCMOVNB,
	I_FCMOVNBE,
	I_FCMOVNE,
	I_FCMOVNU,
	I_FCMOVU,
	I_FCOM,
	I_FCOMI,
	I_FCOMIP,
	I_FCOMP,
	I_FCOMPP,
	I_FCOS,
	I_FDECSTP,
	I_FDISI,
	I_FDIV,
	I_FDIVP,
	I_FDIVR,
	I_FDIVRP,
	I_FEMMS,
	I_FENI,
	I_FFREE,
	I_FFREEP,
	I_FIADD,
	I_FICOM,
	I_FICOMP,
	I_FIDIV,
	I_FIDIVR,
	I_FILD,
	I_FIMUL,
	I_FINCSTP,
	I_FINIT,
	I_FIST,
	I_FISTP,
	I_FISUB,
	I_FISUBR,
	I_FLD,
	I_FLD1,
	I_FLDCW,
	I_FLDENV,
	I_FLDL2E,
	I_FLDL2T,
	I_FLDLG2,
	I_FLDLN2,
	I_FLDPI,
	I_FLDZ,
	I_FMUL,
	I_FMULP,
	I_FNCLEX,
	I_FNDISI,
	I_FNENI,
	I_FNINIT,
	I_FNOP,
	I_FNSAVE,
	I_FNSTCW,
	I_FNSTENV,
	I_FNSTSW,
	I_FPATAN,
	I_FPREM,
	I_FPREM1,
	I_FPTAN,
	I_FRNDINT,
	I_FRSTOR,
	I_FSAVE,
	I_FSCALE,
	I_FSETPM,
	I_FSIN,
	I_FSINCOS,
	I_FSQRT,
	I_FST,
	I_FSTCW,
	I_FSTENV,
	I_FSTP,
	I_FSTSW,
	I_FSUB,
	I_FSUBP,
	I_FSUBR,
	I_FSUBRP,
	I_FTST,
	I_FUCOM,
	I_FUCOMI,
	I_FUCOMIP,
	I_FUCOMP,
	I_FUCOMPP,
	I_FWAIT,
	I_FXAM,
	I_FXCH,
	I_FXRSTOR,
	I_FXSAVE,
	I_FXTRACT,
	I_FYL2X,
	I_FYL2XP1,
	I_HLT,
	I_IBTS,
	I_ICEBP,
	I_IDIV,
	I_IMUL,
	I_IN,
	I_INC,
	I_INSB,
	I_INSD,
	I_INSW,
	I_INT,
	I_INT01,
	I_INT03,
	I_INT1,
	I_INT3,
	I_INTO,
	I_INVD,
	I_INVLPG,
	I_IRET,
	I_IRETD,
	I_IRETW,
	I_JCXZ,
	I_JECXZ,
	I_JMP,
	I_LAHF,
	I_LAR,
	I_LDMXCSR,
	I_LDS,
	I_LEA,
	I_LEAVE,
	I_LES,
	I_LFENCE,
	I_LFS,
	I_LGDT,
	I_LGS,
	I_LIDT,
	I_LLDT,
	I_LMSW,
	I_LOADALL,
	I_LOADALL286,
	I_LODSB,
	I_LODSD,
	I_LODSW,
	I_LOOP,
	I_LOOPE,
	I_LOOPNE,
	I_LOOPNZ,
	I_LOOPZ,
	I_LSL,
	I_LSS,
	I_LTR,
	I_MASKMOVDQU,
	I_MASKMOVQ,
	I_MAXPD,
	I_MAXPS,
	I_MAXSD,
	I_MAXSS,
	I_MFENCE,
	I_MINPD,
	I_MINPS,
	I_MINSD,
	I_MINSS,
	I_MOV,
	I_MOVAPD,
	I_MOVAPS,
	I_MOVD,
	I_MOVDQ2Q,
	I_MOVDQA,
	I_MOVDQU,
	I_MOVHLPS,
	I_MOVHPD,
	I_MOVHPS,
	I_MOVLHPS,
	I_MOVLPD,
	I_MOVLPS,
	I_MOVMSKPD,
	I_MOVMSKPS,
	I_MOVNTDQ,
	I_MOVNTI,
	I_MOVNTPD,
	I_MOVNTPS,
	I_MOVNTQ,
	I_MOVQ,
	I_MOVQ2DQ,
	I_MOVSB,
	I_MOVSD,
	I_MOVSS,
	I_MOVSW,
	I_MOVSX,
	I_MOVUPD,
	I_MOVUPS,
	I_MOVZX,
	I_MUL,
	I_MULPD,
	I_MULPS,
	I_MULSD,
	I_MULSS,
	I_NEG,
	I_NOP,
	I_NOT,
	I_OR,
	I_ORPD,
	I_ORPS,
	I_OUT,
	I_OUTSB,
	I_OUTSD,
	I_OUTSW,
	I_PACKSSDW,
	I_PACKSSWB,
	I_PACKUSWB,
	I_PADDB,
	I_PADDD,
	I_PADDQ,
	I_PADDSB,
	I_PADDSIW,
	I_PADDSW,
	I_PADDUSB,
	I_PADDUSW,
	I_PADDW,
	I_PAND,
	I_PANDN,
	I_PAUSE,
	I_PAVEB,
	I_PAVGB,
	I_PAVGUSB,
	I_PAVGW,
	I_PCMPEQB,
	I_PCMPEQD,
	I_PCMPEQW,
	I_PCMPGTB,
	I_PCMPGTD,
	I_PCMPGTW,
	I_PDISTIB,
	I_PEXTRW,
	I_PF2ID,
	I_PF2IW,
	I_PFACC,
	I_PFADD,
	I_PFCMPEQ,
	I_PFCMPGE,
	I_PFCMPGT,
	I_PFMAX,
	I_PFMIN,
	I_PFMUL,
	I_PFNACC,
	I_PFPNACC,
	I_PFRCP,
	I_PFRCPIT1,
	I_PFRCPIT2,
	I_PFRSQIT1,
	I_PFRSQRT,
	I_PFSUB,
	I_PFSUBR,
	I_PI2FD,
	I_PI2FW,
	I_PINSRW,
	I_PMACHRIW,
	I_PMADDWD,
	I_PMAGW,
	I_PMAXSW,
	I_PMAXUB,
	I_PMINSW,
	I_PMINUB,
	I_PMOVMSKB,
	I_PMULHRIW,
	I_PMULHRWA,
	I_PMULHRWC,
	I_PMULHUW,
	I_PMULHW,
	I_PMULLW,
	I_PMULUDQ,
	I_PMVGEZB,
	I_PMVLZB,
	I_PMVNZB,
	I_PMVZB,
	I_POP,
	I_POPA,
	I_POPAD,
	I_POPAW,
	I_POPF,
	I_POPFD,
	I_POPFW,
	I_POR,
	I_PREFETCH,
	I_PREFETCHNTA,
	I_PREFETCHT0,
	I_PREFETCHT1,
	I_PREFETCHT2,
	I_PREFETCHW,
	I_PSADBW,
	I_PSHUFD,
	I_PSHUFHW,
	I_PSHUFLW,
	I_PSHUFW,
	I_PSLLD,
	I_PSLLDQ,
	I_PSLLQ,
	I_PSLLW,
	I_PSRAD,
	I_PSRAW,
	I_PSRLD,
	I_PSRLDQ,
	I_PSRLQ,
	I_PSRLW,
	I_PSUBB,
	I_PSUBD,
	I_PSUBQ,
	I_PSUBSB,
	I_PSUBSIW,
	I_PSUBSW,
	I_PSUBUSB,
	I_PSUBUSW,
	I_PSUBW,
	I_PSWAPD,
	I_PUNPCKHBW,
	I_PUNPCKHDQ,
	I_PUNPCKHQDQ,
	I_PUNPCKHWD,
	I_PUNPCKLBW,
	I_PUNPCKLDQ,
	I_PUNPCKLQDQ,
	I_PUNPCKLWD,
	I_PUSH,
	I_PUSHA,
	I_PUSHAD,
	I_PUSHAW,
	I_PUSHF,
	I_PUSHFD,
	I_PUSHFW,
	I_PXOR,
	I_RCL,
	I_RCPPS,
	I_RCPSS,
	I_RCR,
	I_RDMSR,
	I_RDPMC,
	I_RDSHR,
	I_RDTSC,
	I_RESB,
	I_RESD,
	I_RESQ,
	I_REST,
	I_RESW,
	I_RET,
	I_RETF,
	I_RETN,
	I_ROL,
	I_ROR,
	I_RSDC,
	I_RSLDT,
	I_RSM,
	I_RSQRTPS,
	I_RSQRTSS,
	I_RSTS,
	I_SAHF,
	I_SAL,
	I_SALC,
	I_SAR,
	I_SBB,
	I_SCASB,
	I_SCASD,
	I_SCASW,
	I_SFENCE,
	I_SGDT,
	I_SHL,
	I_SHLD,
	I_SHR,
	I_SHRD,
	I_SHUFPD,
	I_SHUFPS,
	I_SIDT,
	I_SLDT,
	I_SMI,
	I_SMINT,
	I_SMINTOLD,
	I_SMSW,
	I_SQRTPD,
	I_SQRTPS,
	I_SQRTSD,
	I_SQRTSS,
	I_STC,
	I_STD,
	I_STI,
	I_STMXCSR,
	I_STOSB,
	I_STOSD,
	I_STOSW,
	I_STR,
	I_SUB,
	I_SUBPD,
	I_SUBPS,
	I_SUBSD,
	I_SUBSS,
	I_SVDC,
	I_SVLDT,
	I_SVTS,
	I_SYSCALL,
	I_SYSENTER,
	I_SYSEXIT,
	I_SYSRET,
	I_TEST,
	I_UCOMISD,
	I_UCOMISS,
	I_UD0,
	I_UD1,
	I_UD2,
	I_UMOV,
	I_UNPCKHPD,
	I_UNPCKHPS,
	I_UNPCKLPD,
	I_UNPCKLPS,
	I_VERR,
	I_VERW,
	I_WAIT,
	I_WBINVD,
	I_WRMSR,
	I_WRSHR,
	I_XADD,
	I_XBTS,
	I_XCHG,
	I_XLAT,
	I_XLATB,
	I_XOR,
	I_XORPD,
	I_XORPS,
	I_XSTORE,
	I_CMOVcc,
	I_Jcc,
	I_SETcc
};

#define MAX_X86INSLEN 11
