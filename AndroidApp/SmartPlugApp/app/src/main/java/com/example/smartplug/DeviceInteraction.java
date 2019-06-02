package com.example.smartplug;

import android.bluetooth.BluetoothDevice;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.IBinder;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.Iterator;
import java.util.UUID;

public class DeviceInteraction extends AppCompatActivity {


    BluetoothService mBoundService;
    boolean mServiceBound = false;
    public static final String PREFERENCE_FILENAME = "ServerSettingsPrefs";

    private CountDownTimer timer;


    private ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mServiceBound = false;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            BluetoothService.BtBinder myBinder = (BluetoothService.BtBinder) service;
            mBoundService = myBinder.getService();
            mServiceBound = true;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_interaction);
        Toolbar toolbar = findViewById(R.id.toolbar);
        final RequestQueue queue = Volley.newRequestQueue(this);
        final TextView readingtxt = findViewById(R.id.reading);
        timer = new CountDownTimer(1000, 500) {

            @Override
            public void onTick(long millisUntilFinished) {

            }
            @Override
            public void onFinish() {
                try{
                    String reading = mBoundService.read();
                    String[] separate= reading.split(";");
                    readingtxt.setText(separate[0]);
                    timer.start();
                }catch(Exception e){
                    e.printStackTrace();
                }
            }
        };

        setSupportActionBar(toolbar);

        Intent serviceIntent = new Intent(this, BluetoothService.class);
        startService(serviceIntent);
        bindService(serviceIntent, mServiceConnection, Context.BIND_AUTO_CREATE);


        final SharedPreferences serverSettingsP = getSharedPreferences(PREFERENCE_FILENAME,MODE_PRIVATE);
        final String serverIp=serverSettingsP.getString("ServerIp","");
        final String serverpsw=serverSettingsP.getString("ServerPsw","");
        final String serveraddr=serverSettingsP.getString("ServerAddr","");
        final String serverport=serverSettingsP.getString("ServerPort","");


        Button devon = findViewById(R.id.devon);
        devon.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                try {
                    if(mBoundService.isConnected()){
                        mBoundService.write("BT+MM=1;-");
                        Toast.makeText(getBaseContext(), "Sent command", Toast.LENGTH_SHORT).show();
                    }
                    else{

                    }
                }
                catch (Exception ex){

                }
            }
        });

        Button devoff = findViewById(R.id.devoff);
        devoff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {

                    if(mBoundService.isConnected()){

                        mBoundService.write("BT+MM=0;-");
                        Toast.makeText(getBaseContext(), "Sent command", Toast.LENGTH_SHORT).show();
                    }
                    else{

                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        Button wifi = findViewById(R.id.wifi);
        wifi.setEnabled(false);
        wifi.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {

                    if(mBoundService.isConnected()){

                        mBoundService.write("BT+WS="+serverIp+";"+serverpsw+";-");
                        Toast.makeText(getBaseContext(), "Sent command", Toast.LENGTH_SHORT).show();
                    }
                    else{

                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        Button server = findViewById(R.id.server);
        server.setEnabled(false);
        server.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View view) {
                try {
                    StringRequest stringRequest = new StringRequest(Request.Method.GET, "http://"+serveraddr + ":" + serverport+"/newdevice",
                            new Response.Listener<String>() {
                                @Override
                                public void onResponse(String response) {
                                    try {
                                        JSONObject reader = new JSONObject(response);
                                        if(mBoundService.isConnected()){
                                            mBoundService.write("BT+SS="+serveraddr+";"+serverport+";"+reader.getString("id")+";"+reader.getString("key")+";-");
                                            Toast.makeText(getBaseContext(), "Sent command", Toast.LENGTH_SHORT).show();
                                        }
                                        else{

                                        }
                                    } catch (JSONException e) {
                                        e.printStackTrace();
                                    }

                                }
                            }, new Response.ErrorListener() {
                                    @Override
                                    public void onErrorResponse(VolleyError error) {
                                        Toast.makeText(getBaseContext(), "Could not connect to server, command not sent", Toast.LENGTH_LONG).show();
                                    }
                            });

                    queue.add(stringRequest);

                }catch (Exception e){

                }
            }
        });

        Button watt = findViewById(R.id.watt);
        watt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                EditText wattn = findViewById(R.id.wattage);
                try {
                    if(mBoundService.isConnected()){
                        mBoundService.write("BT+DSW="+wattn.getText().toString()+";-");
                        Toast.makeText(getBaseContext(), "Sent command", Toast.LENGTH_SHORT).show();
                    }
                    else{
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        Button faston = findViewById(R.id.faston);
        faston.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    if(mBoundService.isConnected()){
                        mBoundService.write("BT+MF=1;-");
                        timer.start();
                        Toast.makeText(getBaseContext(), "Sent command", Toast.LENGTH_SHORT).show();
                    }
                    else{
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        Button fastoff = findViewById(R.id.fastoff);
        fastoff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    if(mBoundService.isConnected()){
                        mBoundService.write("BT+MF=0;-");
                        timer.cancel();
                        readingtxt.setText("Enable fast mode to get readings from the device");
                        Toast.makeText(getBaseContext(), "Sent command", Toast.LENGTH_SHORT).show();
                    }
                    else{
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });


        //Checks it it has the settings required to send the server pairing commands
        if(serverIp !="" && serverpsw !=""){
            wifi.setEnabled(true);
            if(serveraddr!="" && serverport!=""){
                server.setEnabled(true);
            }
        }

    }

    @Override
    public void onStop() {
        mBoundService.close();
        unbindService(mServiceConnection);
        super.onStop();
    }

}
