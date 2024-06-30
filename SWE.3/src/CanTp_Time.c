
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
*   @file     CanTp_Time.c
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

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( void, CANTP_CODE ) CanTp_RxTimerInit ( CanTp_SduIdType tNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_TxTimerInit ( CanTp_SduIdType tNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_RxTimerHandle ( CanTp_SduIdType tNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_TxTimerHandle ( CanTp_SduIdType tNSduId );
ECO_FUNC( void, CANTP_CODE ) CanTp_StartTimer ( CanTp_SduIdType tNSduId,
                                            CanTp_TimerType tTimeId,
                                            u4 tTimeValue,
                                            CanTp_FrameDirType tFrameDir );
ECO_FUNC( void, CANTP_CODE ) CanTp_StopTimer ( CanTp_SduIdType tNSduId,
                                           CanTp_TimerType tTimeId,
                                           CanTp_FrameDirType tFrameDir );
ECO_FUNC( u1, CANTP_CODE ) CanTp_IsTimeout ( CanTp_SduIdType tNSduId,
                                               CanTp_TimerType tTimeId,
                                               CanTp_FrameDirType tFrameDir );
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxTimerInit
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxTimerInit ( CanTp_SduIdType tNSduId )
{
    u1 tTimerId;
    
    for ( tTimerId = CANTP_TIMER_N_Ax; tTimerId < CANTP_TIMER_MAX; tTimerId++ )
    {
        CanTp_SetRxTimeDisable( tNSduId, tTimerId );
        CanTp_SetRxTimer( tNSduId, tTimerId, 0x00U );
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxTimerInit
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_TxTimerInit ( CanTp_SduIdType tNSduId )
{
    u1 tTimerId;
    
    for ( tTimerId = CANTP_TIMER_N_Ax; tTimerId < CANTP_TIMER_MAX; tTimerId++ )
    {
        CanTp_SetTxTimeDisable( tNSduId, tTimerId );
        CanTp_SetTxTimer( tNSduId, tTimerId, 0x00U );
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_RxTimerHandle
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_RxTimerHandle ( CanTp_SduIdType tNSduId )
{
    u1 tTimerId;
    
    for ( tTimerId = CANTP_TIMER_N_Ax; tTimerId < CANTP_TIMER_MAX; tTimerId++ )
    {
        if ( ( 0x00U != CanTp_GetRxTimeFlag(tNSduId, tTimerId) )
          && ( 0x00U != CanTp_GetRxTimer(tNSduId, tTimerId)    ) )
        {
            CanTp_DecRxTimer( tNSduId, tTimerId );
        }
        else
        {
            ;
        }
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_TxTimerHandle
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_TxTimerHandle ( CanTp_SduIdType tNSduId )
{
    u1 tTimerId;

    for ( tTimerId = CANTP_TIMER_N_Ax; tTimerId < CANTP_TIMER_MAX; tTimerId++ )
    {
        if ( ( 0x00U != CanTp_GetTxTimeFlag(tNSduId, tTimerId) )
          && ( 0x00U != CanTp_GetTxTimer(tNSduId, tTimerId)    ) )
        {
            CanTp_DecTxTimer( tNSduId, tTimerId );
        }
        else
        {
            ;
        }
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_StartTimer
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_StartTimer ( CanTp_SduIdType tNSduId,
                                                CanTp_TimerType tTimeId,
                                                u4 tTimeValue,
                                                CanTp_FrameDirType tFrameDir )
{
    switch ( tFrameDir )
    {
      case CANTP_SEND:
          CanTp_SetTxTimeEnbale( tNSduId, tTimeId );
          CanTp_SetTxTimer( tNSduId, tTimeId, tTimeValue );
        break;

      case CANTP_RECIVED:
          CanTp_SetRxTimeEnbale( tNSduId, tTimeId );
          CanTp_SetRxTimer( tNSduId, tTimeId, tTimeValue );
        break;

      default:
          /* Do nothing */
        break;
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_StopTimer
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_StopTimer ( CanTp_SduIdType tNSduId,
                                               CanTp_TimerType tTimeId,
                                               CanTp_FrameDirType tFrameDir )
{
    switch ( tFrameDir )
    {
      case CANTP_SEND:
          CanTp_SetRxTimeDisable( tNSduId, tTimeId );
        break;

      case CANTP_RECIVED:
          CanTp_SetRxTimeDisable( tNSduId, tTimeId );
        break;

      default:
          /* Do nothing */
        break;
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_IsTimeout
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( u1, CANTP_CODE ) CanTp_IsTimeout ( CanTp_SduIdType tNSduId,
                                               CanTp_TimerType tTimeId,
                                               CanTp_FrameDirType tFrameDir )
{
    u1 tRetVal = FALSE;

    if ( CANTP_SEND == tFrameDir )
    {
        if ( ( 0x00U != CanTp_GetTxTimeFlag(tNSduId, tTimeId) )
          && ( 0x00U == CanTp_GetTxTimer(tNSduId, tTimeId)    ) )
        {
            tRetVal = TRUE;
        }
        else
        {
            tRetVal = FALSE;
        }
    }
    else
    {
        if ( ( 0x00U != CanTp_GetRxTimeFlag(tNSduId, tTimeId) )
          && ( 0x00U == CanTp_GetRxTimer(tNSduId, tTimeId)    ) )
        {
            tRetVal = TRUE;
        }
        else
        {
            tRetVal = FALSE;
        }
    }
    
    return tRetVal;
}
/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
