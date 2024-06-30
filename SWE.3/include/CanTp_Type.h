
/*******************************************************************************
*   Project              : none                                               **
*   Peripheral           : none                                               **
*   Dependencies         : none                                               **
*                                                                             **
*   SW Version           : 1.0.0                                              **
*   (c) Copyright xxx                                                         **
*   All Rights Reserved.                                                      **
********************************************************************************/
/*=============================================================================*/
/**
*   @file     CanTp_Type.h
*   @brief    TODO
*   @details  TODO
*/
/*-----------------------------------------------------------------------------*
 * -REVISION HISTORY:
 *-----------------------------------------------------------------------------*/
/*  -VERSION-   -DATE-       -AUTHOR-    -COMMENT-
 *  V1.0.0      2021-12-07   LIANG.X     First Version
 *
 */
/*=============================================================================*/

#ifndef CANTP_TYPE_H
#define CANTP_TYPE_H

/*-----------------------------------------------------------------------------*
 * -INCLUDE FILES
 *-----------------------------------------------------------------------------*/
#include "EcoBase_Type.h"
#include "ComStack_Types.h"
/*-----------------------------------------------------------------------------*
 * -FILE VERSION
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -DEFINITION AND TYPEDEF
 *-----------------------------------------------------------------------------*/
#define CANTP_CODE
#define CANTP_CONST
#define CANTP_VAR

typedef u2 CanTp_SduIdType;

typedef enum
{
    CANTP_WAIT,
    CANTP_PROCESSING,
} CanTp_StateType;

typedef enum
{
  CANTP_STANDARD,
  CANTP_EXTENDED,
  CANTP_MIXED,
  CANTP_MIXED29BIT,
  CANTP_NORMALFIXED
}CanTp_AddrFormatType;

typedef enum
{
  CANTP_FUNCTIONAL,
  CANTP_PHYSICAL
}CanTp_TaType;

typedef enum
{
  CANTP_CAN_2_0,
  CANTP_CAN_SMALL_CAN_FD,
  CANTP_CAN_FD
}CanTp_CanType;

typedef enum
{
  CANTP_PADDING_OFF,
  CANTP_PADDING_ON
}CanTp_CanPaddingType;

typedef enum
{
  CANTP_OFF,
  CANTP_ON
}CanTp_RuntimeType;

typedef enum
{
  CANTP_SEND,
  CANTP_RECIVED
}CanTp_FrameDirType;

/**
  * Send Sdu Configuration Type
  */
typedef struct
{
  /**
    * N_As Max Timeout Value 
    */
  ECO_CONSTVAR( u4, CANTP_VAR ) m_Timeout_N_As;
  /**
    * N_Bs Max Timeout Value
    */
  ECO_CONSTVAR( u4, CANTP_VAR ) m_Timeout_N_Bs;
  /**
    * N_Cs Performace Value
    */
  ECO_CONSTVAR( u4, CANTP_VAR ) m_Timeout_N_Cs;
  /**
    * The Pointer of Source Address/Target Address/Address Extend
    */
  ECO_CONSTPVAR( u1, CANTP_CONST ) m_SaTaAePtr;
  /**
    * CAN Type
    */
  ECO_CONSTVAR( CanTp_CanType, CANTP_VAR ) m_CanType;
  /**
    * CAN Addressing Format Type
    */
  ECO_CONSTVAR( CanTp_AddrFormatType, CANTP_VAR ) m_AddrFormat;
  /**
    * Flag of CAN Padding or not
    */
  ECO_CONSTVAR( CanTp_CanPaddingType, CANTP_VAR ) m_IsPadding;
  /**
    * Target Address Type
    */
  ECO_CONSTVAR( CanTp_TaType, CANTP_VAR ) m_TaType;
  /**
    * Upper Layer Send Protocal Data Unit Identifier
    */
  ECO_CONSTVAR( PduIdType, CANTP_VAR ) m_TxPduId;
  /**
    * Low Layer Send Protocal Data Unit Identifier
    */
  ECO_CONSTVAR( PduIdType, CANTP_VAR ) m_TxCanIfId;
  /**
    * Reference Of Recived Service Data Unit Identifier
    */
  ECO_CONSTVAR( CanTp_SduIdType, CANTP_VAR ) m_RefRxSduId;

}CanTp_TxSduCfgType;

/**
  * Recived Sdu Configuration Type
  */
typedef struct
{
  /**
    * N_Ar Max Timeout Value 
    */
  ECO_CONSTVAR( u4, CANTP_VAR ) m_Timeout_N_Ar;
  /**
    * N_Br Performace Value
    */
  ECO_CONSTVAR( u4, CANTP_VAR ) m_Timeout_N_Br;
  /**
    * N_Cr Max Timeout Value
    */
  ECO_CONSTVAR( u4, CANTP_VAR ) m_Timeout_N_Cr;
  /**
    * Block Size
    */
  ECO_CONSTVAR( u1, CANTP_VAR ) m_BS;
  /**
    * Separation time min
    */
  ECO_CONSTVAR( u1, CANTP_VAR ) m_STMin;
  /**
    * The Max Time Of Flow Control Wait
    */
  ECO_CONSTVAR( u2, CANTP_VAR ) m_WftMax;
  /**
    * CAN Type
    */
  ECO_CONSTVAR( CanTp_CanType, CANTP_VAR ) m_CanType;
  /**
    * CAN Addressing Format Type
    */
  ECO_CONSTVAR( CanTp_AddrFormatType, CANTP_VAR ) m_AddrFormat;
  /**
    * Flag of CAN Padding or not
    */
  ECO_CONSTVAR( CanTp_CanPaddingType, CANTP_VAR ) m_IsPadding;
  /**
    * Target Address Type
    */
  ECO_CONSTVAR( CanTp_TaType, CANTP_VAR ) m_TaType;
  /**
    * Upper Layer Recived Protocal Data Unit Identifier
    */
  ECO_CONSTVAR( PduIdType, CANTP_VAR ) m_RxPduId;
  /**
    * Low Layer Recived Protocal Data Unit Identifier
    */
  ECO_CONSTVAR( PduIdType, CANTP_VAR ) m_RxCanIfId;
  /**
    * Reference Of Send Service Data Unit Identifier
    */
  ECO_CONSTVAR( CanTp_SduIdType, CANTP_VAR ) m_RefTxSduId;

}CanTp_RxSduCfgType;

typedef struct
{
  ECO_CONSTPVAR( CanTp_TxSduCfgType, CANTP_VAR ) m_TxSduCfgPtr;
  ECO_CONSTPVAR( CanTp_RxSduCfgType, CANTP_VAR ) m_RxSduCfgPtr;
}CanTp_ConfigType;

/*-----------------------------------------------------------------------------*
 * -ENUMS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL VARIABLE DECLARATIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/

#endif /* CANTP_TYPE_H */

