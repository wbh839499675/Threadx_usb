;/**************************************************************************/
;/*                                                                        */
;/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
;/*                                                                        */
;/*       This software is licensed under the Microsoft Software License   */
;/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
;/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
;/*       and in the root directory of this software.                      */
;/*                                                                        */
;/**************************************************************************/
;
;
;/**************************************************************************/
;/**************************************************************************/
;/**                                                                       */ 
;/** ThreadX Component                                                     */ 
;/**                                                                       */
;/**   Thread                                                              */
;/**                                                                       */
;/**************************************************************************/
;/**************************************************************************/
;
;#define TX_SOURCE_CODE
;
;
;/* Include necessary system files.  */
;
;#include "tx_api.h"
;#include "tx_thread.h"
;
    section .text:CODE:ROOT
;/**************************************************************************/ 
;/*                                                                        */ 
;/*  FUNCTION                                               RELEASE        */ 
;/*                                                                        */ 
;/*    _tx_thread_interrupt_control                         RXv2/IAR       */
;/*                                                           6.1.9        */
;/*  AUTHOR                                                                */ 
;/*                                                                        */ 
;/*    William E. Lamie, Microsoft Corporation                             */
;/*                                                                        */ 
;/*  DESCRIPTION                                                           */ 
;/*                                                                        */ 
;/*    This function is responsible for changing the interrupt lockout     */ 
;/*    posture of the system.                                              */ 
;/*                                                                        */ 
;/*  INPUT                                                                 */ 
;/*                                                                        */ 
;/*    new_posture                           New interrupt lockout posture */ 
;/*                                                                        */ 
;/*  OUTPUT                                                                */ 
;/*                                                                        */ 
;/*    old_posture                           Old interrupt lockout posture */ 
;/*                                                                        */ 
;/*  CALLS                                                                 */ 
;/*                                                                        */ 
;/*    None                                                                */ 
;/*                                                                        */ 
;/*  CALLED BY                                                             */ 
;/*                                                                        */ 
;/*    Application Code                                                    */ 
;/*                                                                        */ 
;/*  RELEASE HISTORY                                                       */ 
;/*                                                                        */ 
;/*    DATE              NAME                      DESCRIPTION             */ 
;/*                                                                        */ 
;/*  12-30-2020     William E. Lamie         Initial Version 6.1.3         */
;/*  10-15-2021     William E. Lamie         Modified comment(s),          */
;/*                                            resulting in version 6.1.9  */
;/*                                                                        */ 
;/**************************************************************************/ 
;UINT   _tx_thread_interrupt_control(UINT new_posture)
;{
    public __tx_thread_interrupt_control
__tx_thread_interrupt_control:
;
;    /* Pickup current interrupt lockout posture.  */
;
    
    MVFC      PSW, R2           ; Save PSW to R2
    MOV.L     R2, R3            ; Make a copy of PSW in r3
    
;
;    /* Apply the new interrupt posture.  */
;
    
    BTST    #16, R1             ; Test I bit of PSW of "new posture"
    BMNE    #16, R2             ; Conditionally set I bit of intermediate posture
    
    MVTC    R2, PSW             ; Save intermediate posture to PSW
    
    MOV.L   R3,R1               ; Get original SR 
    RTS                         ; Return to caller
;}
    END
