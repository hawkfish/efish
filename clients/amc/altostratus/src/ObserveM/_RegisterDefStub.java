/*
 * File: ./OBSERVEM/_REGISTERDEFSTUB.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public class _RegisterDefStub
	extends org.omg.CORBA.portable.ObjectImpl
    	implements ObserveM.RegisterDef {

    public _RegisterDefStub(org.omg.CORBA.portable.Delegate d) {
          super();
          _set_delegate(d);
    }

    private static final String _type_ids[] = {
        "IDL:ObserveM/RegisterDef:1.0"
    };

    public String[] _ids() { return (String[]) _type_ids.clone(); }

    //	IDL operations
    //	    Implementation of ::ObserveM::RegisterDef::subscribe
    public void subscribe(ObserveM.CallbackObjDef callback, short[] observerTypes)
        throws ObserveM.BadObserveTypeException {
           org.omg.CORBA.Request r = _request("subscribe");
           org.omg.CORBA.Any _callback = r.add_in_arg();
           ObserveM.CallbackObjDefHelper.insert(_callback, callback);
           org.omg.CORBA.Any _observerTypes = r.add_in_arg();
           ObserveM.StatusBitListHelper.insert(_observerTypes, observerTypes);
           r.exceptions().add(ObserveM.BadObserveTypeExceptionHelper.type());
           r.invoke();
           java.lang.Exception __ex = r.env().exception();
           if (__ex instanceof org.omg.CORBA.UnknownUserException) {
               org.omg.CORBA.UnknownUserException __userEx = (org.omg.CORBA.UnknownUserException) __ex;
               if (__userEx.except.type().equals(ObserveM.BadObserveTypeExceptionHelper.type())) {
                   throw ObserveM.BadObserveTypeExceptionHelper.extract(__userEx.except);
               }
           }
   }

};
