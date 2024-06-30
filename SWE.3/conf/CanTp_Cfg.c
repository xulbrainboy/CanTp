
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
*   @file           : CanTp_Cfg.c
*   @brief          : Configration File
*   @Generated Date : 
*   @details        : 
*/
/*=============================================================================*/

/*-----------------------------------------------------------------------------*
 * -INCLUDE FILES
 *-----------------------------------------------------------------------------*/
#include "CanTp_Cfg.h"

/*-----------------------------------------------------------------------------*
 * -FILE VERSION
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -DEFINITION AND TYPEDEF
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -ENUMS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -LOCAL VARIABLE DECLARATIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL VARIABLE DECLARATIONS
 *-----------------------------------------------------------------------------*/
ECO_CONSTVAR( CanTp_RxSduCfgType, CANTP_CONST ) CanTp_RxSduCfg[CANTP_RXSDU_NUM] =
{
  /* Rx SDU 0 */
  {
    100U,
    100U,
    100U,
    0x0FU,
    50U,
    3U,
    CANTP_CAN_2_0,
    CANTP_STANDARD,
    CANTP_ON,
    CANTP_PHYSICAL,
    1U,
    1U,
    0U
  }
};

ECO_CONSTVAR( CanTp_TxSduCfgType, CANTP_CONST ) CanTp_TxSduCfg[CANTP_TXSDU_NUM] =
{
  /* Tx SDU 0 */
  {
    100U,
    100U,
    100U,
    NULL_PTR,
    CANTP_CAN_2_0,
    CANTP_STANDARD,
    CANTP_ON,
    CANTP_PHYSICAL,
    0U,
    0U,
    0U
  }
};

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
