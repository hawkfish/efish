#include "PhotoDocCommandAttachment.h"

class PhotoPrintCommandAttachment : public PhotoDocCommandAttachment {
	public:
				PhotoPrintCommandAttachment(CommandT inCommand, PhotoPrintDoc* inDoc);
		virtual ~PhotoPrintCommandAttachment() {};
		
		virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
													 void*				inCommandData);
		
	};//end class PhotoPrintCommandAttachment