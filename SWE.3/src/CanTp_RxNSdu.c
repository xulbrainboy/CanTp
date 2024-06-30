
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
*   @file     CanTp_RxNSdu.c
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
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_SF ( 
                                             CanTp_SduIdType tRxNSduId,
                                             const PduInfoType* tPduInfoPtr );
ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_FF ( 
                                             CanTp_SduIdType tRxNSduId, u2 tDL,
                                             const PduInfoType* tPduInfoPtr );
ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_CF ( 
                                             CanTp_SduIdType tRxNSduId,
                                             const PduInfoType* tPduInfoPtr );
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxNPduToNSdu_SF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : SWS_CanTp_00339
  */
ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_SF ( 
                                                    CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr )
{
    CanTp_FrameStateType tFrameState;
    BufReq_ReturnType tBufStatus;
    PduIdType tRxPduId;
    PduLengthType tSduLength;
    PduLengthType tRemainingRxBuffLen = CanTp_GetRxRemainBufLen(tRxNSduId);
    
    tRxPduId = CanTp_GetCfg_RxPduId(tRxNSduId);
    tSduLength = tPduInfoPtr->SduLength;
    /* Set request to upper layer */
    tBufStatus = PduR_CanTpStartOfReception( tRxPduId, tPduInfoPtr, tSduLength,
                                             &tRemainingRxBuffLen );
    if ( tBufStatus == BUFREQ_OK )
    {
        if ( tRemainingRxBuffLen > tSduLength )
        {
            /* Copy Rx Data to upper layer */
            tBufStatus = PduR_CanTpCopyRxData( tRxPduId, tPduInfoPtr, &tRemainingRxBuffLen );
            if ( tBufStatus == BUFREQ_OK )
            {
                PduR_CanTpRxIndication( tRxPduId, E_OK );
                tFrameState = CANTP_FRAME_OK;
            }
            else
            {
                PduR_CanTpRxIndication( tRxPduId, E_NOT_OK );
                tFrameState = CANTP_IDLE;
            }
        }
        else
        {
            PduR_CanTpRxIndication( tRxPduId, E_NOT_OK );
            tFrameState = CANTP_IDLE;
        }
    }
    else /* BUFREQ_E_BUSY or BUFREQ_E_NOT_OK or BUFREQ_E_OVFL */
    {
        tFrameState = CANTP_IDLE;
    }
    
    CanTp_SetRxRemainBufLen( tRxNSduId, tRemainingRxBuffLen );
    
    return tFrameState;

}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxNPduToNSdu_FF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : SWS_CanTp_00339
  */
ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_FF ( 
                                                    CanTp_SduIdType tRxNSduId, u2 tDL,
                                                    const PduInfoType* tPduInfoPtr )
{
    CanTp_FrameStateType tFrameState;
    BufReq_ReturnType tBufStatus;
    PduIdType tRxPduId;
    PduLengthType tSduLength;
    PduLengthType tRemainingRxBuffLen = CanTp_GetRxRemainBufLen(tRxNSduId);
    
    tRxPduId = CanTp_GetCfg_RxPduId(tRxNSduId);
    tSduLength = tPduInfoPtr->SduLength;
    CanTp_SetRxBufTotalLength( tRxNSduId, tDL );
    /* Set request to upper layer */
    tBufStatus = PduR_CanTpStartOfReception( tRxPduId, tPduInfoPtr, tDL,
                                             &tRemainingRxBuffLen );
    if ( tBufStatus == BUFREQ_OK )
    {
        /* Start N_Br timer */
        CanTp_StartTimer( tRxNSduId, CANTP_TIMER_N_Bx, CanTp_GetCfgRx_N_Br(tRxNSduId), CANTP_RECIVED );

        if ( tRemainingRxBuffLen > tSduLength )
        {
            /* Copy Rx Data to upper layer */
            tBufStatus = PduR_CanTpCopyRxData( tRxPduId, tPduInfoPtr, &tRemainingRxBuffLen );
            if ( tBufStatus == BUFREQ_OK )
            {
                PduR_CanTpRxIndication( tRxPduId, E_OK );
                tFrameState = CANTP_FRAME_REQUEST_FC;
                CanTp_SetFlowStatus( tRxNSduId, CANTP_FSTYPE_CTS );
            }
            else
            {
                PduR_CanTpRxIndication( tRxPduId, E_NOT_OK );
                tFrameState = CANTP_IDLE;
            }
        }
        else
        {
            PduR_CanTpRxIndication( tRxPduId, E_NOT_OK );
            tFrameState = CANTP_IDLE;
        }
    }
    else if ( tBufStatus == BUFREQ_E_OVFL )
    {
        tFrameState = CANTP_FRAME_REQUEST_FC;
        CanTp_SetFlowStatus( tRxNSduId, CANTP_FSTYPE_OVFL );
    }
    else /* BUFREQ_E_BUSY or BUFREQ_E_NOT_OK */
    {
        tFrameState = CANTP_IDLE;
    }

    CanTp_SetRxRemainBufLen( tRxNSduId, tRemainingRxBuffLen );

    return tFrameState;
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxNPduToNSdu_CF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_CF ( 
                                                    CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr )
{
    CanTp_FrameStateType tFrameState;
    PduIdType tRxPduId;
    BufReq_ReturnType tBufStatus;
    PduLengthType tRemainingRxBuffLen = CanTp_GetRxRemainBufLen(tRxNSduId);
    PduLengthType tRxBufSize = CanTp_GetRxBufTotalLength(tRxNSduId);

    tRxPduId = CanTp_GetCfg_RxPduId(tRxNSduId);
    /* Copy Rx Data to upper layer */
    tBufStatus = PduR_CanTpCopyRxData( tRxPduId, tPduInfoPtr, &tRemainingRxBuffLen );
    if ( tBufStatus == BUFREQ_OK )
    {
        /* remaining buffer size  */
        tRxBufSize = (PduLengthType)( tRxBufSize - tPduInfoPtr->SduLength );
        CanTp_SetRxBufTotalLength( tRxNSduId, tRxBufSize );
        if ( 0x00 != tRxBufSize )
        {
            if ( 0x00 == CanTp_GetRxFrameBs(tRxNSduId) )
            {
                CanTp_SetRxFrameBs( tRxNSduId, CanTp_GetCfgRx_BS(tRxNSduId) );
                tFrameState = CANTP_FRAME_REQUEST_FC;
                /* Start N_Br timer */
                CanTp_StartTimer( tRxNSduId, CANTP_TIMER_N_Bx, CanTp_GetCfgRx_N_Br(tRxNSduId), CANTP_RECIVED );
            }
            else
            {
                tFrameState = CANTP_FRAME_CONTINUE;
                CanTp_StartTimer( tRxNSduId, CANTP_TIMER_N_Cx, CanTp_GetCfgRx_N_Cr(tRxNSduId), CANTP_RECIVED );
            }
        }
        else /* Transfer complete */
        {
            PduR_CanTpRxIndication( tRxPduId, E_OK );
            tFrameState = CANTP_FRAME_OK;
        }
    }
    else
    {
        PduR_CanTpRxIndication( tRxPduId, E_NOT_OK );
        tFrameState = CANTP_IDLE;
    }

    CanTp_SetRxRemainBufLen( tRxNSduId, tRemainingRxBuffLen );

    return tFrameState;
}

/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
