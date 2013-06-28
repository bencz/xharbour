/*  
 * ClipNet Project source code:
 * Parser main file.
 *
 * Copyright 2001 Ron Pinkas <ronpinkas@profit-master.com>
 * www - http://www.RonPinkas.com
 *
 */

#include "common.h"

PARSER_CONTEXT * New_Context()
{
   PARSER_CONTEXT *pContext = NEW( PARSER_CONTEXT );
   
   ZERO( pContext );
   
   return pContext;
}

ID * New_ID( char *sName, PARSER_CONTEXT *Parser_pContext )
{ 
   // TODO add search and reuse logic. 
   ID *pID = NEW( ID ); 

   strcpy( pID->Name, sName );

   return pID; 
}

FUNCTION * New_Function( char *sFunc, FUNC_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{
   ID *pID = New_ID( sFunc, Parser_pContext );
   FUNCTION *pFunc = NEW( FUNCTION );

   ZERO( pFunc );

   pFunc->pName = pID;
   pFunc->Kind  = Kind;

   if( Parser_pContext->Parser_Functions.iFunctions == 0 )
   {
      Parser_pContext->Parser_Functions.pFirst = pFunc;
      Parser_pContext->Parser_Functions.pLast  = pFunc;
   }
   else
   {
      Parser_pContext->Parser_Functions.pLast->pNext = pFunc;
      Parser_pContext->Parser_Functions.pLast = pFunc;
   }

   Parser_pContext->Parser_Functions.iFunctions++;

   return pFunc;
}

BODY * New_Body( PARSER_CONTEXT *Parser_pContext )
{
   BODY *pBody = NEW( BODY );
 
   pBody->pLines = NULL;
 
   return pBody;
}

LINE * New_Line( void *x, LINE_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{
   LINE *pLine = NEW( LINE ); 

   pLine->Kind = Kind;
   pLine->iNo  = Parser_pContext->Parser_iLine;
   
   pLine->pNext = NULL;

   switch( Kind ) 
   {
      case LINE_KIND_ASSIGNMENT : 
         //pLine->Value.pAssignment = ReduceValue[ ( (VALUE *) x )->Kind ]( (VALUE *) x, Parser_pContext );
         pLine->Value.pAssignment = (VALUE *) x;
         break;

      case LINE_KIND_CASE : 
         pLine->Value.pCase = (CASE *) x; 
         break;

      case LINE_KIND_OTHERWISE : 
         pLine->Value.pOtherwise = (OTHERWISE *) x; 
         break;


      case LINE_KIND_FOR : 
         pLine->Value.pFor = (FOR *) x; 
         break;

      case LINE_KIND_WHILE : 
         pLine->Value.pWhile = (WHILE *) x; 
         break;

      case LINE_KIND_FLOW : 
         pLine->Value.Flow = (FLOW_KIND) (int) x; 
         break;


      case LINE_KIND_FUNC_CALL : 
         //pLine->Value.pAssignment = ReduceValue[ ( (VALUE *) x )->Kind ]( (VALUE *) x, Parser_pContext );
         pLine->Value.pAssignment = (VALUE *) x;
         break;

      case LINE_KIND_IIF : 
         //pLine->Value.pAssignment = ReduceValue[ ( (VALUE *) x )->Kind ]( (VALUE *) x, Parser_pContext );
         pLine->Value.pAssignment = (VALUE *) x;
         break;

      case LINE_KIND_METHOD_CALL : 
         //pLine->Value.pAssignment = ReduceValue[ ( (VALUE *) x )->Kind ]( (VALUE *) x, Parser_pContext );
         pLine->Value.pAssignment = (VALUE *) x;
         break;


      case LINE_KIND_IF : 
         pLine->Value.pIf = (IF *) x; 
         break;

      case LINE_KIND_ELSEIF : 
         pLine->Value.pElseIf = (ELSEIF *) x; 
         break;

      case LINE_KIND_ELSE : 
         pLine->Value.pElse = (ELSE *) x; 
         break;


      case LINE_KIND_PARAMETERS : 
         pLine->Value.pParamters = (DECLARED *) x;
         break;

      case LINE_KIND_PRIVATES : 
         pLine->Value.pPrivates = (DECLARED *) x;
         break;

      case LINE_KIND_PUBLICS : 
         pLine->Value.pPublics = (DECLARED *) x;
         break;


      case LINE_KIND_RETURN : 
         pLine->Value.pReturn = (VALUE *) x; 
         break;


      case LINE_KIND_SEQUENCE : 
         pLine->Value.pSequence = (SEQUENCE *) x; 
         break;

      case LINE_KIND_RECOVER : 
         pLine->Value.pRecover = (RECOVER *) x; 
         break;


      case LINE_KIND_SWITCH :
         pLine->Value.pSwitch = (SWITCH *) x; 
         break;

      case LINE_KIND_SWITCHCASE :
         pLine->Value.pSwitchCase = (SWITCHCASE *) x; 
         break;

      case LINE_KIND_SWITCHDEAFULT :
         pLine->Value.pSwitchDefault = (SWITCHDEFAULT *) x; 
         break;


      case LINE_KIND_TRY :
         pLine->Value.pTry = (TRY *) x; 
         break;

      case LINE_KIND_CATCH :
         pLine->Value.pCatch = (CATCH *) x; 
         break;

      case LINE_KIND_FINALLY :
         pLine->Value.pFinally = (BODY *) x; 
         break;

      case LINE_KIND_UNARY : 
         //pLine->Value.pUnary = ReduceValue[ ( (VALUE *) x )->Kind ]( (VALUE *) x, Parser_pContext );
         pLine->Value.pUnary = (VALUE *) x;
         break;

       default:
        printf( "Kind: %i\n", Kind );
        PARSE_ERROR( PARSER_ERR_SYNTAX, yytext, ", internal error - unexpected case in New_Line(): " __SOURCE__ );
   } 

   return pLine;
}

INLINE * New_Inline( char *sName, PARSER_CONTEXT *Parser_pContext )
{
   INLINE *pInline = NEW( INLINE );

   pInline->sName      = sName;
   pInline->pCode      = NULL;
   pInline->lPCodeSize = 0;
   pInline->pNext      = NULL;
   pInline->sFileName  = ClipNet_strdup( Parser_pContext->Parser_Files.pLast->sName );
   pInline->iLine      = Parser_pContext->Parser_iLine - 1;

   return pInline;
}

VALUE * New_Value( void *x, VALUE_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{
   VALUE *pValue = NEW( VALUE );

   pValue->Kind = Kind;

   switch( Kind )
   {
      case VALUE_KIND_NIL : 
         pValue->Type = PRG_TYPE_NIL;
         break;

      case VALUE_KIND_CONSTANT : 
         pValue->Value.pConstant = (CONSTANT *) x; 
         pValue->Type = pValue->Value.pConstant->Type;
         break;
 
      case VALUE_KIND_LVALUE : 
         pValue->Value.pLValue = (LVALUE *) x;  
         pValue->Type = pValue->Value.pLValue->Type;
         break;

      case VALUE_KIND_ARRAY : 
         pValue->Value.pArray = (LIST *) x;   
         pValue->Type = pValue->Value.pArray->Type;
         break;

      case VALUE_KIND_BLOCK : 
         pValue->Value.pBlock = (BLOCK *) x;   
         pValue->Type = PRG_TYPE_BLOCK;
         break;
 
      case VALUE_KIND_UNARY : 
         pValue->Value.pUnary = (UNARY *) x;
         pValue->Type = pValue->Value.pUnary->Type;
         break;         
 
      case VALUE_KIND_BINARY : 
         pValue->Value.pBinary = (BINARY *) x;   
         pValue->Type = pValue->Value.pBinary->Type;
         break;
 
      case VALUE_KIND_ALIASED : 
         pValue->Value.pAliased = (ALIASED *) x;   
         pValue->Type = pValue->Value.pAliased->Type;
         break;
 
      case VALUE_KIND_ASSIGNMENT : 
         pValue->Value.pAssignment = (ASSIGNMENT *) x;   
         pValue->Type = pValue->Value.pAssignment->Type;
         break;
 
      case VALUE_KIND_FUNC_CALL : 
         pValue->Value.pFuncCall = (FUNC_CALL *) x;   
         pValue->Type = pValue->Value.pFuncCall->Type;
         break;
 
      case VALUE_KIND_IIF : 
         pValue->Value.pIIF = (IIF *) x;   
         pValue->Type = pValue->Value.pIIF->Type;
         break;
  
      case VALUE_KIND_METHOD_CALL : 
         pValue->Value.pMethodCall = (METHOD_CALL *) x;    
         pValue->Type = pValue->Value.pMethodCall->Type;
         break;

      case VALUE_KIND_LIST : 
         pValue->Value.pList = (LIST *) x;
         pValue->Type = pValue->Value.pList->Type;
         break;
           
       case VALUE_KIND_BYREF :
           //TODO pValue->Value. = (BYREF *) x;
           //TODO pValue->Type = pValue->Value.pByRef->Type;
           PARSE_ERROR( PARSER_ERR_SYNTAX, yytext, ", internal error - unexpected case in New_Value()." );
           break;
   }

   return pValue;    
}

VALUE * New_NIL( PARSER_CONTEXT *Parser_pContext )
{
   return New_Value( NULL, VALUE_KIND_NIL, Parser_pContext );
}

VALUE * New_Constant( CONSTANT * pConstant, PARSER_CONTEXT *Parser_pContext )
{
   CONSTANT *pCopy = NEW( CONSTANT);

   memcpy( (void *) pCopy, (void *) pConstant, sizeof( CONSTANT ) );

   switch( pCopy->Kind )
   {
      case CONSTANT_KIND_INTEGER :
      case CONSTANT_KIND_LONG :
      case CONSTANT_KIND_DOUBLE :
        pCopy->Type = PRG_TYPE_NUMERIC;
        break;

      case CONSTANT_KIND_STRING :
        pCopy->Type = PRG_TYPE_STRING;
        break;

      case CONSTANT_KIND_BOOL:
        pCopy->Type = PRG_TYPE_LOGICAL;
        break;

      case CONSTANT_KIND_DATE :
        pCopy->Type = PRG_TYPE_DATE;
        break;

      default :
        printf( "Kind: %i\n", pCopy->Kind );
        PARSE_ERROR( PARSER_ERR_SYNTAX, yytext, ", internal error - unexpected case in NewConstant()." );
   }

   return New_Value( (void *) pCopy, VALUE_KIND_CONSTANT, Parser_pContext );
}

VALUE * New_Block( PARSER_CONTEXT *Parser_pContext )
{ 
   BLOCK *pBlock = NEW( BLOCK );
    
   pBlock->pBlockLocals = NULL;
   pBlock->iBlockLocals = 0;
   pBlock->pList        = NULL;

   return New_Value( (void *) pBlock, VALUE_KIND_BLOCK, Parser_pContext );
} 

VALUE * New_Unary( VALUE * pValue, UNARY_KIND Kind, UNARY_WHEN When, PARSER_CONTEXT *Parser_pContext )
{
   UNARY *pUnary;

   if( pValue == NULL || pValue->Kind != VALUE_KIND_LVALUE )
   {
      PARSE_ERROR( PARSER_ERR_INVALID_LVALUE, yytext, "unary operation." );
      return NULL;
   }

   pUnary = NEW( UNARY );
   
   pUnary->Kind = Kind;
   pUnary->When = When;
   pUnary->pLValue = pValue;

   return New_Value( (void *) pUnary, VALUE_KIND_UNARY, Parser_pContext );
}

VALUE * New_LValue( void *x, LVALUE_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{
   LVALUE *pLValue = NEW( LVALUE );

   pLValue->Kind = Kind;
   pLValue->Type = PRG_TYPE_UNDEF;
   
   switch( Kind )
   {
      case LVALUE_KIND_VARIABLE:
         pLValue->Value.pVariable = (DECLARED *) x;
         break;

      case LVALUE_KIND_MACRO:
         pLValue->Value.pMacro = (MACRO *) x;
         break;

      case LVALUE_KIND_ARRAY_ELEMENT:
         pLValue->Value.pArrayElement = (ARRAY_ELEMENT *) x;
         break;

      case LVALUE_KIND_OBJ_PROPERTY:
         pLValue->Value.pProperty = (OBJ_PROPERTY *) x;
         break;

      case LVALUE_KIND_ALIASED_FIELD:
         pLValue->Value.pAliasedField = (ALIASED_FIELD *) x;
         break;

      default :
         PARSE_ERROR( PARSER_ERR_SYNTAX, yytext, ", internal error - unexpected type in New_LValue()" );
   }

   return New_Value( (void *) pLValue, VALUE_KIND_LVALUE, Parser_pContext );
}

VALUE * New_LValueID( char *sName, DECLARED_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{
   DECLARED *pDeclared = New_DeclaredID( sName, Kind ,Parser_pContext );
   
   return New_LValue( (void *) pDeclared, LVALUE_KIND_VARIABLE, Parser_pContext );
}

VALUE * New_Binary( VALUE *pLeft, VALUE *pRight, BINARY_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{
   BINARY *pBinary = NEW( BINARY );
   VALUE  *pValue;

   //printf( "Binary: %i, Left: %i Right: %i\n", Kind, pLeft->Kind, pRight->Kind );

   pBinary->Kind = Kind;
   pBinary->pLeft = pLeft;
   pBinary->pRight = pRight;

   pValue = New_Value( (void *) pBinary, VALUE_KIND_BINARY, Parser_pContext );

   #define DEBUG_BINARY

   if( pRight->Kind == VALUE_KIND_BINARY )
   {
      #ifdef DEBUG_BINARY
         printf( "Comparing: %i(%i), %i(%i)\n", Kind, BinaryPrecedence[ Kind ], pRight->Value.pBinary->Kind, BinaryPrecedence[ pRight->Value.pBinary->Kind ] );
      #endif

      if( BinaryPrecedence[ Kind ] > BinaryPrecedence[ pRight->Value.pBinary->Kind ] )
      {
         #ifdef DEBUG_BINARY
            printf( "Fliping \n" );
         #endif

         pBinary->pRight = pRight->Value.pBinary->pLeft;
         pRight->Value.pBinary->pLeft = pValue;

         pValue = pRight;
      }
   }

   return pValue;
}

VALUE * New_Macro( void *x, MACRO_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{
   MACRO *pMacro = NEW( MACRO );

   pMacro->Kind = Kind;

   switch( Kind )
   {
      case MACRO_KIND_SIMPLE:
         pMacro->Value.pID = New_ID( (char *) x, Parser_pContext );
         break;

      case MACRO_KIND_COMPLEX:
         pMacro->Value.pComplex = (VALUE *) x;
         break;

      default :
         PARSE_ERROR( PARSER_ERR_SYNTAX, yytext, ", internal error - unexpected type in: " __SOURCE__ );
   }

   return New_LValue( (void *) pMacro, LVALUE_KIND_MACRO, Parser_pContext );
}

VALUE * New_Aliased( VALUE * pArea, VALUE * pValue, PARSER_CONTEXT *Parser_pContext )
{
   ALIASED *pAliased = NEW( ALIASED );

   pAliased->pArea  = pArea;
   pAliased->pValue = pValue;

   return New_Value( (void *) pAliased, VALUE_KIND_ALIASED, Parser_pContext );
}

VALUE * New_IIF( VALUE * pCond, VALUE * pTrue, VALUE * pFalse, PARSER_CONTEXT *Parser_pContext )
{
   IIF *pIIF = NEW( IIF );

   pIIF->pCond = pCond;
   pIIF->pTrue = pTrue;
   pIIF->pFalse = pFalse;

   return New_Value( (void *) pIIF, VALUE_KIND_IIF, Parser_pContext );
} 

VALUE * New_Assignment( VALUE * pLValue, VALUE * pValue, ASSIGNMENT_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{ 
   ASSIGNMENT *pAssignment = NEW( ASSIGNMENT );
    
   ASSERT( pLValue );
   ASSERT( pValue );

   pAssignment->Kind = Kind;

   pAssignment->pLValue = pLValue;
   pAssignment->Type = pValue->Type;
   
   pAssignment->pValue  = pValue;
   pAssignment->Type = pValue->Type;
   
   if( pLValue->Kind != VALUE_KIND_LVALUE )
   {
      PARSE_ERROR( PARSER_ERR_INVALID_LVALUE, yytext, "invalid assignment receiver." );
   }

   return New_Value( (void *) pAssignment, VALUE_KIND_ASSIGNMENT, Parser_pContext );
} 
 
DECLARED * New_Declared( PARSER_CONTEXT *Parser_pContext ) 
{ 
   DECLARED *pDeclared = NEW( DECLARED );

   pDeclared->Kind  = DECLARED_KIND_NONE;
   pDeclared->pID   = NULL;
   pDeclared->pInit = NULL;
 
   return pDeclared; 
}

DECLARED * New_DeclaredID( char *sName, DECLARED_KIND Kind, PARSER_CONTEXT *Parser_pContext )
{ 
   DECLARED *pDeclared = NEW( DECLARED );

   pDeclared->Kind  = Kind;
   pDeclared->pID   = New_ID( sName, Parser_pContext );
   pDeclared->pInit = NULL;
 
   return pDeclared; 
}

LINE * New_If( VALUE * pCondExp, BODY *pBody, PARSER_CONTEXT *Parser_pContext )
{
   IF *pIf = NEW( IF );
   
   pIf->pCondExp = pCondExp;
   pIf->pBody    = pBody;
   pIf->pElseIf  = NULL;
   pIf->pElse    = NULL;

   return New_Line( (void *) pIf, LINE_KIND_IF, Parser_pContext );
}

LINE * New_ElseIf( VALUE * pCondExp, BODY *pBody, PARSER_CONTEXT *Parser_pContext )
{
   ELSEIF *pElseIf = NEW( ELSEIF );

   pElseIf->pCondExp = pCondExp;
   pElseIf->pBody    = pBody;
   pElseIf->pNext    = NULL;

   return New_Line( (void *) pElseIf, LINE_KIND_ELSEIF, Parser_pContext );
}

LINE * New_Else( BODY *pBody, PARSER_CONTEXT *Parser_pContext )
{
   ELSE *pElse = NEW( ELSE );

   pElse->pBody = pBody;

   return New_Line( (void *) pElse, LINE_KIND_ELSE, Parser_pContext );
}

LIST * New_ListNode( LIST * pList, VALUE * pValue, PARSER_CONTEXT *Parser_pContext )
{
   LIST_NODE *pListNode = NEW( LIST_NODE );

   if( pValue == NULL )
   {
      PARSE_ERROR( PARSER_ERR_SYNTAX, yytext, "NULL Node in:" __SOURCE__ );
   }

   pListNode->pValue = pValue;
   pListNode->pNext = NULL;

   if( pList->iNodes )
   {
      pList->iNodes++;
      pList->pLast->pNext = pListNode;
      pList->pLast = pListNode;
   }
   else
   {
      pList->iNodes = 1;
      pList->pFirst = pListNode;
      pList->pLast  = pListNode;
   }

   return pList;
}   

LIST * New_List( VALUE * pValue, PARSER_CONTEXT *Parser_pContext )
{
   LIST *pList = NEW( LIST );

   pList->iNodes = 0;
   pList->pFirst = NULL;
   pList->pLast  = NULL;

   if( pValue )
   {
      New_ListNode( pList, pValue, Parser_pContext );
   }

   return pList;
}

VALUE * New_FuncCall( VALUE * pSymVal, VALUE * pArgList, PARSER_CONTEXT *Parser_pContext )
{
   FUNC_CALL *pFuncCall = NEW( FUNC_CALL );

   pFuncCall->pSymbol = pSymVal ;
   pFuncCall->pArguments = pArgList;
   pFuncCall->Type = PRG_TYPE_UNDEF;

   return New_Value( (void *) pFuncCall, VALUE_KIND_FUNC_CALL, Parser_pContext );
}

VALUE * New_ArrayElement( VALUE * pArray, VALUE * pIndexList, PARSER_CONTEXT *Parser_pContext )
{
   ARRAY_ELEMENT *pArrayElement = NEW( ARRAY_ELEMENT );

   if( pIndexList == NULL )
   {
      PARSE_ERROR( PARSER_ERR_SYNTAX, yytext, "invalid array index." );
   }

   pArrayElement->pArray = pArray;
   pArrayElement->pIndexList = pIndexList;

   return New_LValue( (void *) pArrayElement, LVALUE_KIND_ARRAY_ELEMENT, Parser_pContext );
}
