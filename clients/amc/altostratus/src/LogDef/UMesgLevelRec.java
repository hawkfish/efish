/*
 * File: ./LOGDEF/UMESGLEVELREC.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public final class UMesgLevelRec implements org.omg.CORBA.portable.IDLEntity {
    //	instance variables
    public short level;
    public byte[] mesg;
    //	constructors
    public UMesgLevelRec() { }
    public UMesgLevelRec(short __level, byte[] __mesg) {
	level = __level;
	mesg = __mesg;
    }
}
