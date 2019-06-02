package com.example.smartplug;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;


public class ServerSettings extends AppCompatActivity {
    public static final String PREFERENCE_FILENAME = "ServerSettingsPrefs";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_server_settings);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        final RequestQueue queue = Volley.newRequestQueue(this);

        //Gets the saved settings from the prefences and puts them in the right areas
        final SharedPreferences serverSettingsP = getSharedPreferences(PREFERENCE_FILENAME,MODE_PRIVATE);
        String serverIp=serverSettingsP.getString("ServerIp","");
        String serverpsw=serverSettingsP.getString("ServerPsw","");
        String serveraddr=serverSettingsP.getString("ServerAddr","");
        String serverport=serverSettingsP.getString("ServerPort","");

        final EditText ssid = findViewById(R.id.ssid);
        ssid.setText(serverIp);
        final EditText psw = findViewById(R.id.wifipassword);
        psw.setText(serverpsw);
        final EditText addr = findViewById(R.id.serverip);
        addr.setText(serveraddr);
        final EditText port = findViewById(R.id.serverport);
        port.setText(serverport);



        //Saves the settings. Its disable because you need to check if the settings are right before saving them
        final Button savebtn = findViewById(R.id.save);
        savebtn.setEnabled(false);
        savebtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SharedPreferences.Editor prefEditor = serverSettingsP.edit();
                prefEditor.putString("ServerIp",ssid.getText().toString());
                prefEditor.putString("ServerPsw",psw.getText().toString());
                prefEditor.putString("ServerAddr",addr.getText().toString());
                prefEditor.putString("ServerPort",port.getText().toString());
                prefEditor.commit();
                Snackbar.make(view, "Settings saved", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                kill_activity();
            }
        });

        //Checks if the settings are right by pinging the specified server
        Button pingbtn = findViewById(R.id.ping);
        pingbtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View view) {
                // Instantiate the RequestQueue.
                // Request a string response from the provided URL.
                StringRequest stringRequest = new StringRequest(Request.Method.GET, "http://"+addr.getText().toString() + ":" + port.getText().toString(),
                        new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {
                                Snackbar.make(view, "Ping succesful", Snackbar.LENGTH_LONG)
                                        .setAction("Action", null).show();
                                savebtn.setEnabled(true);
                            }
                        }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Snackbar.make(view, "Could not ping, : "+ error.getMessage(), Snackbar.LENGTH_LONG)
                                .setAction("Action", null).show();
                    }
                });

                // Add the request to the RequestQueue.
                queue.add(stringRequest);
            }
        });

    }
    void kill_activity()
    {
        finish();
    }
}
