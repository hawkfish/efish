/*
 * File: ./OBSERVEM/_CALLBACKOBJDEFSTUB.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public class _CallbackObjDefStub
	extends org.omg.CORBA.portable.ObjectImpl
    	implements ObserveM.CallbackObjDef {

    public _CallbackObjDefStub(org.omg.CORBA.portable.Delegate d) {
          super();
          _set_delegate(d);
    }

    private static final String _type_ids[] = {
        "IDL:ObserveM/CallbackObjDef:1.0"
    };

    public String[] _ids() { return (String[]) _type_ids.clone(); }

    //	IDL operations
    //	    Implementation of ::ObserveM::CallbackObjDef::update
    public void update(org.omg.CORBA.Any obj, short[] observerTypes)
 {
           org.omg.CORBA.Request r = _request("update");
           org.omg.CORBA.Any _obj = r.add_in_arg();
           _obj.insert_any(obj);
           org.omg.CORBA.Any _observerTypes = r.add_in_arg();
           ObserveM.StatusBitListHelper.insert(_observerTypes, observerTypes);
           r.send_oneway();
   }

};
