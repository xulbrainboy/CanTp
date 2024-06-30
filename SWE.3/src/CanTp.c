
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
*   @file     CanTp.c
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
#include "CanTp_Public.h"
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
ECO_FUNC( void, CANTP_CODE ) CanTp_Init( const CanTp_ConfigType *tCfgPtr );
ECO_FUNC( void, CANTP_CODE ) CanTp_MainFunction( void );
ECO_FUNC( void, CANTP_CODE ) CanTp_Shutdown(void);
/*-----------------------------------------------------------------------------*
 * -LOCAL FUNCTIONS
 *-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_Init
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_Init(const CanTp_ConfigType *tCfgPtr)
{
    CanTp_SduIdType tNSduId;

    (void)tCfgPtr;

    CanTp_SetRuntimeState( CANTP_ON );

    /* Rx Frame Init */
    for ( tNSduId = 0U; tNSduId < CANTP_RXSDU_NUM; tNSduId++ )
    {
        CanTp_RxNPduInit( tNSduId );
    }
    
    /* Tx Frame Init */
    for ( tNSduId = 0U; tNSduId < CANTP_TXSDU_NUM; tNSduId++ )
    {
        CanTp_TxNPduInit( tNSduId );
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_MainFunction
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_MainFunction(void)
{
    CanTp_SduIdType tNSduId;

    /* Check the Module runing or not */
    if ( CANTP_OFF == CanTp_GetRuntimeState() )
    {
        /* Report error */
    }
    else
    {
        /* Rx Frame Check */
        for ( tNSduId = 0U; tNSduId < CANTP_RXSDU_NUM; tNSduId++ )
        {
            /* Timer handle */
            CanTp_RxTimerHandle( tNSduId );
            /* Frame Handle */
            CanTp_RxNPduHandle( tNSduId );
        }
        
        /* Tx Frame Check */
        for ( tNSduId = 0U; tNSduId < CANTP_TXSDU_NUM; tNSduId++ )
        {
            /* Timer Handle */
            CanTp_TxTimerHandle( tNSduId );
            /* Frame Handle */
            CanTp_TxNPduHandle( tNSduId );
        }
    }
}

/*-----------------------------------------------------------------------------*/
/** - Service Name : CanTp_MainFunction
  * - Sync/Async   : Synchronous
  * - Param(in)    : TODO
  * - Return value : TODO
  * - Description  : TODO
  * - Requirement  : TODO
  */
ECO_FUNC( void, CANTP_CODE ) CanTp_Shutdown(void)
{
    if ( CANTP_OFF != CanTp_GetRuntimeState() )
    {
        CanTp_SetRuntimeState( CANTP_OFF );
    }
    else
    {
        /* report error */
    }
}

/*-----------------------------------------------------------------------------*
 * -END OF FILE
 *-----------------------------------------------------------------------------*/
