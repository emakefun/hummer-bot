package com.keywish.blutooth.test;

import java.util.Timer;
import java.util.TimerTask;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothAdapter.LeScanCallback;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v4.widget.SwipeRefreshLayout.OnRefreshListener;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

import com.keywish.blutooth.adapter.BleDeviceListAdapter;
import com.keywish.blutooth.utils.Utils;
import com.keywish.blutooth.test.R;


public class MainActivity extends AppCompatActivity {
    ListView listView;
    SwipeRefreshLayout swagLayout;
    BluetoothAdapter mBluetoothAdapter;
    private LeScanCallback mLeScanCallback;
    private static final String TAG = "BleTools_MainActivity";
    BleDeviceListAdapter mBleDeviceListAdapter;
    boolean isExit;
    Handler handler;
    private int REQUEST_ACCESS_COARSE_LOCATION=1;
    SharedPreferences sharedPreferences;
    Editor editor;
  //  @SuppressLint("NewApi")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(Build.VERSION.SDK_INT>=23){
            if (ContextCompat.checkSelfPermission(MainActivity.this,
                    Manifest.permission.ACCESS_COARSE_LOCATION)!= PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this,new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},
                        REQUEST_ACCESS_COARSE_LOCATION);
                if(ActivityCompat.shouldShowRequestPermissionRationale(this,
                        Manifest.permission.READ_CONTACTS)) {
                    Toast.makeText(MainActivity.this,"shouldShowRequestPermissionRationale", Toast.LENGTH_SHORT).show();
                }
            }
        }
        Log.d(TAG, "onCreate: ");
        // set no title
        // requestWindowFeature(Window.FEATURE_NO_TITLE);
        // set full screnn
        // getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        // set keep screen on
        // getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        getSupportActionBar().setTitle(R.string.app_title);
        setContentView(R.layout.activity_main);

        sharedPreferences = getPreferences(0);
        editor = sharedPreferences.edit();
        getBleAdapter();
        init();
        getScanResualt();
        new Thread(new Runnable() {
            @SuppressWarnings("deprecation")
            @Override
            public void run() {
                // TODO Auto-generated method stub
                mBluetoothAdapter.startLeScan(mLeScanCallback);
            }
        }).start();
    }

    private void init() {
        // TODO Auto-generated method stub
        listView = (ListView) findViewById(R.id.lv_deviceList);
        listView.setEmptyView(findViewById(R.id.pb_empty));
        swagLayout = (SwipeRefreshLayout) findViewById(R.id.swagLayout);
        swagLayout.setVisibility(View.VISIBLE);
        swagLayout.setOnRefreshListener(new OnRefreshListener() {

        @SuppressWarnings("deprecation")
        @SuppressLint("NewApi")
        @Override
        public void onRefresh() {
            // TODO Auto-generated method stub
            mBleDeviceListAdapter.clear();
            mBluetoothAdapter.startLeScan(mLeScanCallback);
            swagLayout.setRefreshing(false);
        }
        });
        mBleDeviceListAdapter = new BleDeviceListAdapter(this);
        listView.setAdapter(mBleDeviceListAdapter);
        setListItemListener();
    }

    @SuppressLint("NewApi")
    private void getBleAdapter() {
        final BluetoothManager bluetoothManager = (BluetoothManager) this
                .getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();
    }

    @SuppressLint("NewApi")
    private void getScanResualt() {
        mLeScanCallback = new LeScanCallback() {
            @Override
            public void onLeScan(final BluetoothDevice device, final int rssi,
                                 final byte[] scanRecord) {
                MainActivity.this.runOnUiThread(new Runnable() {
                    public void run() {
                        mBleDeviceListAdapter.addDevice(device, rssi,
                                Utils.bytesToHex(scanRecord));
                        mBleDeviceListAdapter.notifyDataSetChanged();
                        invalidateOptionsMenu();
                    }
                });
            }
        };
    }

    private void setListItemListener() {
        listView.setOnItemClickListener(new OnItemClickListener() {
            @SuppressLint("NewApi")
            @Override
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {
                // TODO Auto-generated method stub
                BluetoothDevice device = mBleDeviceListAdapter
                        .getDevice(position);
                final Intent intent = new Intent(MainActivity.this,
                        DeviceConnect.class);
                intent.putExtra(DeviceConnect.EXTRAS_DEVICE_NAME,
                        device.getName());
                intent.putExtra(DeviceConnect.EXTRAS_DEVICE_ADDRESS,
                        device.getAddress());
                startActivity(intent);
            }
        });
    }

    @SuppressLint("NewApi")
    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
    }

    @SuppressWarnings("deprecation")
    @SuppressLint("NewApi")
    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        mBluetoothAdapter.stopLeScan(mLeScanCallback);
        mBleDeviceListAdapter.clear();
        mBluetoothAdapter.cancelDiscovery();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        menu.getItem(0).setTitle(getResources().getString(R.string.total1) + ' ' + mBleDeviceListAdapter.getCount() + ' ' + getResources().getString(R.string.unit1));
        return true;
    }

    @SuppressWarnings("deprecation")
    @SuppressLint("NewApi")
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_stop:
                mBluetoothAdapter.stopLeScan(mLeScanCallback);
                break;
            case R.id.menu_autoconnect:
        		if (sharedPreferences.getBoolean("AutoConnect", true)) {
                    editor.putBoolean("AutoConnect", false);
                    editor.commit();
                    Toast.makeText(this, R.string.Cancel_automatic_connection, Toast.LENGTH_SHORT).show();
                } else {
                    editor.putBoolean("AutoConnect", true);
                    editor.commit();
                    Toast.makeText(this, R.string.Automatically_set_to_disconnected, Toast.LENGTH_SHORT).show();
                }
                break;
            case R.id.menu_about:
                MainActivity.this.startActivity(new Intent(this,
                        AboutActivity.class));
                break;
            case R.id.menu_qrcode:
                MainActivity.this.startActivity(new Intent(this,
                        QrcodeActivity.class));
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            exitBy2Click();
        }
        return false;
    }

    private void exitBy2Click() {
        if (!isExit) {
            isExit = true;
            Toast.makeText(this, R.string.Press_again_to_exit, Toast.LENGTH_SHORT).show();
            new Timer().schedule(new TimerTask() {
                public void run() {
                    isExit = false;
                }
            }, 2000);
        } else {
            onDestroy();
            finish();
            System.exit(0);
        }
    }
}
