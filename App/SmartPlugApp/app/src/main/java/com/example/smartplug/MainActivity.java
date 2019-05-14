package com.example.smartplug;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.app.Activity;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.io.IOException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {


    private BluetoothAdapter btAdapter;
    private final int ENABLE_BT_REQ = 1;
    private Set<BluetoothDevice> nbDevices = new HashSet<>() ;
    private BluetoothSocket conn;
    private final BroadcastReceiver br = new BroadcastReceiver () {
        @Override
        public void onReceive (Context context , Intent intent ){
            BluetoothDevice device = null ;
            String action = intent.getAction();
            TextView text = findViewById(R.id.test);

            if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action)) {
                //discovery starts, we can show progress dialog or perform other tasks
                text.setText("Starting");
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                text.append("-Finished");
            } else if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device != null){
                    nbDevices.add((BluetoothDevice)device);
                    text.append("-"+device.getName());
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        nbDevices.clear();


        /*FloatingActionButton web = findViewById(R.id.web);

        final RequestQueue queue = Volley.newRequestQueue(this);
        final String url ="http://192.168.1.62:8000/";

        web.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                final TextView text = findViewById(R.id.test);
                text.append("hahahahahah");
                // Instantiate the RequestQueue.
                // Request a string response from the provided URL.
                StringRequest stringRequest = new StringRequest(Request.Method.GET, url,
                        new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {
                                // Display the first 500 characters of the response string.
                                text.setText("Response is: "+ response);
                            }
                        }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        text.setText("That didn't work!");
                    }
                });

// Add the request to the RequestQueue.
                queue.add(stringRequest);
            }
        });
*/
        Button  list = findViewById(R.id.list);
        list.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                TextView text = findViewById(R.id.test);
                text.append("Finding...");
                btAdapter.startDiscovery();
            }
        });
        Button pair = findViewById(R.id.pair);
        pair.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                TextView text = findViewById(R.id.test);
                text.setText("Heybby-");

                Iterator<BluetoothDevice> it = nbDevices.iterator();
                BluetoothDevice toConnect=null;
                while (it.hasNext()){
                    BluetoothDevice b = it.next();
                    String name= b.getName();
                    if(name != null && name.contains("DSD")) {
                        text.append("yeahce");
                        toConnect=b;
                    }
                }
                if(toConnect!=null){
                    try {
                        conn = toConnect.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
                        btAdapter.cancelDiscovery();
                        conn.connect();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                //btAdapter.startDiscovery();
            }
        });

        Button perm = findViewById(R.id.perm);
        perm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                getperm();
            }
        });
        Button wifi = findViewById(R.id.wifi);
        wifi.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+WS=Almawifi per poveri;SanVitale45;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button watt = findViewById(R.id.watt);
        watt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+DSW=100;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button routine = findViewById(R.id.routine);
        routine.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+DSR=0000111011010100;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button devon = findViewById(R.id.devon);
        devon.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+MM=1;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button devoff = findViewById(R.id.devoff);
        devoff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+MM=0;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button faston = findViewById(R.id.faston);
        faston.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+MF=1;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button fastoff = findViewById(R.id.fastoff);
        fastoff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+MF=0;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        Button server = findViewById(R.id.server);
        server.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Premuto bt", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                try {
                    TextView text = findViewById(R.id.test);
                    if(conn.isConnected()){
                        text.setText("yeboi");
                        conn.getOutputStream().write("BT+SS=10.12.13.14;8000;01;4242;-".getBytes());
                    }
                    else{
                        text.setText("noboi");
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });


        btAdapter = BluetoothAdapter.getDefaultAdapter();
        if( btAdapter == null ){
            Log.e(" MyApp ","BT is not available on this device ");
            finish () ;
        }
        if (! btAdapter . isEnabled () ){
            startActivityForResult (
                    new Intent( BluetoothAdapter.ACTION_REQUEST_ENABLE),ENABLE_BT_REQ );
        }
        IntentFilter filter = new IntentFilter();

        filter.addAction(BluetoothDevice.ACTION_FOUND);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);

        registerReceiver(br, filter);
        //registerReceiver( , new IntentFilter(BluetoothDevice.ACTION_FOUND));
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
    @Override
    public void onActivityResult (int reqID , int res , Intent data ){
        if( reqID == ENABLE_BT_REQ && res == Activity.RESULT_OK ){
            Snackbar.make(findViewById(android.R.id.content),"Attivato", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show();
        }
        if( reqID == ENABLE_BT_REQ && res == Activity.RESULT_CANCELED ){
            Snackbar.make(findViewById(android.R.id.content), "Non attivato", Snackbar.LENGTH_LONG)
                    .setAction("Action", null).show();
        }
    }
    @Override
    public void onStart () {
        super . onStart () ;

    }

    public void getperm(){

        TextView text = findViewById(R.id.test);
        text.setText("Pressed button");
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.ACCESS_COARSE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {

            // Permission is not granted
            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.ACCESS_COARSE_LOCATION)) {

                // Not to annoy user.
                Toast.makeText(this, "Permission must be granted to use the app.", Toast.LENGTH_SHORT).show();
            } else {

                // Request permission.
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},
                        1);
            }
        } else {
            // Permission has already been granted.
            Toast.makeText(this, "Permission already granted.", Toast.LENGTH_SHORT).show();

        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String permissions[], @NonNull int[] grantResults) {
        switch (requestCode) {
            case 1: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {

                    Toast.makeText(this, "Permission granted.", Toast.LENGTH_SHORT).show();

                    // Permission granted.
                } else {
                    Toast.makeText(this, "Permission must be granted to use the application.", Toast.LENGTH_SHORT).show();
                }
            }
        }
    }
    @Override
    public void onStop () {
        super . onStop () ;
        btAdapter . cancelDiscovery () ;
    }
}
