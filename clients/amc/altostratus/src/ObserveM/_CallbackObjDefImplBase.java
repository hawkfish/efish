/*
 * File: ./OBSERVEM/_CALLBACKOBJDEFIMPLBASE.JAVA
 * From: OBSERVEDEF.IDL
 * Date: Fri Nov 10 16:17:22 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package ObserveM;
public abstract class _CallbackObjDefImplBase extends org.omg.CORBA.DynamicImplementation implements ObserveM.CallbackObjDef {
    // Constructor
    public _CallbackObjDefImplBase() {
         super();
//    	System.out.println("_CallbackObjDefImplBase.<constructor>");	// &&&
    }
    // Type strings for this class and its superclases
    private static final String _type_ids[] = {
        "IDL:ObserveM/CallbackObjDef:1.0"
    };

    public String[] _ids() { return (String[]) _type_ids.clone(); }

    private static java.util.Dictionary _methods = new java.util.Hashtable();
    static {
      _methods.put("update", new java.lang.Integer(0));
     }
    // DSI Dispatch call
    public void invoke(org.omg.CORBA.ServerRequest r) {
//    	System.out.println("_CallbackObjDefImplBase.invoke");	// &&&
       switch (((java.lang.Integer) _methods.get(r.op_name())).intValue()) {
           case 0: // ObserveM.CallbackObjDef.update
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _obj = _orb().create_any();
              _obj.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_any));
              _list.add_value("obj", _obj, org.omg.CORBA.ARG_IN.value);
              org.omg.CORBA.Any _observerTypes = _orb().create_any();
              _observerTypes.type(ObserveM.StatusBitListHelper.type());
              _list.add_value("observerTypes", _observerTypes, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              org.omg.CORBA.Any obj;
              obj = _obj.extract_any();
              short[] observerTypes;
              observerTypes = ObserveM.StatusBitListHelper.extract(_observerTypes);
                            this.update(obj, observerTypes);
              org.omg.CORBA.Any __return = _orb().create_any();
              __return.type(_orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_void));
              r.result(__return);
              }
              break;
            default:
              throw new org.omg.CORBA.BAD_OPERATION(0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
       }
 }
}
