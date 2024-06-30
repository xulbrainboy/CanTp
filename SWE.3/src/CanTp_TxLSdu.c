
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
*   @file     CanTp_TxLSdu.c
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

/*-----------------------------------------------------------------------------*
 * -INCLUDE FILES
 *-----------------------------------------------------------------------------*/
#include "CanTp.h"
#include "CanTp_Priv.h"
#include "Interface.h"
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
 * -GLOBAL VARIABLE DECLARATIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -LOCAL VARIABLE DECLARATIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_LSduTransmit_FC ( CanTp_SduIdType tRxNSduId,
                                                     CanTp_SduIdType tTxNSduId,
                                                     const PduInfoType* tPduInfoPtr );
ECO_FUNC( void, CANTP_CODE ) CanTp_LSduTransmit ( CanTp_SduIdType tTxNSduId,
                                                  const PduInfoType* tPduInfoPtr );
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_LSduTransmit_FC
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_LSduTransmit_FC ( CanTp_SduIdType tRxNSduId,
                                                     CanTp_SduIdType tTxNSduId,
                                                     const PduInfoType* tPduInfoPtr )
{
    PduIdType tTxPduId;

    tTxPduId = CanTp_GetCfgTx_TxCanIfId(tTxNSduId);

    /* Start N_Ar timer */
    CanTp_StartTimer( tTxNSduId, CANTP_TIMER_N_Ax, CanTp_GetCfgRx_N_Cr(tRxNSduId), CANTP_RECIVED );

    /* transfer Frame Infomation To CanIf Layer */
    return ( CanIf_Transmit( tTxPduId, tPduInfoPtr ) );

}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_LSduTransmit
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_LSduTransmit ( CanTp_SduIdType tTxNSduId,
                                                  const PduInfoType* tPduInfoPtr )
{
    PduIdType tTxPduId;

    tTxPduId = CanTp_GetCfgTx_TxCanIfId(tTxNSduId);

    /* Start N_As timer */
    CanTp_StartTimer( tTxNSduId, CANTP_TIMER_N_Ax, CanTp_GetCfgTx_N_Cs(tTxNSduId), CANTP_SEND );
    /* transfer Frame Infomation To CanIf Layer */
    if ( E_OK != CanIf_Transmit( tTxPduId, tPduInfoPtr ) )
    {
        CanTp_SetTxFrameState( tTxNSduId, CANTP_IDLE );
    }
    else
    {
        ;
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxConfirmation
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_TxConfirmation ( PduIdType TxPduId,
                                                    Std_ReturnType result )
{
    
}
/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
