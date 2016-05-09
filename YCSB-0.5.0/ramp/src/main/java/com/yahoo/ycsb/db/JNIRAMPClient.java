package com.yahoo.ycsb.db;

import java.util.Map;
import java.util.List;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;
import java.util.Vector;
import java.nio.ByteBuffer;
import com.yahoo.ycsb.ByteIterator;


public class JNIRAMPClient {

    static {
	System.loadLibrary("ramp");
	_configure();
    }
    
    public long obj_p;
    
    JNIRAMPClient() {
	this.obj_p = _construct();
    }
    
    private static native void _configure();
    private native long _construct();
    private native void _destruct(long obj_p);    
    private native void _connect_all(long obj_p);
    private native void _close_all(long obj_p);
    private native void _read(long obj_p, String key, Set<String> fields);
    private native void _insert(long obj_p, String key, HashMap<String, byte[]> values);
    private native void _put_all(long obj_p);
    private native void _get_all(long obj_p);
    private native void _clear(long obj_p);

    
    public void connect_all() {
	_connect_all(this.obj_p);
    }
    
    public void close_all() {
	_close_all(this.obj_p);
    }
    
    public void destruct() {
	_destruct(this.obj_p);
    }
    
    public void read(String key, Set<String> fields) {
	_read(this.obj_p, key, fields);
    }
    
    public void insert(String key, HashMap<String, ByteIterator> values) {

	HashMap<String, byte[]> array_values = new HashMap<String, byte[]>();
	for (Map.Entry<String, ByteIterator> e : values.entrySet()) {
	    array_values.put(e.getKey(), e.getValue().toArray());
	}
	_insert(this.obj_p, key, array_values);
    }

    public void delete(String key) {
	HashMap<String, byte[]> array_values = new HashMap<String, byte[]>();
	array_values.put(key, new byte[0]);
	_insert(this.obj_p, key, array_values);
    }

    public void put_all() {
	_put_all(this.obj_p);
    }
    
    public void get_all() {
	_get_all(this.obj_p);
    }

    public void clear() {
	_clear(this.obj_p);
    }
}