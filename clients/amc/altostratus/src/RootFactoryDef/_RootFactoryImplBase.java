/*
 * File: ./ROOTFACTORYDEF/_ROOTFACTORYIMPLBASE.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public abstract class _RootFactoryImplBase extends org.omg.CORBA.DynamicImplementation implements RootFactoryDef.RootFactory {
    // Constructor
    public _RootFactoryImplBase() {
         super();
    }
    // Type strings for this class and its superclases
    private static final String _type_ids[] = {
        "IDL:RootFactoryDef/RootFactory:1.0",
        "IDL:ObserveM/RegisterDef:1.0"
    };

    public String[] _ids() { return (String[]) _type_ids.clone(); }

    private static java.util.Dictionary _methods = new java.util.Hashtable();
    static {
      _methods.put("subscribe", new java.lang.Integer(0));
      _methods.put("createVteObjectContainer", new java.lang.Integer(1));
      _methods.put("getAllVteObjectContainers", new java.lang.Integer(2));
     }
    // DSI Dispatch call
    public void invoke(org.omg.CORBA.ServerRequest r) {
       switch (((java.lang.Integer) _methods.get(r.op_name())).intValue()) {
           case 0: // ObserveM.RegisterDef.subscribe
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _callback = _orb().create_any();
              _callback.type(ObserveM.CallbackObjDefHelper.type());
              _list.add_value("callback", _callback, org.omg.CORBA.ARG_IN.value);
              org.omg.CORBA.Any _observerTypes = _orb().create_any();
              _observerTypes.type(ObserveM.StatusBitListHelper.type());
              _list.add_value("observerTypes", _observerTypes, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              ObserveM.CallbackObjDef callback;
              callback = ObserveM.CallbackObjDefHelper.extract(_callback);
              short[] observerTypes;
              observerTypes = ObserveM.StatusBitListHelper.extract(_observerTypes);
              try {
                            this.subscribe(callback, observerTypes);
              }
              catch (ObserveM.BadObserveTypeException e0) {
                            org.omg.CORBA.Any _except = _orb().create_any();
                            ObserveM.BadObserveTypeExceptionHelper.insert(_except, e0);
                            r.except(_except);
                            return;
              }
              org.omg.CORBA.Any __return = _orb().create_any();
              __return.type(_orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_void));
              r.result(__return);
              }
              break;
           case 1: // RootFactoryDef.RootFactory.createVteObjectContainer
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              r.params(_list);
              VteEngineServer.VteObjectContainer ___result;
                            ___result = this.createVteObjectContainer();
              org.omg.CORBA.Any __result = _orb().create_any();
              VteEngineServer.VteObjectContainerHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
           case 2: // RootFactoryDef.RootFactory.getAllVteObjectContainers
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              r.params(_list);
              VteEngineServer.VteObjectContainer[] ___result;
                            ___result = this.getAllVteObjectContainers();
              org.omg.CORBA.Any __result = _orb().create_any();
              RootFactoryDef.VteObjectContainerListHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
            default:
              throw new org.omg.CORBA.BAD_OPERATION(0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
       }
 }
}
