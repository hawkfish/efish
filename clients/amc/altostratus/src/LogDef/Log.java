/*
 * File: ./LOGDEF/LOG.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public interface Log
    extends org.omg.CORBA.Object, org.omg.CORBA.portable.IDLEntity,
	    ObserveM.RegisterDef {
    void setDestinationList(LogDef.DestType[] destinations)
;
    void setDestinationFilename(String path)
;
    String getDestinationFilename()
;
    LogDef.DestType[] getDestinationList()
;
    LogDef.UMesgLevelRec error(String format, byte[][] argList)
;
    LogDef.UMesgLevelRec info(String format, byte[][] argList)
;
    LogDef.UMesgLevelRec warn(String format, byte[][] argList)
;
    LogDef.UMesgLevelRec debug(String format, byte[][] argList)
;
    int numMessageInLog()
;
    LogDef.UMesgLevelRec[] getMessageList(int startOffset, int numMessages)
;
    void deleteContents()
;
    void setFileKind(LogDef.FileKind kind)
;
    LogDef.FileKind getFileKind()
;
}
