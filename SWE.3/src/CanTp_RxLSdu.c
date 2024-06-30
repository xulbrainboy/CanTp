
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
*   @file     CanTp_RxLSdu.c
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
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_GetRxNSduIdFromLSdu (     \
                                                     PduIdType tRxPduId,    \
                                                     CanTp_SduIdType* tRxNSduIdPtr );

ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_RxLSdu_PCITypeHandle (               \
                                                  CanTp_SduIdType tRxNSduId, \
                                                  u1 tN_PCI,                 \
                                                  u1 tOfs,                   \
                                                  const PduInfoType* tPduInfoPtr );

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( void, CANTP_CODE ) CanTp_RxIndication ( PduIdType tRxPduId, \
                                                  const PduInfoType* tPduInfoPtr );
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_GetRxNSduIdFromLSdu
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - SWRS         : TODO
  */
ECO_LOCALFUNC( Std_ReturnType, CANTP_CODE ) CanTp_GetRxNSduIdFromLSdu (     \
                                                     PduIdType tRxPduId,    \
                                                     CanTp_SduIdType* tRxNSduIdPtr )
{
    Std_ReturnType  tRetVal;
    CanTp_SduIdType tRxNSduId;

    tRetVal = E_NOT_OK;
    for ( tRxNSduId = 0U; tRxNSduId < CANTP_RXSDU_NUM; tRxNSduId++ )
    {
        if ( tRxPduId == CanTp_GetCfg_RxCanIfId(tRxNSduId) )
        {
            tRetVal = E_OK;
            break;
        }
    }

    *tRxNSduIdPtr = (CanTp_SduIdType)( tRxNSduId - 1U );

    return tRetVal;
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_GetRxNSduIdFromLSdu
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - SWRS         : SWS_CanTp_00350
  */
ECO_LOCALFUNC( void, CANTP_CODE ) CanTp_RxLSdu_PCITypeHandle (               \
                                                  CanTp_SduIdType tRxNSduId, \
                                                  u1 tN_PCI,                 \
                                                  u1 tOfs,                   \
                                                  const PduInfoType* tPduInfoPtr )
{
    PduInfoType tRxPduinfo;
    u2 tDL;

    switch ( tN_PCI )
    {
      case CANTP_SINGLE_FRAME:
          /* Check CAN FD the Offset need to plus one */
          if ( ( CANTP_CAN_FD == CanTp_GetCfgRx_CanType(tRxNSduId)     )
            && ( tPduInfoPtr->SduLength > CANTP_CAN2_0_SMALLFD_MAXSIZE ) )
          {
              tOfs++;
          }
          /* Get data length */
          tDL = (u2)( tPduInfoPtr->SduDataPtr[tOfs] );
          /* Get the data start index */
          tOfs++;
          tRxPduinfo.SduDataPtr = &tPduInfoPtr->SduDataPtr[tOfs];
          tRxPduinfo.SduLength = tDL;
          CanTp_RxLSduToNPdu_SF( tRxNSduId, &tRxPduinfo );
        break;

      case CANTP_FIRST_FRAME:
          /* Check Physical Address or not */
          if ( CANTP_PHYSICAL == CanTp_GetCfgRx_TaType(tRxNSduId) )
          {
              /* Get data length */
              tDL = ( (u2)( tPduInfoPtr->SduDataPtr[tOfs] & 0x0FU ) << 8U );
              tOfs++;
              tDL |= (u2)( tPduInfoPtr->SduDataPtr[tOfs] );
              /* Get the data start index */
              tOfs++;
              tRxPduinfo.SduDataPtr = &tPduInfoPtr->SduDataPtr[tOfs];
              tRxPduinfo.SduLength = ( tPduInfoPtr->SduLength - tOfs );
              CanTp_RxLSduToNPdu_FF( tRxNSduId, tDL, &tRxPduinfo );
          }
          else
          {
              CanTp_RxLSduToNPdu_Invalid(tRxNSduId);
          }
        break;

      case CANTP_CONSECUTIVE_FRAME:
          /* Check Sn match or not */
          if ( (tPduInfoPtr->SduDataPtr[tOfs] & 0x0FU) == ((CanTp_GetRxFrameSn(tRxNSduId) + 0x01U) & 0x0FU) )
          {
              /* Get the data start index */
              tOfs++;
              tRxPduinfo.SduDataPtr = &tPduInfoPtr->SduDataPtr[tOfs];
              tRxPduinfo.SduLength = ( tPduInfoPtr->SduLength - tOfs );
              CanTp_RxLSduToNPdu_CF( tRxNSduId, &tRxPduinfo );
          }
          else
          {
              CanTp_RxLSduToNPdu_Invalid(tRxNSduId);
          }
        break;

      case CANTP_FLOWCONTROL_FRAME:
          /* Check Physical Address or not */
          if ( CANTP_PHYSICAL == CanTp_GetCfgRx_TaType(tRxNSduId) )
          {
              CanTp_RxLSduToNPdu_FC( tRxNSduId, tPduInfoPtr );
          }
          else
          {
              CanTp_RxLSduToNPdu_Invalid(tRxNSduId);
          }
        break;

      default:
          /* Frame Invalid */
          CanTp_RxLSduToNPdu_Invalid(tRxNSduId);
        break;
    }
}

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxIndication
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - SWRS         : SWS_CanTp_00345
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxIndication ( PduIdType tRxPduId, \
                                                  const PduInfoType* tPduInfoPtr )
{
    CanTp_SduIdType tRxNSduId;
    u1 tOfs;
    u1 tN_PCI;

    /* Check the Module runing or not */
    if ( CANTP_OFF == CanTp_GetRuntimeState() )
    {
        /* Report error */
    }
    else if ( NULL_PTR == tPduInfoPtr )
    {
        /* Report error */
    }
    /* Step 1: Get RxNSduId from CanIf PduId */
    else if ( E_NOT_OK == CanTp_GetRxNSduIdFromLSdu( tRxPduId, &tRxNSduId ) )
    {
        /* Report error */
    }
    /* Step 2: Check DL based on padding configuration */
    else if ( ( CANTP_PADDING_ON == CanTp_GetCfgRx_IsPadding(tRxNSduId)             )
           && ( ( tPduInfoPtr->SduLength < CANTP_CAN2_0_SMALLFD_MAXSIZE           )
             && ( ( CANTP_CAN_2_0 == CanTp_GetCfgRx_CanType(tRxNSduId)          )
               || ( CANTP_CAN_SMALL_CAN_FD == CanTp_GetCfgRx_CanType(tRxNSduId) ) ) ) )
    {
        /* Reject the reception */
    }
    else
    {
        /* Step 3: Get the Buffer Offset Based On Addressing Format Type */
        tOfs = CanTp_GetBufferOffset_Af( CanTp_GetCfgRx_AddrFormat(tRxNSduId) );
        /* Step 4: Get the N-PCI */
        tN_PCI = ( tPduInfoPtr->SduDataPtr[tOfs] & CANTP_NPCI_FIEILDMASK );
        /* Step 5: Different Frames Handle */
        CanTp_RxLSdu_PCITypeHandle( tRxNSduId, tN_PCI, tOfs, tPduInfoPtr );
    }
}

/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
