/*
 * File: ./ROOTFACTORYDEF/VTEOBJECTCONTAINERLISTHOLDER.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public final class VteObjectContainerListHolder
    implements org.omg.CORBA.portable.Streamable
{
    //	instance variable 
    public VteEngineServer.VteObjectContainer[] value;
    //	constructors 
    public VteObjectContainerListHolder() {
	this(null);
    }
    public VteObjectContainerListHolder(VteEngineServer.VteObjectContainer[] __arg) {
	value = __arg;
    }
    public void _write(org.omg.CORBA.portable.OutputStream out) {
        RootFactoryDef.VteObjectContainerListHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = RootFactoryDef.VteObjectContainerListHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return RootFactoryDef.VteObjectContainerListHelper.type();
    }
}
