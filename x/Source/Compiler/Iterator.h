//
//  Iterator.h
//  x
//
//  Created by Ron Pinkas on 6/26/13.
//  Copyright (c) 2013 http://www.xHarbour.com. All rights reserved.
//

#ifndef ITERATOR_DEFINED

   #define ITERATOR_DEFINED

   extern int IterateAST( PARSER_CONTEXT *Parset_pContext );
   extern int IterateBody( BODY *pBody, PARSER_CONTEXT *Parser_pContext, int iNestedLevel );
   extern int IterateLine( LINE *pLine, PARSER_CONTEXT *Parser_pContext, int iNestedLevel );
   extern int IterateValue( VALUE *pValue, PARSER_CONTEXT *Parser_pContext, int iNestedLevel );

#endif
