/**
 * Copyright (c) 2010 Yahoo! Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You
 * may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License. See accompanying
 * LICENSE file.
 */

package com.yahoo.ycsb.db;

import com.yahoo.ycsb.*;

import java.lang.Exception;
import java.lang.Integer;
import java.lang.Math;
import java.lang.String;
import java.lang.System;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Vector;
import java.util.Random;
import java.util.Properties;
import java.nio.ByteBuffer;


public class RAMPClient extends DB {
    
    boolean terminated = false;

    JNIRAMPClient client;

    int put_num = 0;
    int get_num = 0;
    
    public void init() throws DBException {
	try {
	    client = new JNIRAMPClient();
	    client.connect_all();
        } catch (Exception e) {
	    e.printStackTrace();
            throw new DBException(e);
        }
    }

    public void cleanup() throws DBException {
        try {
	    terminated = true;
	    client.close_all();
	    client.destruct();
        } catch (Exception e) {
            throw new DBException(e);
        }
    }

    public Status read(String table, String key, Set<String> fields, HashMap<String, ByteIterator> result) {
        try {
	    get_num += 1;
	    client.read(key, fields);
        } catch (Exception e) {
            System.err.println(e);
	    e.printStackTrace();
            return Status.ERROR;
        }
        return Status.OK;
    }

    public Status scan(String table, String startkey, int recordcount, Set<String> fields, 
		       Vector<HashMap<String, ByteIterator>> result) {
        return Status.ERROR;
    }

    public Status update(String table, String key, HashMap<String, ByteIterator> values) {
        return insert(table, key, values);
    }

    public Status insert(String table, String key, HashMap<String, ByteIterator> values) {
        try {
	    put_num += 1;
	    client.insert(key, values);
        } catch (Exception e) {
            System.err.println(e);
            return Status.ERROR;
        }
        return Status.OK;
    }

    public Status delete(String table, String key)  {
        try {
	    put_num += 1;
	    client.delete(key);
        } catch (Exception e) {
            System.err.println(e);
	    e.printStackTrace();
            return Status.ERROR;
        }
        return Status.OK;
    }
    
    public int numOperationsQueued() {
        return Math.max(put_num, get_num);
    }

    public Status commit() {
	
        if (terminated) {
            return Status.ERROR;
	}

        try {
            if (put_num > 0) {
                try {
                    client.put_all();
                    if (get_num > 0) {
                        System.err.println("Whoa? Get-only and put only!");
                    }
                } catch (Exception e) {
                    System.err.println(e);
                    e.printStackTrace();
                    return Status.ERROR;
                }
            }
	    
            if (get_num > 0) {
                try {
		    client.get_all();
		    if (put_num > 0) {
			System.err.println("Whoa? Get-only and put only!");
		    }
		} catch (Exception e) {
		    System.err.println(e);
		    e.printStackTrace();
		    return Status.ERROR;
                }
            }

            return Status.OK;

        } finally {
	    client.clear();
	    put_num = 0;
	    get_num = 0;
        }
    }
}
