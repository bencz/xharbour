#include "vxh.ch"
//---------------------------------------- End of system code ----------------------------------------//

//------------------------------------------------------------------------------------------------------------------------------------

CLASS __Organigram INHERIT Application
   // Components declaration
   METHOD Init() CONSTRUCTOR

   // Event declaration
ENDCLASS

METHOD Init( oParent, aParameters ) CLASS __Organigram
   ::Super:Init( oParent, aParameters )


   // Populate Components
   // Properties declaration
   ::Resources            := {  }
   ::SetDeleted           := .T.
   ::SetDateFormat        := "mm-dd-yyyy"

   ::Create()

   // Populate Children
RETURN Self

