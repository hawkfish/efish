/*
 * File: ./LOGDEF/_LOGIMPLBASE.JAVA
 * From: LOGDEF.IDL
 * Date: Fri Nov 10 16:07:52 2000
 *   By: C:\ALTOST~1\IDL\IDLTOJ~1.EXE Java IDL 1.2 Aug 18 1998 16:25:34
 */

package LogDef;
public abstract class _LogImplBase extends org.omg.CORBA.DynamicImplementation implements LogDef.Log {
    // Constructor
    public _LogImplBase() {
         super();
    }
    // Type strings for this class and its superclases
    private static final String _type_ids[] = {
        "IDL:LogDef/Log:1.0",
        "IDL:ObserveM/RegisterDef:1.0"
    };

    public String[] _ids() { return (String[]) _type_ids.clone(); }

    private static java.util.Dictionary _methods = new java.util.Hashtable();
    static {
      _methods.put("subscribe", new java.lang.Integer(0));
      _methods.put("setDestinationList", new java.lang.Integer(1));
      _methods.put("setDestinationFilename", new java.lang.Integer(2));
      _methods.put("getDestinationFilename", new java.lang.Integer(3));
      _methods.put("getDestinationList", new java.lang.Integer(4));
      _methods.put("error", new java.lang.Integer(5));
      _methods.put("info", new java.lang.Integer(6));
      _methods.put("warn", new java.lang.Integer(7));
      _methods.put("debug", new java.lang.Integer(8));
      _methods.put("numMessageInLog", new java.lang.Integer(9));
      _methods.put("getMessageList", new java.lang.Integer(10));
      _methods.put("deleteContents", new java.lang.Integer(11));
      _methods.put("setFileKind", new java.lang.Integer(12));
      _methods.put("getFileKind", new java.lang.Integer(13));
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
           case 1: // LogDef.Log.setDestinationList
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _destinations = _orb().create_any();
              _destinations.type(LogDef.DestListHelper.type());
              _list.add_value("destinations", _destinations, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              LogDef.DestType[] destinations;
              destinations = LogDef.DestListHelper.extract(_destinations);
                            this.setDestinationList(destinations);
              org.omg.CORBA.Any __return = _orb().create_any();
              __return.type(_orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_void));
              r.result(__return);
              }
              break;
           case 2: // LogDef.Log.setDestinationFilename
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _path = _orb().create_any();
              _path.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
              _list.add_value("path", _path, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              String path;
              path = _path.extract_string();
                            this.setDestinationFilename(path);
              org.omg.CORBA.Any __return = _orb().create_any();
              __return.type(_orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_void));
              r.result(__return);
              }
              break;
           case 3: // LogDef.Log.getDestinationFilename
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              r.params(_list);
              String ___result;
                            ___result = this.getDestinationFilename();
              org.omg.CORBA.Any __result = _orb().create_any();
              __result.insert_string(___result);
              r.result(__result);
              }
              break;
           case 4: // LogDef.Log.getDestinationList
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              r.params(_list);
              LogDef.DestType[] ___result;
                            ___result = this.getDestinationList();
              org.omg.CORBA.Any __result = _orb().create_any();
              LogDef.DestListHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
           case 5: // LogDef.Log.error
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _format = _orb().create_any();
              _format.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
              _list.add_value("format", _format, org.omg.CORBA.ARG_IN.value);
              org.omg.CORBA.Any _argList = _orb().create_any();
              _argList.type(LogDef.UMesgListHelper.type());
              _list.add_value("argList", _argList, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              String format;
              format = _format.extract_string();
              byte[][] argList;
              argList = LogDef.UMesgListHelper.extract(_argList);
              LogDef.UMesgLevelRec ___result;
                            ___result = this.error(format, argList);
              org.omg.CORBA.Any __result = _orb().create_any();
              LogDef.UMesgLevelRecHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
           case 6: // LogDef.Log.info
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _format = _orb().create_any();
              _format.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
              _list.add_value("format", _format, org.omg.CORBA.ARG_IN.value);
              org.omg.CORBA.Any _argList = _orb().create_any();
              _argList.type(LogDef.UMesgListHelper.type());
              _list.add_value("argList", _argList, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              String format;
              format = _format.extract_string();
              byte[][] argList;
              argList = LogDef.UMesgListHelper.extract(_argList);
              LogDef.UMesgLevelRec ___result;
                            ___result = this.info(format, argList);
              org.omg.CORBA.Any __result = _orb().create_any();
              LogDef.UMesgLevelRecHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
           case 7: // LogDef.Log.warn
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _format = _orb().create_any();
              _format.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
              _list.add_value("format", _format, org.omg.CORBA.ARG_IN.value);
              org.omg.CORBA.Any _argList = _orb().create_any();
              _argList.type(LogDef.UMesgListHelper.type());
              _list.add_value("argList", _argList, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              String format;
              format = _format.extract_string();
              byte[][] argList;
              argList = LogDef.UMesgListHelper.extract(_argList);
              LogDef.UMesgLevelRec ___result;
                            ___result = this.warn(format, argList);
              org.omg.CORBA.Any __result = _orb().create_any();
              LogDef.UMesgLevelRecHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
           case 8: // LogDef.Log.debug
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _format = _orb().create_any();
              _format.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
              _list.add_value("format", _format, org.omg.CORBA.ARG_IN.value);
              org.omg.CORBA.Any _argList = _orb().create_any();
              _argList.type(LogDef.UMesgListHelper.type());
              _list.add_value("argList", _argList, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              String format;
              format = _format.extract_string();
              byte[][] argList;
              argList = LogDef.UMesgListHelper.extract(_argList);
              LogDef.UMesgLevelRec ___result;
                            ___result = this.debug(format, argList);
              org.omg.CORBA.Any __result = _orb().create_any();
              LogDef.UMesgLevelRecHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
           case 9: // LogDef.Log.numMessageInLog
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              r.params(_list);
              int ___result;
                            ___result = this.numMessageInLog();
              org.omg.CORBA.Any __result = _orb().create_any();
              __result.insert_ulong(___result);
              r.result(__result);
              }
              break;
           case 10: // LogDef.Log.getMessageList
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _startOffset = _orb().create_any();
              _startOffset.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_ulong));
              _list.add_value("startOffset", _startOffset, org.omg.CORBA.ARG_IN.value);
              org.omg.CORBA.Any _numMessages = _orb().create_any();
              _numMessages.type(org.omg.CORBA.ORB.init().get_primitive_tc(org.omg.CORBA.TCKind.tk_ulong));
              _list.add_value("numMessages", _numMessages, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              int startOffset;
              startOffset = _startOffset.extract_ulong();
              int numMessages;
              numMessages = _numMessages.extract_ulong();
              LogDef.UMesgLevelRec[] ___result;
                            ___result = this.getMessageList(startOffset, numMessages);
              org.omg.CORBA.Any __result = _orb().create_any();
              LogDef.UMesgLevelListHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
           case 11: // LogDef.Log.deleteContents
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              r.params(_list);
                            this.deleteContents();
              org.omg.CORBA.Any __return = _orb().create_any();
              __return.type(_orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_void));
              r.result(__return);
              }
              break;
           case 12: // LogDef.Log.setFileKind
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              org.omg.CORBA.Any _kind = _orb().create_any();
              _kind.type(LogDef.FileKindHelper.type());
              _list.add_value("kind", _kind, org.omg.CORBA.ARG_IN.value);
              r.params(_list);
              LogDef.FileKind kind;
              kind = LogDef.FileKindHelper.extract(_kind);
                            this.setFileKind(kind);
              org.omg.CORBA.Any __return = _orb().create_any();
              __return.type(_orb().get_primitive_tc(org.omg.CORBA.TCKind.tk_void));
              r.result(__return);
              }
              break;
           case 13: // LogDef.Log.getFileKind
              {
              org.omg.CORBA.NVList _list = _orb().create_list(0);
              r.params(_list);
              LogDef.FileKind ___result;
                            ___result = this.getFileKind();
              org.omg.CORBA.Any __result = _orb().create_any();
              LogDef.FileKindHelper.insert(__result, ___result);
              r.result(__result);
              }
              break;
            default:
              throw new org.omg.CORBA.BAD_OPERATION(0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
       }
 }
}
