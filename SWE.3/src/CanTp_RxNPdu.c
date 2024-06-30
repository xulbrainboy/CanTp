
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
ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_RxNPduReq_FC ( CanTp_SduIdType tRxNSduId );

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( void, CANTP_CODE ) CanTp_RxNPduInit ( CanTp_SduIdType tNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_RxNPduHandle ( CanTp_SduIdType tRxNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_Invalid ( CanTp_SduIdType tRxNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_SF ( CanTp_SduIdType tRxNSduId,
                                             const PduInfoType* tPduInfoPtr );
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_FF ( CanTp_SduIdType tRxNSduId, u2 tDL,
                                             const PduInfoType* tPduInfoPtr );
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_CF ( CanTp_SduIdType tRxNSduId,
                                             const PduInfoType* tPduInfoPtr );
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_FC ( CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr );
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxNPduReq_FC
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_RxNPduReq_FC ( CanTp_SduIdType tRxNSduId )
{
    u2 tWftMax = 0;
    Std_ReturnType tRetVal;
    CanTp_FrameStateType tFrameState = CanTp_GetRxFrameState(tRxNSduId);

    /* Stop N_Br timer */
    CanTp_StopTimer( tRxNSduId, CANTP_TIMER_N_Bx, CANTP_RECIVED );

    if ( CANTP_FSTYPE_WT == CanTp_GetFlowStatus(tRxNSduId) )
    {
        if ( FALSE == CanTp_IsTimeout(tRxNSduId, CANTP_TIMER_N_Bx, CANTP_RECIVED) )
        {
            tWftMax = CanTp_GetRxFrameWftMax( tRxNSduId );
            if ( 0x00U != tWftMax )
            {
                tWftMax--;
                CanTp_SetRxFrameWftMax( tRxNSduId, tWftMax );
            }
            else
            {
                tFrameState = CANTP_IDLE;
            }
        }
    }

    /* Build TX Flow Control */
    if ( CANTP_IDLE != tFrameState )
    {
        tRetVal = CanTp_BuildFlowControl( tRxNSduId );
        if ( E_OK == tRetVal )
        {
            tFrameState = CANTP_FRAME_OK;
        }
        else
        {
            tFrameState = CANTP_IDLE;
        }
    }
    
    /* Update the Frame State */
    CanTp_SetRxFrameState( tRxNSduId, tFrameState );
}

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxNPduInit
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxNPduInit ( CanTp_SduIdType tNSduId )
{

    CanTp_SetRxBufTotalLength( tNSduId, 0x00U );
    CanTp_SetRxRemainBufLen( tNSduId, 0x00U );
    CanTp_SetRxState( tNSduId, CANTP_WAIT );
    CanTp_SetRxFrameState( tNSduId, CANTP_IDLE );
    CanTp_SetRxFrameSTmin( tNSduId, CanTp_GetCfgRx_STmin(tNSduId) );
    CanTp_SetRxFrameSn( tNSduId, 0x00U );
    CanTp_SetRxFrameBs( tNSduId, CanTp_GetCfgRx_BS(tNSduId) );
    CanTp_SetRxFrameWftMax( tNSduId, CanTp_GetRxFrameWftMax(tNSduId) );

    CanTp_RxTimerInit( tNSduId );
    
    for ( u1 idx = 0; idx < CANTP_NSDUMAXSIZE; idx++ )
    {
        CanTp_SetRxNSduBuffer( tNSduId, idx, 0x00U );
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxNPduHandle
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxNPduHandle ( CanTp_SduIdType tRxNSduId )
{
    switch ( CanTp_GetRxFrameState(tRxNSduId) )
    {
        case CANTP_FRAME_REQUEST_FC:
            CanTp_RxNPduReq_FC( tRxNSduId );
          break;
        
        case CANTP_IDLE:
        default:
            CanTp_RxNPduInit( tRxNSduId );
          break;
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxLSduToNPdu_Invalid
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_Invalid ( CanTp_SduIdType tRxNSduId )
{
    /* Reset the Frame State to idle */
    CanTp_SetRxFrameState( tRxNSduId, CANTP_IDLE );
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxLSduToNPdu_SF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_SF ( CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr )
{
    CanTp_FrameStateType tFrameState = CANTP_IDLE;

    if ( CANTP_WAIT == CanTp_GetRxState( tRxNSduId ) )
    {
        /* Set connection channel state to processing */
        CanTp_SetRxState( tRxNSduId, CANTP_PROCESSING );
        /* Send NPdu infomation to NSdu */
        tFrameState = CanTp_RxNPduToNSdu_SF( tRxNSduId, tPduInfoPtr );
    }
    else
    {
        /* report error */
    }
    CanTp_SetRxFrameState( tRxNSduId, tFrameState );
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxLSduToNPdu_FF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_FF ( CanTp_SduIdType tRxNSduId, u2 tDL,
                                                    const PduInfoType* tPduInfoPtr )
{
    CanTp_FrameStateType tFrameState = CANTP_IDLE;

    if ( CANTP_WAIT == CanTp_GetRxState( tRxNSduId ) )
    {
        /* Set connection channel state to processing */
        CanTp_SetRxState( tRxNSduId, CANTP_PROCESSING );

        /* Set SN, BS and WftMax */
        CanTp_SetRxFrameSn( tRxNSduId, 0U );
        CanTp_SetRxFrameBs( tRxNSduId, CanTp_GetCfgRx_BS(tRxNSduId) );
        CanTp_SetRxFrameWftMax( tRxNSduId, CanTp_GetCfgRx_WftMax(tRxNSduId) );
        /* Send NPdu infomation to NSdu */
        tFrameState = CanTp_RxNPduToNSdu_FF( tRxNSduId, tDL, tPduInfoPtr );
    }
    else
    {
        /* report error */
    }
    CanTp_SetRxFrameState( tRxNSduId, tFrameState );
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxLSduToNPdu_CF
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_CF ( CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr )
{
    CanTp_FrameStateType tFrameState = CANTP_IDLE;
    u2 tBS;
    u1 tSn;

    if ( ( CANTP_PROCESSING == CanTp_GetRxState( tRxNSduId )                    )
      && ( FALSE == CanTp_IsTimeout(tRxNSduId, CANTP_TIMER_N_Cx, CANTP_RECIVED) ) )
    {
        /* Stop N_CR timer */
        CanTp_StopTimer( tRxNSduId, CANTP_TIMER_N_Cx, CANTP_RECIVED );

        tSn = CanTp_GetRxFrameSn( tRxNSduId ) + 1U;
        tBS = CanTp_GetRxFrameBs( tRxNSduId ) - 1U;
        CanTp_SetRxFrameSn( tRxNSduId, tSn );
        CanTp_SetRxFrameBs( tRxNSduId, tBS );
        /* Send NPdu infomation to NSdu */
        tFrameState = CanTp_RxNPduToNSdu_CF( tRxNSduId, tPduInfoPtr );
    }
    else
    {
        /* report error */
    }
    CanTp_SetRxFrameState( tRxNSduId, tFrameState );
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxLSduToNPdu_FC
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_FC ( CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr )
{
    CanTp_FrameStateType tFrameState = CANTP_IDLE;
    CanTp_SduIdType tTxNSduId;

    /* Find the Tx NSdu Id */
    tTxNSduId = CanTp_GetCfg_RxRefTxSduId(tRxNSduId);

    if ( ( CANTP_PROCESSING == CanTp_GetTxState( tTxNSduId )                 )
      && ( FALSE == CanTp_IsTimeout(tRxNSduId, CANTP_TIMER_N_Bx, CANTP_SEND) ) )
    {

        /* Stop timer N_Bs and start timer N_Cs */
        CanTp_StopTimer( tTxNSduId, CANTP_TIMER_N_Bx, CANTP_SEND );
        CanTp_StartTimer( tTxNSduId, CANTP_TIMER_N_Cx, CanTp_GetCfgTx_N_Cs(tTxNSduId), CANTP_SEND );

        switch ( tPduInfoPtr->SduDataPtr[0x00U] )
        {
            case CANTP_FSTYPE_CTS:
              CanTp_SetTxFrameBs(tTxNSduId, tPduInfoPtr->SduDataPtr[0x01U]);
              if ( tPduInfoPtr->SduDataPtr[0x02U] > 0x7FU ) /* > 127ms */
              {
                  CanTp_SetTxFrameSTmin( tTxNSduId, 0x7FU );
              }
              else
              {
                  CanTp_SetTxFrameSTmin( tTxNSduId, tPduInfoPtr->SduDataPtr[0x02U] );
              }
              tFrameState = CANTP_FRAME_REQUEST_CF;
              break;

            case CANTP_FSTYPE_WT:
              /* Start timer N_Bs */
              CanTp_StartTimer( tTxNSduId, CANTP_TIMER_N_Bx, CanTp_GetCfgTx_N_Bs(tTxNSduId), CANTP_SEND );
              tFrameState = CANTP_FRAME_WAIT_CF;
              break;

            case CANTP_FSTYPE_OVFL:
              break;

            default:
              break;
        }
    }
    else
    {
        /* report error */
    }
    CanTp_SetTxFrameState( tTxNSduId, tFrameState );
}

/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
