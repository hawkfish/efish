/*
 * File: ./LOGDEF/FILEKIND.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public final class FileKind implements org.omg.CORBA.portable.IDLEntity {
     public static final int _HTMLFILE = 0,
	  		     _UTF8FILE = 1;
     public static final FileKind HTMLFILE = new FileKind(_HTMLFILE);
     public static final FileKind UTF8FILE = new FileKind(_UTF8FILE);
     public int value() {
         return _value;
     }
     public static final FileKind from_int(int i)  throws  org.omg.CORBA.BAD_PARAM {
           switch (i) {
             case _HTMLFILE:
                 return HTMLFILE;
             case _UTF8FILE:
                 return UTF8FILE;
             default:
	              throw new org.omg.CORBA.BAD_PARAM();
           }
     }
     private FileKind(int _value){
         this._value = _value;
     }
     private int _value;
}
