/*
 * File: ./ROOTFACTORYDEF/VTEOBJECTCONTAINERLISTHELPER.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public class VteObjectContainerListHelper {
     // It is useless to have instances of this class
     private VteObjectContainerListHelper() { }

    public static void write(org.omg.CORBA.portable.OutputStream out, VteEngineServer.VteObjectContainer[] that)  {
          {
              out.write_long(that.length);
              for (int __index = 0 ; __index < that.length ; __index += 1) {
                  VteEngineServer.VteObjectContainerHelper.write(out, that[__index]);
              }
          }
    }
    public static VteEngineServer.VteObjectContainer[] read(org.omg.CORBA.portable.InputStream in) {
          VteEngineServer.VteObjectContainer[] that;
          {
              int __length = in.read_long();
              that = new VteEngineServer.VteObjectContainer[__length];
              for (int __index = 0 ; __index < that.length ; __index += 1) {
                  that[__index] = VteEngineServer.VteObjectContainerHelper.read(in);
              }
          }
          return that;
    }
   public static VteEngineServer.VteObjectContainer[] extract(org.omg.CORBA.Any a) {
     org.omg.CORBA.portable.InputStream in = a.create_input_stream();
     return read(in);
   }
   public static void insert(org.omg.CORBA.Any a, VteEngineServer.VteObjectContainer[] that) {
     org.omg.CORBA.portable.OutputStream out = a.create_output_stream();
     a.type(type());
     write(out, that);
     a.read_value(out.create_input_stream(), type());
   }
   private static org.omg.CORBA.TypeCode _tc;
   synchronized public static org.omg.CORBA.TypeCode type() {
          if (_tc == null)
             _tc = org.omg.CORBA.ORB.init().create_alias_tc(id(), "VteObjectContainerList", org.omg.CORBA.ORB.init().create_sequence_tc(0, VteEngineServer.VteObjectContainerHelper.type()));
      return _tc;
   }
   public static String id() {
       return "IDL:RootFactoryDef/VteObjectContainerList:1.0";
   }
}
