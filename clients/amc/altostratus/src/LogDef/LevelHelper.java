/*
 * File: ./LOGDEF/LEVELHELPER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public class LevelHelper {
     // It is useless to have instances of this class
     private LevelHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, LogDef.Level that) {
        out.write_Object(that);
    }
    public static LogDef.Level read(org.omg.CORBA.portable.InputStream in) {
        return LogDef.LevelHelper.narrow(in.read_Object());
    }
   public static LogDef.Level extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, LogDef.Level that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_interface_tc(id(), "Level");
      return _tc;
   }
   public static String id() {
       return "IDL:LogDef/Level:1.0";
   }
   public static LogDef.Level narrow(org.omg.CORBA.Object that)
	    throws org.omg.CORBA.BAD_PARAM {
        if (that == null)
            return null;
        if (that instanceof LogDef.Level)
            return (LogDef.Level) that;
	if (!that._is_a(id())) {
	    throw new org.omg.CORBA.BAD_PARAM();
	}
        org.omg.CORBA.portable.Delegate dup = ((org.omg.CORBA.portable.ObjectImpl)that)._get_delegate();
        LogDef.Level result = new LogDef._LevelStub(dup);
        return result;
   }
}
