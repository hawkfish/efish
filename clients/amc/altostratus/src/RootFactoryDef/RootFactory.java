/*
 * File: ./ROOTFACTORYDEF/ROOTFACTORY.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public interface RootFactory
    extends org.omg.CORBA.Object, org.omg.CORBA.portable.IDLEntity,
	    ObserveM.RegisterDef {
    final short SHUTDOWN_TYPE = (short) (1L << 0L);
    VteEngineServer.VteObjectContainer createVteObjectContainer()
;
    VteEngineServer.VteObjectContainer[] getAllVteObjectContainers()
;
}
