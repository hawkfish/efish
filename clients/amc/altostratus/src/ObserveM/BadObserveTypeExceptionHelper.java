/*
 * File: ./OBSERVEM/BADOBSERVETYPEEXCEPTIONHELPER.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public class BadObserveTypeExceptionHelper {
     // It is useless to have instances of this class
     private BadObserveTypeExceptionHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, ObserveM.BadObserveTypeException that) {
    out.write_string(id());

	out.write_string(that.msg);
    }
    public static ObserveM.BadObserveTypeException read(org.omg.CORBA.portable.InputStream in) {
        ObserveM.BadObserveTypeException that = new ObserveM.BadObserveTypeException();
         // read and discard the repository id
        in.read_string();

	that.msg = in.read_string();
    return that;
    }
   public static ObserveM.BadObserveTypeException extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, ObserveM.BadObserveTypeException that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
       int _memberCount = 1;
       org.omg.CORBA.StructMember[] _members = null;
          if (_tc == null) {
               _members= new org.omg.CORBA.StructMember[1];
               _members[0] = new org.omg.CORBA.StructMember(
                 "msg",
                 org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string),
                 null);
             _tc = org.omg.CORBA.ORB.init().create_exception_tc(id(), "BadObserveTypeException", _members);
          }
      return _tc;
   }
   public static String id() {
       return "IDL:ObserveM/BadObserveTypeException:1.0";
   }
}
