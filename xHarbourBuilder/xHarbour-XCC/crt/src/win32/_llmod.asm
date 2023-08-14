;+--------------------------------------------------------------------------+
;|                                                                          |
;| File    : _llmod.asm                                                     |
;|                                                                          |
;| Purpose : long long integer remainder (signed).                          |
;|                                                                          |
;| History : Date      Reason                                               |
;|           00-09-15  Created                                              |
;|                                                                          |
;+--------------------------------------------------------------------------+

                BITS 32

                SECTION .text

                GLOBAL  ___llmod

;+--------------------------------------------------------------------------+

                FUNCTION ___llmod
___llmod:
                push    ebx
                push    edi

; Set up the local stack and save the index registers.

%define DVND esp+20     ; stack address of dividend (a)
%define DVSR esp+12     ; stack address of divisor (b)
%define ARGSZ 16

; Determine sign of the result (edi = 0 if result is positive, non-zero
; otherwise) and make operands positive.

                xor     edi,edi         ; result sign assumed positive

                mov     eax,[DVND+4]    ; hi word of a
                or      eax,eax         ; test to see if signed
                jge     short L1        ; skip rest if a is already positive
                inc     edi             ; complement result sign flag bit
                mov     edx,[DVND]      ; lo word of a
                neg     eax             ; make a positive
                neg     edx
                sbb     eax,0
                mov     [DVND+4],eax    ; save positive value
                mov     [DVND],edx
L1:
                mov     eax,[DVSR+4]    ; hi word of b
                or      eax,eax         ; test to see if signed
                jge     short L2        ; skip rest if b is already positive
                mov     edx,[DVSR]      ; lo word of b
                neg     eax             ; make b positive
                neg     edx
                sbb     eax,0
                mov     [DVSR+4],eax    ; save positive value
                mov     [DVSR],edx
L2:

;
; Now do the divide.  First look to see if the divisor is less than 4194304K.
; If so, then we can use a simple algorithm with word divides, otherwise
; things get a little more complex.
;
; NOTE - eax currently contains the high order word of DVSR
;

                or      eax,eax         ; check to see if divisor < 4194304K
                jnz     short L3        ; nope, gotta do this the hard way
                mov     ecx,[DVSR]      ; load divisor
                mov     eax,[DVND+4]    ; load high word of dividend
                xor     edx,edx
                div     ecx             ; edx <- remainder
                mov     eax,[DVND]      ; edx:eax <- remainder:lo word of dividend
                div     ecx             ; edx <- final remainder
                mov     eax,edx         ; edx:eax <- remainder
                xor     edx,edx
                dec     edi             ; check result sign flag
                jns     short L4        ; negate result, restore stack and return
                jmp     short L8        ; result sign ok, restore stack and return

;
; Here we do it the hard way.  Remember, eax contains the high word of DVSR
;

L3:
                mov     ebx,eax         ; ebx:ecx <- divisor
                mov     ecx,[DVSR]
                mov     edx,[DVND+4]    ; edx:eax <- dividend
                mov     eax,[DVND]
L5:
                shr     ebx,1           ; shift divisor right one bit
                rcr     ecx,1
                shr     edx,1           ; shift dividend right one bit
                rcr     eax,1
                or      ebx,ebx
                jnz     short L5        ; loop until divisor < 4194304K
                div     ecx             ; now divide, ignore remainder

;
; We may be off by one, so to check, we will multiply the quotient
; by the divisor and check the result against the orignal dividend
; Note that we must also check for overflow, which can occur if the
; dividend is close to 2**64 and the quotient is off by 1.
;

                mov     ecx,eax         ; save a copy of quotient in ECX
                mul     dword [DVSR+4]
                xchg    ecx,eax         ; save product, get quotient in EAX
                mul     dword [DVSR]
                add     edx,ecx         ; EDX:EAX = QUOT * DVSR
                jc      short L6        ; carry means Quotient is off by 1

;
; do long compare here between original dividend and the result of the
; multiply in edx:eax.  If original is larger or equal, we are ok, otherwise
; subtract the original divisor from the result.
;

                cmp     edx,[DVND+4]    ; compare hi words of result and original
                ja      short L6        ; if result > original, do subtract
                jb      short L7        ; if result < original, we are ok
                cmp     eax,[DVND]      ; hi words are equal, compare lo words
                jbe     short L7        ; if less or equal we are ok, else subtract
L6:
                sub     eax,[DVSR]      ; subtract divisor from result
                sbb     edx,[DVSR+4]
L7:

;
; Calculate remainder by subtracting the result from the original dividend.
; Since the result is already in a register, we will do the subtract in the
; opposite direction and negate the result if necessary.
;

                sub     eax,[DVND]      ; subtract dividend from result
                sbb     edx,[DVND+4]

;
; Now check the result sign flag to see if the result is supposed to be positive
; or negative.  It is currently negated (because we subtracted in the 'wrong'
; direction), so if the sign flag is set we are done, otherwise we must negate
; the result to make it positive again.
;

                dec     edi             ; check result sign flag
                jns     short L8        ; result is ok, restore stack and return
L4:
                neg     edx             ; otherwise, negate the result
                neg     eax
                sbb     edx,0

;
; Just the cleanup left to do.  edx:eax contains the quotient.
; Restore the saved registers and return.
;

L8:
                pop     edi
                pop     ebx
                ret     ARGSZ

                ALIGN   16

