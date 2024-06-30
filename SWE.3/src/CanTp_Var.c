
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
*   @file     CanTp_Var.c
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
ECO_VAR( CanTp_RuntimeType, CANTP_VAR ) CanTp_RTState;
ECO_VAR( CanTp_RxRtConnInfoType, CANTP_VAR ) CanTp_RxRtConnSdu[CANTP_RXSDU_NUM];
ECO_VAR( CanTp_TxRtConnInfoType, CANTP_VAR ) CanTp_TxRtConnSdu[CANTP_TXSDU_NUM];
/*-----------------------------------------------------------------------------*
 * -LOCAL VARIABLE DECLARATIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTION PROTOTYPES
 *-----------------------------------------------------------------------------*/
ECO_FUNC( u1, CANTP_CODE ) CanTp_GetBufferOffset_Af ( CanTp_AddrFormatType tAddrFormat );

/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------*
 * -GLOBAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_GetBufferOffset
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( u1, CANTP_CODE ) CanTp_GetBufferOffset_Af ( CanTp_AddrFormatType tAddrFormat )
{
    u1 tOfs;

    if ( ( tAddrFormat == CANTP_STANDARD    )
      || ( tAddrFormat == CANTP_NORMALFIXED ) ) /* Standard or Normal Fixed */
    {
        tOfs = 0U;
    }
    else /* Extend or Mixed 11bit or Mixed 29 bit */
    {
        tOfs = 1U;
    }

    return tOfs;
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_GetMatchingDL
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( u1, CANTP_CODE ) CanTp_GetMatchingDL ( CanTp_CanType tCanType, PduLengthType tLength, u1 tOfs )
{
    u1 tRetVal = 0U;

    if ( ( CANTP_CAN_2_0 == tCanType          )
      || ( CANTP_CAN_SMALL_CAN_FD == tCanType ) )
    {
        tRetVal = CANTP_CAN2_0_SMALLFD_MAXSIZE;
    }
    else
    {
        u1 tId;
        u1 tDL[8U] = { 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U };
        PduLengthType tSize = (PduLengthType)( tLength + tOfs );

        for ( tId = 0U; tId < 8U; tId++ )
        {
            if ( tSize <= tDL[tId] )
            {
                tRetVal = tDL[tId];
                break;
            }
        }
        if ( tId >= 8U )
        {
            tRetVal = tDL[7U];
        }
        else
        {
            ;
        }
    }
    
    return tRetVal;
}
/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
