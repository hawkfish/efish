/*
 * File: ./LOGDEF/UMESGLEVELRECHELPER.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public class UMesgLevelRecHelper {
     // It is useless to have instances of this class
     private UMesgLevelRecHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, LogDef.UMesgLevelRec that) {
	out.write_short(that.level);
	{
	    out.write_long(that.mesg.length);
	    out.write_octet_array(that.mesg, 0, that.mesg.length);
	}
    }
    public static LogDef.UMesgLevelRec read(org.omg.CORBA.portable.InputStream in) {
        LogDef.UMesgLevelRec that = new LogDef.UMesgLevelRec();
	that.level = in.read_short();
	{
	    int __length = in.read_long();
	    that.mesg = new byte[__length];
	    in.read_octet_array(that.mesg, 0, that.mesg.length);
	}
        return that;
    }
   public static LogDef.UMesgLevelRec extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, LogDef.UMesgLevelRec that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
       int _memberCount = 2;
       org.omg.CORBA.StructMember[] _members = null;
          if (_tc == null) {
               _members= new org.omg.CORBA.StructMember[2];
               _members[0] = new org.omg.CORBA.StructMember(
                 "level",
                 org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_short),
                 null);

               _members[1] = new org.omg.CORBA.StructMember(
                 "mesg",
                 org.omg.CORBA.ORB.init().create_sequence_tc(0, org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_octet)),
                 null);
             _tc = org.omg.CORBA.ORB.init().create_struct_tc(id(), "UMesgLevelRec", _members);
          }
      return _tc;
   }
   public static String id() {
       return "IDL:LogDef/UMesgLevelRec:1.0";
   }
}
