#pragma once

#include <Lists.h>
#include <Types.h>

//	Constants
typedef enum EnvListVersion {
	kEnvListCurrentVersion = 1,
	
	kEnvListFnordVersion
	} EnvListVersion;
	
//	Class
#pragma options align=mac68k
typedef struct EnvironmentList {
	EnvListVersion	mVersion;
	short			mVarCount;
	}	EnvironmentList, 
		*EnvironmentListPtr,
		**EnvironmentListHandle;
#pragma options align=reset

//	Methods
void 
EnvListClear (

	EnvironmentListHandle	list);

Boolean	
EnvListEqual (

	EnvironmentListHandle	prefs1, 
	EnvironmentListHandle	prefs2);

Boolean
EnvListGet (

	EnvironmentListHandle	list,
	ConstStr255Param		inKey,
	Str255					outValue,
	ConstStr255Param		inDefault);
	
Boolean
EnvListGet (

	EnvironmentListPtr		list,
	ConstStr255Param		inKey,
	Str255					outValue,
	ConstStr255Param		inDefault);
	
OSErr
EnvListSet (

	EnvironmentListHandle	list,
	ConstStr255Param		inKey,
	ConstStr255Param		inValue);

void
EnvListToListBox (

	EnvironmentListHandle	list,
	ListHandle 				listBox,
	const	char			sep);

OSErr 
EnvListFromListBox (

	EnvironmentListHandle	list,
	ListHandle 				listBox,
	const	char			sep);

OSErr 
EnvListToAEDescList (

	EnvironmentListPtr		list,
	AEDescList* 			aeList);

OSErr 
EnvListToAEDescList (

	EnvironmentListHandle	list,
	AEDescList* 			aeList);

OSErr 
EnvListFromAEDescList (

	EnvironmentListHandle	list,
	const	AEDescList* 	aeList);
