#pragma once

/*	---------------------------------------------------------------------------------------------
	class ABalloon
	
	Author:		James W. Walker <mailto:jw@jwwalker.com>
				<http://www.jwwalker.com/>
	
	Purpose:	PowerPlant attachment class to provide balloon help
				under Mac OS 7/8/9, and Carbon Help (AKA help tags)
				under Carbon.
			
	Usage:		Attach an ABalloon to each pane (usually a control)
				for which you want to provide help.
				Specify a STR# ID and a string index for each state
				(on or off, enabled or disabled).
				And remember to register ABalloon at startup.
				You must compile with RTTI for proper function.
				If you want a balloon to pop up automatically when
				the mouse is motionless over a pane for a certain time,
				you must call EnableAutoPop().
				If you want the help to appear without delay when the
				control key is pressed, you must call EnableControlKeyPop().

	Copyright:	ABalloon is ©1998, 2000 by James W. Walker.
				Permission is granted for use of ABalloon
				free of charge, other than acknowledgement of
				James W. Walker in any program using ABalloon
				(perhaps in an About box or in accompanying
				documentation). This copyright notice must remain
				in the source even if you modify it.
				If you contribute improvements which I incorporate
				into ABalloon, I will acknowledge your contribution,
				but I will retain sole copyright.
	
	Version:	2.0b5 12/10/2000
				See source for change notes.
	---------------------------------------------------------------------------------------------
*/
#include <LAttachment.h>

class LStream;

struct XBalloonImp;

#pragma mark class ABalloonBase
// Abstract base class for a balloon attachment.
class ABalloonBase : public LAttachment
{
public:
					ABalloonBase(
							LStream			*inStream);
					
	virtual			~ABalloonBase();
	
	// Pure virtual method, concrete classes must override
	virtual	void	GetHelpString(
							LPane*			inPane,
							StringPtr		outString ) = 0;

	// Accessors
	bool			GetAutomatic() const;
	void			SetAutomatic( bool inAutoMode );
	
	// Static methods
	static void		EnableAutoPop( void );
	static void		DisableAutoPop( void );
	static void		SetAutoPopDelay( UInt32 inDelayTicks );

	static void		EnableControlKeyPop( void );
	static void		DisableControlKeyPop( void );

private:
	bool			mAutomatic;
};

#pragma mark class ABalloon
// Concrete subclass for help text in a STR# resource
class ABalloon : public ABalloonBase
{
public:
	enum { class_ID = 'ABal' };
	
					ABalloon(
							LStream			*inStream);
					
	virtual			~ABalloon() {}
					
	virtual	void	GetHelpString(
							LPane*			inPane,
							StringPtr		outString );
	
private:
	ResIDT			mStringListID;
	SInt16			mOffDisabledIndex;
	SInt16			mOffEnabledIndex;
	SInt16			mOnDisabledIndex;
	SInt16			mOnEnabledIndex;
};

#pragma mark class AAutoBalloon
class AAutoBalloon : public ABalloon
{
public:
	enum { class_ID = 'AABa' };
	
					AAutoBalloon( LStream *inStream )
						: ABalloon( inStream )
						{ SetAutomatic(true); }
};
