/*
 * File: ./LOGDEF/LEVEL.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public interface Level
    extends org.omg.CORBA.Object, org.omg.CORBA.portable.IDLEntity {
    final short DEBUG = (short) (0L);
    final short INFO = (short) (1L);
    final short WARN = (short) (2L);
    final short ERROR = (short) (3L);
    final short ERASED = (short) (4L);
}
