/*
 * File: ./ROOTFACTORYDEF/_ROOTFACTORYSTUB.JAVA
 * From: ROOTFACTORYDEF.IDL
 * Date: Wed Nov 08 16:42:26 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package RootFactoryDef;
public class _RootFactoryStub
	extends org.omg.CORBA.portable.ObjectImpl
    	implements RootFactoryDef.RootFactory {

    public _RootFactoryStub(org.omg.CORBA.portable.Delegate d) {
          super();
          _set_delegate(d);
    }

    private static final String _type_ids[] = {
        "IDL:RootFactoryDef/RootFactory:1.0",
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
    //	    Implementation of ::RootFactoryDef::RootFactory::createVteObjectContainer
    public VteEngineServer.VteObjectContainer createVteObjectContainer()
 {
           org.omg.CORBA.Request r = _request("createVteObjectContainer");
           r.set_return_type(VteEngineServer.VteObjectContainerHelper.type());
           r.invoke();
           VteEngineServer.VteObjectContainer __result;
           __result = VteEngineServer.VteObjectContainerHelper.extract(r.return_value());
           return __result;
   }
    //	    Implementation of ::RootFactoryDef::RootFactory::getAllVteObjectContainers
    public VteEngineServer.VteObjectContainer[] getAllVteObjectContainers()
 {
           org.omg.CORBA.Request r = _request("getAllVteObjectContainers");
           r.set_return_type(RootFactoryDef.VteObjectContainerListHelper.type());
           r.invoke();
           VteEngineServer.VteObjectContainer[] __result;
           __result = RootFactoryDef.VteObjectContainerListHelper.extract(r.return_value());
           return __result;
   }

};
