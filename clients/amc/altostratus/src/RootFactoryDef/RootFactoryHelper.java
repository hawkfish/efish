/*
 * File: ./ROOTFACTORYDEF/ROOTFACTORYHELPER.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public class RootFactoryHelper {
     // It is useless to have instances of this class
     private RootFactoryHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, RootFactoryDef.RootFactory that) {
        out.write_Object(that);
    }
    public static RootFactoryDef.RootFactory read(org.omg.CORBA.portable.InputStream in) {
        return RootFactoryDef.RootFactoryHelper.narrow(in.read_Object());
    }
   public static RootFactoryDef.RootFactory extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, RootFactoryDef.RootFactory that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_interface_tc(id(), "RootFactory");
      return _tc;
   }
   public static String id() {
       return "IDL:RootFactoryDef/RootFactory:1.0";
   }
   public static RootFactoryDef.RootFactory narrow(org.omg.CORBA.Object that)
	    throws org.omg.CORBA.BAD_PARAM {
        if (that == null)
            return null;
        if (that instanceof RootFactoryDef.RootFactory)
            return (RootFactoryDef.RootFactory) that;
	if (!that._is_a(id())) {
	    throw new org.omg.CORBA.BAD_PARAM();
	}
        org.omg.CORBA.portable.Delegate dup = ((org.omg.CORBA.portable.ObjectImpl)that)._get_delegate();
        RootFactoryDef.RootFactory result = new RootFactoryDef._RootFactoryStub(dup);
        return result;
   }
}
