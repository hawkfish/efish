/*
 * File: ./ROOTFACTORYDEF/CREATEOBJECTEXCEPTIONHOLDER.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public final class CreateObjectExceptionHolder
     implements org.omg.CORBA.portable.Streamable{
    //	instance variable 
    public RootFactoryDef.CreateObjectException value;
    //	constructors 
    public CreateObjectExceptionHolder() {
	this(null);
    }
    public CreateObjectExceptionHolder(RootFactoryDef.CreateObjectException __arg) {
	value = __arg;
    }

    public void _write(org.omg.CORBA.portable.OutputStream out) {
        RootFactoryDef.CreateObjectExceptionHelper.write(out, value);
    }

    public void _read(org.omg.CORBA.portable.InputStream in) {
        value = RootFactoryDef.CreateObjectExceptionHelper.read(in);
    }

    public org.omg.CORBA.TypeCode _type() {
        return RootFactoryDef.CreateObjectExceptionHelper.type();
    }
}
