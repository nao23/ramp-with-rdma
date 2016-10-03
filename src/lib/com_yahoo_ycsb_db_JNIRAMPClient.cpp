#include "com_yahoo_ycsb_db_JNIRAMPClient.h"
#include "LockBasedHandler.h"
#include "NoCCHandler.h"
#include "RAMPFastHandler.h"
#include "ACRAMPFastHandler.h"


JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1configure(JNIEnv *env, jclass my_class) {

    Config& config = Config::get_config();
    config.com_type = ComType::IPoIB;
    config.read_server_list();    
}

JNIEXPORT jlong JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1construct(JNIEnv *env, jobject me) {
    
    TrxHandler* handler = new RAMPFastHandler(0); // TODO: change TrxHandler type dynamicaly and set unique id
    return reinterpret_cast<long>(handler);
}

JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1destruct(JNIEnv *env, jobject me, long obj_p) {
    
    TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p);
    delete handler;
}

JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1connect_1all(JNIEnv *env, jobject me, long obj_p) {
    
    TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p);
    handler->cp.connect_all(); 
}


JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1close_1all(JNIEnv *env, jobject me, long obj_p) {
    
    TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p);
    handler->cp.close_all(); 
}

JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1read(JNIEnv *env, jobject me, long obj_p, jstring jkey, jobject jfields) {

    // Set
    jclass JSet = env->FindClass("Ljava/util/Set;");
    jmethodID iteratorMethod = env->GetMethodID(JSet, "iterator", "()Ljava/util/Iterator;");

    // Iterator
    jclass JIterator = env->FindClass("Ljava/util/Iterator;");
    jmethodID hasNextMethod = env->GetMethodID(JIterator, "hasNext", "()Z");
    jmethodID nextMethod = env->GetMethodID(JIterator, "next", "()Ljava/lang/Object;");

    
    // get Key
    const char* k_addr = env->GetStringUTFChars(jkey, NULL);
    jsize k_len = env->GetStringUTFLength(jkey);
    Key key(k_addr, k_len);

    // get Fields
    std::set<Field> fields;
    if (jfields != NULL) {
	jobject jfields_iter = env->CallObjectMethod(jfields, iteratorMethod);
	while(env->CallBooleanMethod(jfields_iter, hasNextMethod)) {
	    jstring jfield = (jstring)env->CallObjectMethod(jfields_iter, nextMethod);
	    const char* f_addr = env->GetStringUTFChars(jfield, NULL);
	    jsize f_len = env->GetStringUTFLength(jfield);
	    fields.insert(Field(f_addr, f_len));
	}
    }

    TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p);
    handler->trx->add_read(key, fields);
}

JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1insert(JNIEnv *env, jobject me, jlong obj_p, jstring jkey, jobject jvalues) {

    // Set
    jclass JSet = (env)->FindClass("Ljava/util/Set;");
    jmethodID iteratorMethod = env->GetMethodID(JSet, "iterator", "()Ljava/util/Iterator;");

    // HashMap 
    jclass JHashMap = (env)->GetObjectClass(jvalues);
    jmethodID entrySetMethod = (env)->GetMethodID(JHashMap, "entrySet", "()Ljava/util/Set;");

    // Iterator
    jclass JIterator = env->FindClass("Ljava/util/Iterator;");
    jmethodID hasNextMethod = env->GetMethodID(JIterator, "hasNext", "()Z");
    jmethodID nextMethod = env->GetMethodID(JIterator, "next", "()Ljava/lang/Object;");

    // MapEntry
    jclass JMapEntry = (env)->FindClass("java/util/Map$Entry");
    jmethodID getKeyMethod = env->GetMethodID(JMapEntry, "getKey", "()Ljava/lang/Object;");
    jmethodID getValueMethod = env->GetMethodID(JMapEntry, "getValue", "()Ljava/lang/Object;");


    // get Key
    const char* addr = env->GetStringUTFChars(jkey, NULL);
    jsize len = env->GetStringUTFLength(jkey);
    Key key(addr, len);
    
    // get Values
    std::map<Field, Value> values;
    jobject jvalues_entryset = env->CallObjectMethod(jvalues, entrySetMethod);   
    jobject jvalues_entryset_iter = env->CallObjectMethod(jvalues_entryset, iteratorMethod);

    while (env->CallBooleanMethod(jvalues_entryset_iter, hasNextMethod)) {

	jobject jvalues_entry = env->CallObjectMethod(jvalues_entryset_iter, nextMethod);

	// get Field
	jstring jfield = (jstring)env->CallObjectMethod(jvalues_entry, getKeyMethod);
	const char* f_addr = env->GetStringUTFChars(jfield, NULL);
	jsize f_len = env->GetStringUTFLength(jfield); 

	// get Value
	jbyteArray jvalue = (jbyteArray)env->CallObjectMethod(jvalues_entry, getValueMethod);
	const char* v_addr = (const char*)env->GetByteArrayElements(jvalue, NULL);
	jsize v_len = env->GetArrayLength(jvalue); 
	
	values[Field(f_addr, f_len)] = Value(v_addr, v_len);
    }

    TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p);    
    handler->trx->add_write(key, values);
}

JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1put_1all(JNIEnv *env, jobject me, jlong obj_p) {
  
   TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p);
   handler->put_all();
}

JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1get_1all(JNIEnv *env, jobject me, jlong obj_p) {
    
    TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p); 
    handler->get_all();
}

JNIEXPORT void JNICALL Java_com_yahoo_ycsb_db_JNIRAMPClient__1clear(JNIEnv *env, jobject me, jlong obj_p) {

    TrxHandler* handler = reinterpret_cast<TrxHandler*>(obj_p); 
    handler->trx->clear();
    handler->cp.clear_access_coms();
}
