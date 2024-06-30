
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
*   @file     CanTp_TxNPdu.c
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
ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_TxNPduSetPadding ( CanTp_SduIdType tTxNSduId,
                                                           PduInfoType* tPduInfoPtr,
                                                           u1 tOfs );
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_TxNPduReq_SF ( CanTp_SduIdType tTxNSduId,
                                                       PduInfoType* tPduInfoPtr,
                                                       u1* tOfs );
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_TxNPduReq_FF ( CanTp_SduIdType tTxNSduId,
                                                       PduInfoType* tPduInfoPtr,
                                                       u1* tOfs );
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_TxNPduReq_CF ( CanTp_SduIdType tTxNSduId,
                                                       PduInfoType* tPduInfoPtr,
                                                       u1* tOfs );
ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_TxNPduReq ( CanTp_SduIdType tTxNSduId,
                                                    CanTp_FrameStateType tFrameState );

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( void, CANTP_CODE ) CanTp_TxNPduInit ( CanTp_SduIdType tNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_TxNPduHandle ( CanTp_SduIdType tTxNSduId );
ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_BuildFlowControl ( 
                                                     CanTp_SduIdType tRxNSduId );
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNPduSetPadding
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_TxNPduSetPadding ( CanTp_SduIdType tTxNSduId,
                                                           PduInfoType* tPduInfoPtr,
                                                           u1 tOfs )
{
    u1 tDL = 0U;
    CanTp_CanType tCanType;

    tCanType = CanTp_GetCfgTx_CanType(tTxNSduId);
    tDL = CanTp_GetMatchingDL( tCanType, 0x00U, tOfs );
    /* Set padding */
    for ( ; tOfs < tDL; tOfs++ )
    {
        tPduInfoPtr->SduDataPtr[tOfs] = CANTP_PADDING_VALUE;
    }
    tPduInfoPtr->SduLength = tDL;

}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNPduReq_SF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_TxNPduReq_SF ( CanTp_SduIdType tTxNSduId,
                                                       PduInfoType* tPduInfoPtr,
                                                       u1* tOfs )
{
    Std_ReturnType tRetVal;
    u1 tPos = *tOfs;
    PduLengthType tBufSize;

    /* Combined PCI and DL */
    tBufSize = CanTp_GetTxBufTotalLength( tTxNSduId );

    if ( ( CANTP_CAN_2_0 == CanTp_GetCfgTx_CanType(tTxNSduId)          )
      || ( CANTP_CAN_SMALL_CAN_FD == CanTp_GetCfgTx_CanType(tTxNSduId) ) )
    {
        tPduInfoPtr->SduDataPtr[tPos] = CANTP_SINGLE_FRAME + ( tBufSize & 0x0FU );
        tPos++;
    }
    else
    {
        tPduInfoPtr->SduDataPtr[tPos] = CANTP_SINGLE_FRAME;
        tPos++;
        tPduInfoPtr->SduDataPtr[tPos] = tBufSize;
        tPos++;
    }

    /* Copy Tx Data */
    if ( BUFREQ_OK == CanTp_TxNSduToNPdu( tTxNSduId, tPduInfoPtr, &tPos ) )
    {
        *tOfs = tPos;
        /* Set Data Length */
        tPduInfoPtr->SduLength = tPos;
        tRetVal = E_OK;
    }
    else
    {
        tRetVal = E_NOT_OK;
    }

    return tRetVal;
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNPduReq_FF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_TxNPduReq_FF ( CanTp_SduIdType tTxNSduId,
                                                       PduInfoType* tPduInfoPtr,
                                                       u1* tOfs )
{
    Std_ReturnType tRetVal;
    u1 tPos = *tOfs;
    PduLengthType tBufSize;

    /* Combined PCI and DL */
    tBufSize = CanTp_GetTxBufTotalLength( tTxNSduId );

    /* DL < 4096 */
    if ( tBufSize <= 0xFFFU )
    {
        tPduInfoPtr->SduDataPtr[tPos] = CANTP_FIRST_FRAME + (u1)( (u1)( tBufSize >> 8U ) & 0x0FU );
        tPos++;
        tPduInfoPtr->SduDataPtr[tPos] = (u1)tBufSize & 0xFFU;
        tPos++;
    }
    else /* DL >= 4096 */
    {
        /* TODO */
    }

    /* Reset Sn */
    CanTp_SetTxFrameSn( tTxNSduId, 0x00U );
    /* Reset Bs */
    CanTp_SetTxFrameBs( tTxNSduId, 0x00U );
    /* Copy Tx Data */
    if ( BUFREQ_OK == CanTp_TxNSduToNPdu( tTxNSduId, tPduInfoPtr, &tPos ) )
    {
        *tOfs = tPos;
        /* Set Data Length */
        tPduInfoPtr->SduLength = tPos;
        tRetVal = E_OK;
    }
    else
    {
        tRetVal = E_NOT_OK;
    }

    return tRetVal;
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNPduReq_CF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_TxNPduReq_CF ( CanTp_SduIdType tTxNSduId,
                                                       PduInfoType* tPduInfoPtr,
                                                       u1* tOfs )
{
    Std_ReturnType tRetVal;
    u1 tPos = *tOfs;
    u1 tSn;

    /* Combined PCI and DL */
    tSn = CanTp_GetTxFrameSn( tTxNSduId );
    tPduInfoPtr->SduDataPtr[tPos] = CANTP_CONSECUTIVE_FRAME + ( tSn & 0x0FU );
    tPos++;
    /* Copy Tx Data */
    if ( BUFREQ_OK == CanTp_TxNSduToNPdu( tTxNSduId, tPduInfoPtr, &tPos ) )
    {
        /* Start STmin timer */
        CanTp_StartTimer( tTxNSduId, CANTP_TIMER_N_STMINx, CanTp_GetTxFrameSTmin(tTxNSduId), CANTP_SEND );
        *tOfs = tPos;
        /* Set Data Length */
        tPduInfoPtr->SduLength = tPos;
        tSn++;
        CanTp_SetTxFrameSn( tTxNSduId, tSn );
        /* Stop N_Cs timer */
        CanTp_StopTimer( tTxNSduId, CANTP_TIMER_N_Cx, CANTP_SEND );

        tRetVal = E_OK;
    }
    else
    {
        tRetVal = E_NOT_OK;
    }
    
    return tRetVal;
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNPduReq
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_TxNPduReq ( CanTp_SduIdType tTxNSduId,
                                                    CanTp_FrameStateType tFrameState )
{
    u1 tOfs = 0U;
    Std_ReturnType tRetVal = E_NOT_OK;
    PduInfoType tPduInfo;

    tPduInfo.SduDataPtr = &CanTp_GetTxNSduBuffer(tTxNSduId, 0x00U);

    /* Step 1: Check SaTaAe */
    if ( NULL_PTR != CanTp_GetCfgTx_SaTaAePtr(tTxNSduId) )
    {
        tPduInfo.SduDataPtr[tOfs] = *(CanTp_GetCfgTx_SaTaAePtr(tTxNSduId));
        tOfs++;
    }
    /* Step 2: Get Buffer */
    switch ( tFrameState )
    {
        case CANTP_FRAME_REQUEST_SF:
          tRetVal = CanTp_TxNPduReq_SF( tTxNSduId, &tPduInfo, &tOfs );
          break;

        case CANTP_FRAME_REQUEST_FF:
          tRetVal = CanTp_TxNPduReq_FF( tTxNSduId, &tPduInfo, &tOfs );
          break;

        case CANTP_FRAME_REQUEST_CF:
          tRetVal = CanTp_TxNPduReq_CF( tTxNSduId, &tPduInfo, &tOfs );
          break;

        default:
          break;
    }

    if ( E_OK == tRetVal )
    {
        /* Step 3: check Padding */
        if ( CANTP_PADDING_ON == CanTp_GetCfgTx_IsPadding(tTxNSduId) )
        {
            CanTp_TxNPduSetPadding( tTxNSduId, &tPduInfo, tOfs );
        }
        /* Step 4: transfer to L-NSdu */
        CanTp_LSduTransmit( tTxNSduId, &tPduInfo );
    }
}

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNPduInit
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_TxNPduInit ( CanTp_SduIdType tNSduId )
{
    CanTp_SetTxBufTotalLength( tNSduId, 0x00U );
    CanTp_SetTxRemainBufLen( tNSduId, 0x00U );
    CanTp_SetTxState( tNSduId, CANTP_WAIT );
    CanTp_SetTxFrameState( tNSduId, CANTP_IDLE );
    CanTp_SetTxFrameSTmin( tNSduId, 0x00U );
    CanTp_SetTxFrameSn( tNSduId, 0x00U );
    CanTp_SetTxFrameBs( tNSduId, 0x00U );

    CanTp_TxTimerInit( tNSduId );
    
    for ( u1 idx = 0; idx < CANTP_NSDUMAXSIZE; idx++ )
    {
        CanTp_SetTxNSduBuffer( tNSduId, idx, 0x00U );
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxNPduHandle
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_TxNPduHandle ( CanTp_SduIdType tTxNSduId )
{

    switch ( CanTp_GetTxFrameState(tTxNSduId) )
    {
        /* Request signal frame */
        case CANTP_FRAME_REQUEST_SF:
          CanTp_TxNPduReq( tTxNSduId, CANTP_FRAME_REQUEST_SF );
          break;
        
        /* Request first frame */
        case CANTP_FRAME_REQUEST_FF:
          CanTp_TxNPduReq( tTxNSduId, CANTP_FRAME_REQUEST_FF );
          break;

        /* Request consecutive frame */
        case CANTP_FRAME_REQUEST_CF:
          /* N_Cs timer not expired and ( STMIN expired or first CF start */
          if ( FALSE == CanTp_IsTimeout(tTxNSduId, CANTP_TIMER_N_Cx, CANTP_SEND) )
          {
              if ( ( TRUE == CanTp_IsTimeout(tTxNSduId, CANTP_TIMER_N_STMINx, CANTP_SEND) )
                || ( FALSE == CanTp_GetTxTimeFlag(tTxNSduId, CANTP_TIMER_N_STMINx)        ) )
              {
                  CanTp_TxNPduReq( tTxNSduId, CANTP_FRAME_REQUEST_CF );
              }
          }
          else
          {
              CanTp_SetTxFrameState( tTxNSduId, CANTP_IDLE ); /* reset frame state */
          }
          break;

        /* Waiting flow control frame  */
        case CANTP_FRAME_WAIT_CF:
          break;

        /* Invalid frame or error occurred */
        case CANTP_IDLE:
        default:
          CanTp_TxNPduInit( tTxNSduId );
          break;
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_BuildFlowControl
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_BuildFlowControl ( 
                                                     CanTp_SduIdType tRxNSduId )
{
    Std_ReturnType tRetVal = E_NOT_OK;
    CanTp_SduIdType tTxNSduId;
    u1 tOfs = 0U;
    PduInfoType tPduInfo;

    /* Find the Tx NSdu Id */
    tTxNSduId = CanTp_GetCfg_RxRefTxSduId(tRxNSduId);

    tPduInfo.SduDataPtr = &CanTp_GetRxNSduBuffer(tRxNSduId, 0x00U);
    /* Step 1: Check SaTaAe */
    if ( NULL_PTR != CanTp_GetCfgTx_SaTaAePtr(tTxNSduId) )
    {
        tPduInfo.SduDataPtr[tOfs] = *(CanTp_GetCfgTx_SaTaAePtr(tTxNSduId));
        tOfs++;
    }
    /* Step 2: Build Flow Control Info */
    tPduInfo.SduDataPtr[tOfs] = CanTp_GetFlowStatus(tRxNSduId);
    tOfs++;
    tPduInfo.SduDataPtr[tOfs] = CanTp_GetRxFrameBs(tRxNSduId);
    tOfs++;
    tPduInfo.SduDataPtr[tOfs] = CanTp_GetRxFrameSTmin(tRxNSduId);
    tOfs++;

    /* Step 3: check Padding */
    if ( CANTP_PADDING_ON == CanTp_GetCfgTx_IsPadding(tRxNSduId) )
    {
        CanTp_TxNPduSetPadding( tTxNSduId, &tPduInfo, tOfs );
    }
    /* Step 4: transfer to L-NSdu */
    tRetVal = CanTp_LSduTransmit_FC( tRxNSduId, tTxNSduId, &tPduInfo );
    
    return tRetVal;
}

/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
