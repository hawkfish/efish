/*
 * File: ./LOGDEF/DESTTYPE.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public final class DestType implements org.omg.CORBA.portable.IDLEntity {
     public static final int _STDOUT = 0,
	  		     _STDERR = 1,
	  		     _DISKFILE = 2,
	  		     _MEMORY = 3;
     public static final DestType STDOUT = new DestType(_STDOUT);
     public static final DestType STDERR = new DestType(_STDERR);
     public static final DestType DISKFILE = new DestType(_DISKFILE);
     public static final DestType MEMORY = new DestType(_MEMORY);
     public int value() {
         return _value;
     }
     public static final DestType from_int(int i)  throws  org.omg.CORBA.BAD_PARAM {
           switch (i) {
             case _STDOUT:
                 return STDOUT;
             case _STDERR:
                 return STDERR;
             case _DISKFILE:
                 return DISKFILE;
             case _MEMORY:
                 return MEMORY;
             default:
	              throw new org.omg.CORBA.BAD_PARAM();
           }
     }
     private DestType(int _value){
         this._value = _value;
     }
     private int _value;
}
