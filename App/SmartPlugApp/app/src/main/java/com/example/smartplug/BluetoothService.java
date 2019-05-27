package com.example.smartplug;

import android.app.Service;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;

import java.io.IOException;

public class BluetoothService extends Service {

    private BluetoothSocket conn;
    private IBinder mBinder = new BtBinder();

    public void setupSocket(BluetoothSocket in){
        conn=in;
    }
    public boolean connect(){
        try {
            conn.connect();
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }
    public void close(){
        try {
            conn.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public boolean isConnected(){
        return conn.isConnected();
    }
    public void write(String in){
        try {
            conn.getOutputStream().write(in.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public String read(){
        byte[] buffer = new byte[1024];
        int bytes=0;
        try {
            if(conn.getInputStream().available()>0){
                bytes = conn.getInputStream().read(buffer);
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
        String readMessage = new String(buffer, 0, bytes);
        return readMessage;
    }
    public BluetoothService() {
    }


    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }
    @Override
    public void onRebind(Intent intent) {
        super.onRebind(intent);
    }

    @Override
    public boolean onUnbind(Intent intent) {
        return true;
    }
    public class BtBinder extends Binder {
        BluetoothService getService() {
            return BluetoothService.this;
        }
    }
}
