/*
 * File: ./OBSERVEM/REGISTERDEF.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public interface RegisterDef
    extends org.omg.CORBA.Object, org.omg.CORBA.portable.IDLEntity {
    void subscribe(ObserveM.CallbackObjDef callback, short[] observerTypes)
        throws ObserveM.BadObserveTypeException;
}
