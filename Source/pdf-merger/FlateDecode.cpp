/*
 * Copyright (C) 2015-2018 Département de l'Instruction Publique (DIP-SEM)
 *
 * Copyright (C) 2013 Open Education Foundation
 *
 * Copyright (C) 2010-2013 Groupement d'Intérêt Public pour
 * l'Education Numérique en Afrique (GIP ENA)
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */




#include <iostream>

#include "FlateDecode.h"
//#include "zlib.h"
#include "Utils.h"
#include <string.h>

#include "core/memcheck.h"

using namespace merge_lib;
#define ZLIB_MEM_DELTA 65535
#define ZLIB_CHECK_ERR(err,msg) \
   if( err != Z_OK) {\
   std::cout<<msg<<" ZLIB error:"<<err<<std::endl; \
   }\

FlateDecode::FlateDecode():_predict(NULL)
{
}

FlateDecode::~FlateDecode()
{
   if( _predict )
   {
      delete _predict;
   }
}

void FlateDecode::initialize(Object * objectWithStream)
{
   if( objectWithStream )
   {
      std::string head;
      objectWithStream->getHeader(head);

      if((int) head.find(FilterPredictor::DECODE_PARAM_TOKEN)  != -1 )
      {
         _predict = new FilterPredictor();
         _predict->initialize(objectWithStream);
      }

   }
}

/** @brief encode
*
* @todo:
document this function
*/
bool FlateDecode::encode(std::string &decoded)
{   

   return true;
}

/** @brief decode
*
* @todo: document this function
*/
bool FlateDecode::decode(std::string & encoded)
{


   return true;
}

