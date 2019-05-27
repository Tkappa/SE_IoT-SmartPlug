package com.example.smartplug;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.IBinder;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.util.UUID;



public class MainActivity extends AppCompatActivity {

    private BluetoothAdapter btAdapter;
    private final int ENABLE_BT_REQ = 1;
    private Set<BluetoothDevice> nbDevices = new HashSet<>() ;

    BluetoothService mBoundService;
    boolean mServiceBound = false;
    LinearLayout ll ;

    //Used to bind the service to this Activity
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

    //Used to find the devices
    private final BroadcastReceiver br = new BroadcastReceiver () {
        @Override
        public void onReceive (Context context , Intent intent ){
            BluetoothDevice device = null ;
            String action = intent.getAction();
            TextView text = findViewById(R.id.test);

            if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action)) {
                text.setText("Discovering devices...");
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
                text.setText("Finished discovering devices");
            } else if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                //Creates dinamic buttons, one for each device with a name found
                if (device != null){
                    if(device.getName()!=null) {
                        nbDevices.add((BluetoothDevice) device);
                        Button btn = new Button(getBaseContext());
                        btn.setId(nbDevices.size());
                        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                                LinearLayout.LayoutParams.MATCH_PARENT,
                                LinearLayout.LayoutParams.WRAP_CONTENT);
                        final int id_ = btn.getId();

                        //Creates a button and sets its behaviour on click
                        btn.setText(device.getName());
                        ll.addView(btn, params);
                        Button btn1 = findViewById(id_);
                        btn1.setOnClickListener(new View.OnClickListener() {
                            public void onClick(View view) {
                                Iterator<BluetoothDevice> it = nbDevices.iterator();
                                BluetoothDevice toConnect=null;
                                //Finds the desired device in the list
                                while (it.hasNext()){
                                    BluetoothDevice b = it.next();
                                    String name= b.getName();
                                    if(name != null && name.contains(((Button) view).getText())) {
                                        toConnect=b;
                                    }
                                }
                                //If the button pressed has a valid devices tries to pair with it
                                if(toConnect!=null){
                                    try {
                                        mBoundService.setupSocket(toConnect.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")));
                                        btAdapter.cancelDiscovery();
                                        if(mBoundService.connect()){
                                            unbindService(mServiceConnection);
                                            Intent intent = new Intent(getBaseContext(), DeviceInteraction.class);
                                            startActivity(intent);
                                        }
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                    }
                                }
                            }
                        });
                    }
                    //text.append("-"+);
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
        ll= findViewById(R.id.layout);


        //Binds the activity with the service
        Intent serviceIntent = new Intent(this, BluetoothService.class);
        startService(serviceIntent);
        bindService(serviceIntent, mServiceConnection, Context.BIND_AUTO_CREATE);

        //Sets the list button behaviour
        Button list = findViewById(R.id.list);
        list.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //Cleans all old buttons
                if(ll.getChildCount() > 0)
                    ll.removeAllViews();
                //Gets the required permissions
                getperm();
                TextView text = findViewById(R.id.test);
                text.setText("Finding...");
                //Starts the discovery
                btAdapter.startDiscovery();
            }
        });

        btAdapter = BluetoothAdapter.getDefaultAdapter();
        if( btAdapter == null ){
            TextView text = findViewById(R.id.test);
            text.setText("BT is not available on this device");
            list.setEnabled(false);
        }
        if (! btAdapter . isEnabled () ){
            startActivityForResult (
                    new Intent( BluetoothAdapter.ACTION_REQUEST_ENABLE),ENABLE_BT_REQ );
        }

        //Registers the BT actions
        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothDevice.ACTION_FOUND);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);

        registerReceiver(br, filter);
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

        //Opens the setting menu if clicked
        if (id == R.id.action_settings) {
            unbindService(mServiceConnection);
            Intent intent = new Intent(this, ServerSettings.class);
            startActivity(intent);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
    @Override
    public void onActivityResult (int reqID , int res , Intent data ){
        if( reqID == ENABLE_BT_REQ && res == Activity.RESULT_OK ){
            //Activated BT
        }
        if( reqID == ENABLE_BT_REQ && res == Activity.RESULT_CANCELED ){
            //Didnt activate BT
        }
    }
    @Override
    public void onStart () {
        super . onStart () ;

    }

    public void getperm(){
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.ACCESS_COARSE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {

            // Permission is not granted
            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.ACCESS_COARSE_LOCATION)) {

                Toast.makeText(this, "Permission must be granted to use the app.", Toast.LENGTH_SHORT).show();
            } else {

                // Request permission.
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},
                        1);
            }
        } else {
            // Permission has already been granted.
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
