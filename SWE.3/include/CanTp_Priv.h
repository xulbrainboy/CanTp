
/*******************************************************************************
*   Project              : none                                               **
*   Peripheral           : none                                               **
*   Dependencies         : none                                               **
*                                                                             **
*   SW Version           : 1.0.0                                              **
*   (c) Copyright xxx                                                         **
*   All Rights Reserved.                                                      **
********************************************************************************/
/**
*   @file     CanTp_Priv.h
*   @brief    
*   @details  
*/
/*=============================================================================*/
/*-----------------------------------------------------------------------------*
 * -VERSION HISTORY
 *-----------------------------------------------------------------------------*/
/*  -VERSION-   -DATE-       -AUTHOR-    -COMMENT-
 *  V1.0.0      2021-11-20   LIANG.X     create
 *
 */
/*=============================================================================*/

#ifndef CANTP_PRIV_H
#define CANTP_PRIV_H

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
/**
  * N-PCI Field Mask For Different Frames
  */
#define CANTP_SINGLE_FRAME              0x00U
#define CANTP_FIRST_FRAME               0x10U
#define CANTP_CONSECUTIVE_FRAME         0x20U
#define CANTP_FLOWCONTROL_FRAME         0x30U

#define CANTP_NPCI_FIEILDMASK           0xF0U

/**
  * Flow Status Type
  */
#define CANTP_FSTYPE_CTS                0x30U
#define CANTP_FSTYPE_WT                 0x31U
#define CANTP_FSTYPE_OVFL               0x32U

/**
  * CAN 2.0 or Small FD Data Length
  */
#define CANTP_CAN2_0_SMALLFD_MAXSIZE       8U

/**
  * Frame State
  */
typedef enum
{
  CANTP_FRAME_OK = 0U,
  CANTP_IDLE = 1U,
  CANTP_FRAME_REQUEST_SF = 2U,
  CANTP_FRAME_REQUEST_FF = 3U,
  CANTP_FRAME_REQUEST_CF = 4U,
  CANTP_FRAME_WAIT_CF = 5U,
  CANTP_FRAME_REQUEST_FC = 6U,
  CANTP_FRAME_CONTINUE = 7U
}CanTp_FrameStateType;

/**
  * Timer Type
  */
typedef enum
{
  CANTP_TIMER_N_Ax = 0U,
  CANTP_TIMER_N_Bx = 1U,
  CANTP_TIMER_N_Cx = 2U,
  CANTP_TIMER_N_STMINx = 3U,
  CANTP_TIMER_MAX = 4U
}CanTp_TimerType;

/**
  * Timer Infomation
  */
typedef struct
{
  /**
    * Timer Enable or not flag
    */
  ECO_VAR( u1, CANTP_VAR ) m_TimerEnableFlag;
  /**
    * Timer
    */
  ECO_VAR( u4, CANTP_VAR ) m_Timer[CANTP_TIMER_MAX];
}CanTp_TimerInfoType;

/**
  * Runtime Cache Buffer
  */
typedef struct
{
  /* Cache Buffer */
  ECO_VAR( u1, CANTP_VAR ) m_NSduBuffer[CANTP_NSDUMAXSIZE];
  /* Total Length */
  ECO_VAR( PduLengthType, CANTP_VAR ) m_BufTotalLength;
  /* Remain buffer Length */
  ECO_VAR( PduLengthType, CANTP_VAR ) m_RemainBufLength;
}CanTp_RTCacheBufType;

/**
  * Recived Runtime Connection Information
  */
typedef struct
{
  /* Runtime Cache Buffer */
  ECO_VAR( CanTp_RTCacheBufType, CANTP_VAR ) m_buf;
  /* Runtime State */
  ECO_VAR( CanTp_StateType, CANTP_VAR ) m_RtState;
  /* The State of Sdu */
  ECO_VAR( CanTp_FrameStateType, CANTP_VAR ) m_FrameState;
  /* Runtime Timer */
  ECO_VAR( CanTp_TimerInfoType, CANTP_VAR ) m_TmInfo;
  /* Frame Paramater STmin Value */
  ECO_VAR( u4, CANTP_VAR ) m_ParamSTmin;
  /* Frame Sequence Number */
  ECO_VAR( u1, CANTP_VAR ) m_Sn;
  /* Frame Block Size */
  ECO_VAR( u1, CANTP_VAR ) m_Bs;
  /* The Max Time Of Flow Control Wait */
  ECO_VAR( u2, CANTP_VAR ) m_WftMax;
  /* Flow Status */
  ECO_VAR( u1, CANTP_VAR ) m_Fs;
}CanTp_RxRtConnInfoType;

/**
  * Send Runtime Connection Information
  */
typedef struct
{
  /* Runtime Cache Buffer */
  ECO_VAR( CanTp_RTCacheBufType, CANTP_VAR ) m_buf;
  /* Runtime State */
  ECO_VAR( CanTp_StateType, CANTP_VAR ) m_RtState;
  /* The State of Sdu */
  ECO_VAR( CanTp_FrameStateType, CANTP_VAR ) m_FrameState;
  /* Runtime Timer */
  ECO_VAR( CanTp_TimerInfoType, CANTP_VAR ) m_TmInfo;
  /* Frame Target STmin Value */
  ECO_VAR( u4, CANTP_VAR ) m_TargetSTmin;
  /* Frame Sequence Number */
  ECO_VAR( u1, CANTP_VAR ) m_Sn;
  /* Frame Block Size */
  ECO_VAR( u1, CANTP_VAR ) m_Bs;
}CanTp_TxRtConnInfoType;

/**
  * CanTp Module Runtime State
  */
#define CanTp_GetRuntimeState()        CanTp_RTState
#define CanTp_SetRuntimeState(state)   ( CanTp_GetRuntimeState() = (state) )

/**
  * Rx SDU Configuration Interface
  */
#define CanTp_GetCfgRx_N_Ar(idx)       CanTp_RxSduCfg[idx].m_Timeout_N_Ar
#define CanTp_GetCfgRx_N_Br(idx)       CanTp_RxSduCfg[idx].m_Timeout_N_Br
#define CanTp_GetCfgRx_N_Cr(idx)       CanTp_RxSduCfg[idx].m_Timeout_N_Cr
#define CanTp_GetCfgRx_BS(idx)         CanTp_RxSduCfg[idx].m_BS
#define CanTp_GetCfgRx_STmin(idx)      CanTp_RxSduCfg[idx].m_STMin
#define CanTp_GetCfgRx_WftMax(idx)     CanTp_RxSduCfg[idx].m_WftMax
#define CanTp_GetCfgRx_CanType(idx)    CanTp_RxSduCfg[idx].m_CanType
#define CanTp_GetCfgRx_AddrFormat(idx) CanTp_RxSduCfg[idx].m_AddrFormat
#define CanTp_GetCfgRx_IsPadding(idx)  CanTp_RxSduCfg[idx].m_IsPadding
#define CanTp_GetCfgRx_TaType(idx)     CanTp_RxSduCfg[idx].m_TaType
#define CanTp_GetCfg_RxPduId(idx)      CanTp_RxSduCfg[idx].m_RxPduId
#define CanTp_GetCfg_RxCanIfId(idx)    CanTp_RxSduCfg[idx].m_RxCanIfId
#define CanTp_GetCfg_RxRefTxSduId(idx) CanTp_RxSduCfg[idx].m_RefTxSduId

/**
  * Tx SDU Configuration Interface
  */
#define CanTp_GetCfgTx_N_As(idx)       CanTp_TxSduCfg[idx].m_Timeout_N_As
#define CanTp_GetCfgTx_N_Bs(idx)       CanTp_TxSduCfg[idx].m_Timeout_N_Bs
#define CanTp_GetCfgTx_N_Cs(idx)       CanTp_TxSduCfg[idx].m_Timeout_N_Cs
#define CanTp_GetCfgTx_SaTaAePtr(idx)  CanTp_TxSduCfg[idx].m_SaTaAePtr
#define CanTp_GetCfgTx_CanType(idx)    CanTp_TxSduCfg[idx].m_CanType
#define CanTp_GetCfgTx_AddrFormat(idx) CanTp_TxSduCfg[idx].m_AddrFormat
#define CanTp_GetCfgTx_IsPadding(idx)  CanTp_TxSduCfg[idx].m_IsPadding
#define CanTp_GetCfgTx_TaType(idx)     CanTp_TxSduCfg[idx].m_TaType
#define CanTp_GetCfgTx_TxPduId(idx)    CanTp_TxSduCfg[idx].m_TxPduId
#define CanTp_GetCfgTx_TxCanIfId(idx)  CanTp_TxSduCfg[idx].m_TxCanIfId
#define CanTp_GetCfgTx_RefRxSduId(idx) CanTp_TxSduCfg[idx].m_RefRxSduId

/**
  * Rx SDU Runtime Data Get Interface
  */
#define CanTp_GetRxNSduBuffer(idx,idx2)       CanTp_RxRtConnSdu[idx].m_buf.m_NSduBuffer[idx2]
#define CanTp_GetRxBufTotalLength(idx)        CanTp_RxRtConnSdu[idx].m_buf.m_BufTotalLength
#define CanTp_GetRxRemainBufLen(idx)          CanTp_RxRtConnSdu[idx].m_buf.m_RemainBufLength
#define CanTp_GetRxState(idx)                 CanTp_RxRtConnSdu[idx].m_RtState
#define CanTp_GetRxFrameState(idx)            CanTp_RxRtConnSdu[idx].m_FrameState
#define CanTp_GetRxTimeTotalFlag(idx)         CanTp_RxRtConnSdu[idx].m_TmInfo.m_TimerEnableFlag
#define CanTp_GetRxTimeFlag(idx,idx2)         ( CanTp_GetRxTimeTotalFlag(idx) & ( 1U << idx2 ) )
#define CanTp_GetRxTimer(idx,idx2)            CanTp_RxRtConnSdu[idx].m_TmInfo.m_Timer[idx2]
#define CanTp_GetRxFrameSTmin(idx)            CanTp_RxRtConnSdu[idx].m_ParamSTmin
#define CanTp_GetRxFrameSn(idx)               CanTp_RxRtConnSdu[idx].m_Sn
#define CanTp_GetRxFrameBs(idx)               CanTp_RxRtConnSdu[idx].m_Bs
#define CanTp_GetRxFrameWftMax(idx)           CanTp_RxRtConnSdu[idx].m_WftMax
#define CanTp_GetFlowStatus(idx)              CanTp_RxRtConnSdu[idx].m_Fs

/**
  * Rx SDU Runtime Data Set Interface
  */
#define CanTp_SetRxNSduBuffer(idx,idx2,value) ( CanTp_GetRxNSduBuffer(idx,idx2) = (value) )
#define CanTp_SetRxBufTotalLength(idx,value)  ( CanTp_GetRxBufTotalLength(idx) = (value) )
#define CanTp_SetRxRemainBufLen(idx,value)    ( CanTp_GetRxRemainBufLen(idx) = (value) )
#define CanTp_SetRxState(idx,value)           ( CanTp_GetRxState(idx) = (value) )
#define CanTp_SetRxFrameState(idx,value)      ( CanTp_GetRxFrameState(idx) = (value) )
#define CanTp_SetRxTimeEnbale(idx,idx2)       ( CanTp_GetRxTimeTotalFlag(idx) |= ( 1U << (idx2) ) )
#define CanTp_SetRxTimeDisable(idx,idx2)      ( CanTp_GetRxTimeTotalFlag(idx) &= ~( 1U << (idx2) ) )
#define CanTp_SetRxTimer(idx,idx2,value)      ( CanTp_GetRxTimer(idx,idx2) = (value) )
#define CanTp_DecRxTimer(idx,idx2)            ( CanTp_GetRxTimer(idx,idx2)-- )
#define CanTp_SetRxFrameSTmin(idx,value)      ( CanTp_GetRxFrameSTmin(idx) = (value) )
#define CanTp_SetRxFrameSn(idx,value)         ( CanTp_GetRxFrameSn(idx) = (value) )
#define CanTp_SetRxFrameBs(idx,value)         ( CanTp_GetRxFrameBs(idx) = (value) )
#define CanTp_SetRxFrameWftMax(idx,value)     ( CanTp_GetRxFrameWftMax(idx) = (value) )
#define CanTp_SetFlowStatus(idx,value)        ( CanTp_GetFlowStatus(idx) = (value) )

/**
  * Tx SDU Runtime Data Get Interface
  */
#define CanTp_GetTxNSduBuffer(idx,idx2)       CanTp_TxRtConnSdu[idx].m_buf.m_NSduBuffer[idx2]
#define CanTp_GetTxBufTotalLength(idx)        CanTp_TxRtConnSdu[idx].m_buf.m_BufTotalLength
#define CanTp_GetTxRemainBufLen(idx)          CanTp_TxRtConnSdu[idx].m_buf.m_RemainBufLength
#define CanTp_GetTxState(idx)                 CanTp_TxRtConnSdu[idx].m_RtState
#define CanTp_GetTxFrameState(idx)            CanTp_TxRtConnSdu[idx].m_FrameState
#define CanTp_GetTxTimeTotalFlag(idx)         CanTp_TxRtConnSdu[idx].m_TmInfo.m_TimerEnableFlag
#define CanTp_GetTxTimeFlag(idx,idx2)         ( CanTp_GetTxTimeTotalFlag(idx) & ( 1U << idx2 ) )
#define CanTp_GetTxTimer(idx,idx2)            CanTp_TxRtConnSdu[idx].m_TmInfo.m_Timer[idx2]
#define CanTp_GetTxFrameSTmin(idx)            CanTp_TxRtConnSdu[idx].m_TargetSTmin
#define CanTp_GetTxFrameSn(idx)               CanTp_TxRtConnSdu[idx].m_Sn
#define CanTp_GetTxFrameBs(idx)               CanTp_TxRtConnSdu[idx].m_Bs

/**
  * Tx SDU Runtime Data Set Interface
  */
#define CanTp_SetTxNSduBuffer(idx,idx2,value) ( CanTp_GetTxNSduBuffer(idx,idx2) = (value) )
#define CanTp_SetTxBufTotalLength(idx,value)  ( CanTp_GetTxBufTotalLength(idx) = (value) )
#define CanTp_SetTxRemainBufLen(idx,value)    ( CanTp_GetTxRemainBufLen(idx) = (value) )
#define CanTp_SetTxState(idx,value)           ( CanTp_GetTxState(idx) = (value) )
#define CanTp_SetTxFrameState(idx,value)      ( CanTp_GetTxFrameState(idx) = (value) )
#define CanTp_SetTxTimeEnbale(idx,idx2)       ( CanTp_GetTxTimeTotalFlag(idx) |= ( 1U << (idx2) ) )
#define CanTp_SetTxTimeDisable(idx,idx2)      ( CanTp_GetTxTimeTotalFlag(idx) &= ~( 1U << (idx2) ) )
#define CanTp_SetTxTimer(idx,idx2,value)      ( CanTp_GetTxTimer(idx,idx2) = (value) )
#define CanTp_DecTxTimer(idx,idx2)            ( CanTp_GetTxTimer(idx,idx2)-- )
#define CanTp_SetTxFrameSTmin(idx,value)      ( CanTp_GetTxFrameSTmin(idx) = (value) )
#define CanTp_SetTxFrameSn(idx,value)         ( CanTp_GetTxFrameSn(idx) = (value) )
#define CanTp_SetTxFrameBs(idx,value)         ( CanTp_GetTxFrameBs(idx) = (value) )

/*-----------------------------------------------------------------------------*
 * -ENUMS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL VARIABLE DECLARATIONS
 *-----------------------------------------------------------------------------*/
extern ECO_CONSTVAR( CanTp_RxSduCfgType, CANTP_CONST ) CanTp_RxSduCfg[CANTP_RXSDU_NUM];
extern ECO_CONSTVAR( CanTp_TxSduCfgType, CANTP_CONST ) CanTp_TxSduCfg[CANTP_TXSDU_NUM];
extern ECO_VAR( CanTp_RuntimeType, CANTP_VAR ) CanTp_RTState;
extern ECO_VAR( CanTp_RxRtConnInfoType, CANTP_VAR ) CanTp_RxRtConnSdu[CANTP_RXSDU_NUM];
extern ECO_VAR( CanTp_TxRtConnInfoType, CANTP_VAR ) CanTp_TxRtConnSdu[CANTP_TXSDU_NUM];

/*-----------------------------------------------------------------------------*
 * -FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
extern ECO_FUNC( u1, CANTP_CODE ) CanTp_GetBufferOffset_Af ( CanTp_AddrFormatType tAddrFormat );
extern ECO_FUNC( u1, CANTP_CODE ) CanTp_GetMatchingDL ( CanTp_CanType tCanType, 
                                                   PduLengthType tLength, u1 tOfs );

extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxTimerInit ( CanTp_SduIdType tNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_TxTimerInit ( CanTp_SduIdType tNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxTimerHandle ( CanTp_SduIdType tNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_TxTimerHandle ( CanTp_SduIdType tNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_StartTimer ( CanTp_SduIdType tNSduId,
                                                   CanTp_TimerType tTimeId,
                                                   u4 tTimeValue,
                                                   CanTp_FrameDirType tFrameDir );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_StopTimer ( CanTp_SduIdType tNSduId,
                                                  CanTp_TimerType tTimeId,
                                                  CanTp_FrameDirType tFrameDir );
extern ECO_FUNC( u1, CANTP_CODE ) CanTp_IsTimeout ( CanTp_SduIdType tNSduId,
                                               CanTp_TimerType tTimeId,
                                               CanTp_FrameDirType tFrameDir );

extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxNPduInit ( CanTp_SduIdType tNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxNPduHandle ( CanTp_SduIdType tRxNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_Invalid ( CanTp_SduIdType tRxNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_SF ( CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_FF ( CanTp_SduIdType tRxNSduId, u2 tDL,
                                                    const PduInfoType* tPduInfoPtr );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_CF ( CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_RxLSduToNPdu_FC ( CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr );

extern ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_SF ( 
                                                    CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr );
extern ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_FF ( 
                                                    CanTp_SduIdType tRxNSduId, u2 tDL,
                                                    const PduInfoType* tPduInfoPtr );
extern ECO_FUNC( CanTp_FrameStateType, CANTP_CODE ) CanTp_RxNPduToNSdu_CF ( 
                                                    CanTp_SduIdType tRxNSduId,
                                                    const PduInfoType* tPduInfoPtr );

extern ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_LSduTransmit_FC ( 
                                                     CanTp_SduIdType tRxNSduId,
                                                     CanTp_SduIdType tTxNSduId,
                                                     const PduInfoType* tPduInfoPtr );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_LSduTransmit ( CanTp_SduIdType tTxNSduId,
                                                  const PduInfoType* tPduInfoPtr );

extern ECO_FUNC( void, CANTP_CODE ) CanTp_TxNPduInit ( CanTp_SduIdType tNSduId );
extern ECO_FUNC( void, CANTP_CODE ) CanTp_TxNPduHandle ( CanTp_SduIdType tTxNSduId );
extern ECO_FUNC( Std_ReturnType, CANTP_CODE ) CanTp_BuildFlowControl ( 
                                                     CanTp_SduIdType tRxNSduId );

extern ECO_FUNC( BufReq_ReturnType, CANTP_CODE ) CanTp_TxNSduToNPdu (
                                                    CanTp_SduIdType tTxNSduId,
                                                    const PduInfoType* tPduInfoPtr,
                                                    u1* tOfs );
#endif /* CANTP_PRIV_H */

