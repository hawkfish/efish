/*
 * File: ./LOGDEF/_LOGSTUB.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public class _LogStub
	extends org.omg.CORBA.portable.ObjectImpl
    	implements LogDef.Log {

    public _LogStub(org.omg.CORBA.portable.Delegate d) {
          super();
          _set_delegate(d);
    }

    private static final String _type_ids[] = {
        "IDL:LogDef/Log:1.0",
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
    //	    Implementation of ::LogDef::Log::setDestinationList
    public void setDestinationList(LogDef.DestType[] destinations)
 {
           org.omg.CORBA.Request r = _request("setDestinationList");
           org.omg.CORBA.Any _destinations = r.add_in_arg();
           LogDef.DestListHelper.insert(_destinations, destinations);
           r.invoke();
   }
    //	    Implementation of ::LogDef::Log::setDestinationFilename
    public void setDestinationFilename(String path)
 {
           org.omg.CORBA.Request r = _request("setDestinationFilename");
           org.omg.CORBA.Any _path = r.add_in_arg();
           _path.insert_string(path);
           r.invoke();
   }
    //	    Implementation of ::LogDef::Log::getDestinationFilename
    public String getDestinationFilename()
 {
           org.omg.CORBA.Request r = _request("getDestinationFilename");
           r.set_return_type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
           r.invoke();
           String __result;
           __result = r.return_value().extract_string();
           return __result;
   }
    //	    Implementation of ::LogDef::Log::getDestinationList
    public LogDef.DestType[] getDestinationList()
 {
           org.omg.CORBA.Request r = _request("getDestinationList");
           r.set_return_type(LogDef.DestListHelper.type());
           r.invoke();
           LogDef.DestType[] __result;
           __result = LogDef.DestListHelper.extract(r.return_value());
           return __result;
   }
    //	    Implementation of ::LogDef::Log::error
    public LogDef.UMesgLevelRec error(String format, byte[][] argList)
 {
           org.omg.CORBA.Request r = _request("error");
           r.set_return_type(LogDef.UMesgLevelRecHelper.type());
           org.omg.CORBA.Any _format = r.add_in_arg();
           _format.insert_string(format);
           org.omg.CORBA.Any _argList = r.add_in_arg();
           LogDef.UMesgListHelper.insert(_argList, argList);
           r.invoke();
           LogDef.UMesgLevelRec __result;
           __result = LogDef.UMesgLevelRecHelper.extract(r.return_value());
           return __result;
   }
    //	    Implementation of ::LogDef::Log::info
    public LogDef.UMesgLevelRec info(String format, byte[][] argList)
 {
           org.omg.CORBA.Request r = _request("info");
           r.set_return_type(LogDef.UMesgLevelRecHelper.type());
           org.omg.CORBA.Any _format = r.add_in_arg();
           _format.insert_string(format);
           org.omg.CORBA.Any _argList = r.add_in_arg();
           LogDef.UMesgListHelper.insert(_argList, argList);
           r.invoke();
           LogDef.UMesgLevelRec __result;
           __result = LogDef.UMesgLevelRecHelper.extract(r.return_value());
           return __result;
   }
    //	    Implementation of ::LogDef::Log::warn
    public LogDef.UMesgLevelRec warn(String format, byte[][] argList)
 {
           org.omg.CORBA.Request r = _request("warn");
           r.set_return_type(LogDef.UMesgLevelRecHelper.type());
           org.omg.CORBA.Any _format = r.add_in_arg();
           _format.insert_string(format);
           org.omg.CORBA.Any _argList = r.add_in_arg();
           LogDef.UMesgListHelper.insert(_argList, argList);
           r.invoke();
           LogDef.UMesgLevelRec __result;
           __result = LogDef.UMesgLevelRecHelper.extract(r.return_value());
           return __result;
   }
    //	    Implementation of ::LogDef::Log::debug
    public LogDef.UMesgLevelRec debug(String format, byte[][] argList)
 {
           org.omg.CORBA.Request r = _request("debug");
           r.set_return_type(LogDef.UMesgLevelRecHelper.type());
           org.omg.CORBA.Any _format = r.add_in_arg();
           _format.insert_string(format);
           org.omg.CORBA.Any _argList = r.add_in_arg();
           LogDef.UMesgListHelper.insert(_argList, argList);
           r.invoke();
           LogDef.UMesgLevelRec __result;
           __result = LogDef.UMesgLevelRecHelper.extract(r.return_value());
           return __result;
   }
    //	    Implementation of ::LogDef::Log::numMessageInLog
    public int numMessageInLog()
 {
           org.omg.CORBA.Request r = _request("numMessageInLog");
           r.set_return_type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_ulong));
           r.invoke();
           int __result;
           __result = r.return_value().extract_ulong();
           return __result;
   }
    //	    Implementation of ::LogDef::Log::getMessageList
    public LogDef.UMesgLevelRec[] getMessageList(int startOffset, int numMessages)
 {
           org.omg.CORBA.Request r = _request("getMessageList");
           r.set_return_type(LogDef.UMesgLevelListHelper.type());
           org.omg.CORBA.Any _startOffset = r.add_in_arg();
           _startOffset.insert_ulong(startOffset);
           org.omg.CORBA.Any _numMessages = r.add_in_arg();
           _numMessages.insert_ulong(numMessages);
           r.invoke();
           LogDef.UMesgLevelRec[] __result;
           __result = LogDef.UMesgLevelListHelper.extract(r.return_value());
           return __result;
   }
    //	    Implementation of ::LogDef::Log::deleteContents
    public void deleteContents()
 {
           org.omg.CORBA.Request r = _request("deleteContents");
           r.invoke();
   }
    //	    Implementation of ::LogDef::Log::setFileKind
    public void setFileKind(LogDef.FileKind kind)
 {
           org.omg.CORBA.Request r = _request("setFileKind");
           org.omg.CORBA.Any _kind = r.add_in_arg();
           LogDef.FileKindHelper.insert(_kind, kind);
           r.invoke();
   }
    //	    Implementation of ::LogDef::Log::getFileKind
    public LogDef.FileKind getFileKind()
 {
           org.omg.CORBA.Request r = _request("getFileKind");
           r.set_return_type(LogDef.FileKindHelper.type());
           r.invoke();
           LogDef.FileKind __result;
           __result = LogDef.FileKindHelper.extract(r.return_value());
           return __result;
   }

};
