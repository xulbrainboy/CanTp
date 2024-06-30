
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
*   @file     CanTp_TxNSdu.c
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
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_GetTxNSduIdFromNSdu (     \
                                                     PduIdType tTxPduId,    \
                                                     CanTp_SduIdType* tTxNSduIdPtr );
/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( BufReq_ReturnType, CANTP_CODE ) CanTp_TxNSduToNPdu (
                                                    CanTp_SduIdType tTxNSduId,
                                                    const PduInfoType* tPduInfoPtr,
                                                    u1* tOfs );
ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_Transmit( PduIdType tTxPduId, 
                                                       const PduInfoType *tPduInfoPtr );
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_GetTxNSduIdFromNSdu
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - SWRS         : TODO
  */
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_GetTxNSduIdFromNSdu (     \
                                                     PduIdType tTxPduId,    \
                                                     CanTp_SduIdType* tTxNSduIdPtr )
{
    Std_ReturnType  tRetVal;
    CanTp_SduIdType tTxNSduId;

    tRetVal = E_NOT_OK;
    for ( tTxNSduId = 0U; tTxNSduId < CANTP_TXSDU_NUM; tTxNSduId++ )
    {
        if ( tTxPduId == CanTp_GetCfgTx_TxPduId(tTxNSduId) )
        {
            tRetVal = E_OK;
            break;
        }
    }

    *tTxNSduIdPtr = (CanTp_SduIdType)( tTxNSduId - 1U );

    return tRetVal;
}

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNSduToNPdu
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( BufReq_ReturnType, CANTP_CODE ) CanTp_TxNSduToNPdu (
                                                    CanTp_SduIdType tTxNSduId,
                                                    const PduInfoType* tPduInfoPtr,
                                                    u1* tOfs )
{
    BufReq_ReturnType tBufStatus;
    PduIdType tTxPduId;
    u1 tBs;
    PduInfoType tPduTmp;
    PduLengthType tSduLength;
    PduLengthType tPos = *tOfs;
    PduLengthType tRemainingTxBuffLen = CanTp_GetTxRemainBufLen(tTxNSduId);
    PduLengthType tTxTotalBuffLen = CanTp_GetTxBufTotalLength(tTxNSduId);
    CanTp_FrameStateType tFrameState = CanTp_GetTxFrameState(tTxNSduId);

    tPduTmp.SduDataPtr = &tPduInfoPtr->SduDataPtr[tPos];
    PduLengthType tDL = CanTp_GetMatchingDL( CanTp_GetCfgTx_CanType(tTxNSduId),
                                             tTxTotalBuffLen, tPos );

    tTxPduId = CanTp_GetCfgTx_TxPduId(tTxNSduId);
    tSduLength = tDL - tPos;

    if ( tTxTotalBuffLen <= tSduLength )
    {
        tPduTmp.SduLength = tTxTotalBuffLen;
    }
    else
    {
        tPduTmp.SduLength = tSduLength;
    }

    tBufStatus = PduR_CanTpCopyTxData( tTxPduId, &tPduTmp, NULL_PTR,
                                       &tRemainingTxBuffLen );

    if ( BUFREQ_OK == tBufStatus )
    {
        tPos += tPduTmp.SduLength;
        tTxTotalBuffLen -= tPduTmp.SduLength;
        CanTp_SetTxBufTotalLength( tTxNSduId, tTxTotalBuffLen );
        *tOfs = tPos;
        /* Stop N_As timer */
        CanTp_StopTimer( tTxNSduId, CANTP_TIMER_N_Ax, CANTP_SEND );

        /* Transfer buffer finish */
        if ( 0U == tTxTotalBuffLen )
        {
            tFrameState = CANTP_FRAME_OK;
            PduR_CanTpTxConfirmation( tTxPduId, E_OK );
        }
        else /* 0U < tTxTotalBuffLen */
        {
            tBs = CanTp_GetTxFrameBs(tTxNSduId);
            if ( 0x00U != tBs )
            {
                tBs--;
                tFrameState = CANTP_FRAME_REQUEST_CF;
                CanTp_SetTxFrameBs( tTxNSduId, tBs );
            }
            else /* 0x00U == tBs */
            {
                tFrameState = CANTP_FRAME_WAIT_CF;
                /* N_Bs timer start */
                CanTp_StartTimer( tTxNSduId, CANTP_TIMER_N_Bx, CanTp_GetCfgTx_N_Bs(tTxNSduId), CANTP_SEND );
            }
        }
    }
    else if ( BUFREQ_E_BUSY == tBufStatus )
    {
        ;/* Waiting for timer */
    }
    else /* BUFREQ_E_NOT_OK or BUFREQ_E_OVFL */
    {
        tFrameState = CANTP_IDLE;
        PduR_CanTpTxConfirmation( tTxPduId, E_NOT_OK );
    }

    CanTp_SetTxFrameState( tTxNSduId, tFrameState );

    return tBufStatus;
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_Transmit
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_Transmit( PduIdType tTxPduId, 
                                                       const PduInfoType *tPduInfoPtr )
{
    Std_ReturnType tRetVal = E_NOT_OK;
    CanTp_SduIdType tTxNSduId;
    PduLengthType tDL;
    u1 tOfs;
    CanTp_FrameStateType tFrameState = CANTP_IDLE;

    /* Check the Module runing or not */
    if ( CANTP_OFF == CanTp_GetRuntimeState() )
    {
        /* Report error */
    }
    else if ( NULL_PTR == tPduInfoPtr )
    {
        /* Report error */
    }
    /* Step 1: Get TxNSduId from Tx PduId */
    else if ( E_NOT_OK == CanTp_GetTxNSduIdFromNSdu( tTxPduId, &tTxNSduId ) )
    {
        /* Report error */
    }
    /* Step 2: Check tTxNSduId State  */
    else if ( CANTP_PROCESSING == CanTp_GetTxState( tTxNSduId ) )
    {
        /* Report error */
    }
    /* Step 3: check Transmit data length */
    else if ( tPduInfoPtr->SduLength == 0x0000U )
    {
        /* report error */
    }
    else
    {
        /* Step 4: Get Signal Frame Max Data Length */
        if ( ( CANTP_CAN_2_0 == CanTp_GetCfgTx_CanType(tTxNSduId)          )
          || ( CANTP_CAN_SMALL_CAN_FD == CanTp_GetCfgTx_CanType(tTxNSduId) ) )
        {
            tDL = (PduLengthType)( 7U - CanTp_GetBufferOffset_Af( tTxNSduId ) );
        }
        else
        {
            tDL = (PduLengthType)( 63U - CanTp_GetBufferOffset_Af( tTxNSduId ) );
        }

        /* Step 5: Get the Buffer Offset Based On Addressing Format Type */
        tOfs = CanTp_GetBufferOffset_Af( CanTp_GetCfgTx_AddrFormat(tTxNSduId) );
        tDL -= tOfs;

        /* Step 6: Set Frame State */
        if ( tPduInfoPtr->SduLength <= tDL )
        {
            tFrameState = CANTP_FRAME_REQUEST_SF;
        }
        else /* tPduInfoPtr->SduLength > tDL */
        {
            if ( CANTP_PHYSICAL == CanTp_GetCfgTx_TaType(tTxNSduId) )
            {
                tFrameState = CANTP_FRAME_REQUEST_FF;
            }
            else
            {
                ;
            }
        }

        if ( tFrameState != CANTP_IDLE )
        {
            tRetVal = E_OK;
            /* Set Total Data Length */
            CanTp_SetTxBufTotalLength( tTxNSduId, tPduInfoPtr->SduLength );
            /* Set tTxNSduId State */
            CanTp_SetTxState( tTxNSduId, CANTP_PROCESSING );
        }
    }

    CanTp_SetTxFrameState( tTxNSduId, tFrameState );

    return tRetVal;
}
/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
