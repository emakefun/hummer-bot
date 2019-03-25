package com.keywish.blutooth.adapter;

/**
 * Created by ken on 2018/3/23.
 */
import java.util.ArrayList;

import com.keywish.blutooth.utils.Utils;
import com.keywish.blutooth.test.R;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
public class BleDeviceListAdapter extends BaseAdapter {
    private LayoutInflater mInflater;
    private ArrayList<BluetoothDevice> mLeDevices;
    private ArrayList<Integer> RSSIs;
    private ArrayList<String> scanRecords;

    public BleDeviceListAdapter(Context context) {
        mLeDevices = new ArrayList<BluetoothDevice>();
        RSSIs = new ArrayList<Integer>();
        scanRecords = new ArrayList<String>();
        this.mInflater = LayoutInflater.from(context);
    }

    public void addDevice(BluetoothDevice device, int RSSI, String scanRecord) {
        if (!mLeDevices.contains(device)) {
            this.mLeDevices.add(device);
            this.RSSIs.add(RSSI);
            this.scanRecords.add(scanRecord);
        } else {
            for (int i = 0; i < mLeDevices.size(); i++) {
                BluetoothDevice d = mLeDevices.get(i);
                if (device.getAddress().equals(d.getAddress())) {
                    RSSIs.set(i, RSSI);
                    scanRecords.set(i, scanRecord);
                }
            }
        }
    }

    public BluetoothDevice getDevice(int position) {
        // TODO Auto-generated method stub
        return mLeDevices.get(position);
    }

    @Override
    public int getCount() {
        // TODO Auto-generated method stub
        return mLeDevices.size();
    }

    @Override
    public Object getItem(int arg0) {
        // TODO Auto-generated method stub
        return mLeDevices.get(arg0);
    }

    @Override
    public long getItemId(int arg0) {
        // TODO Auto-generated method stub
        return 0;
    }

    @SuppressLint("InflateParams")
    @Override
    public View getView(int position, View view, ViewGroup arg2) {
        // TODO Auto-generated method stub

        ViewHolder viewholder;
        if (view == null) {
            view = mInflater.inflate(R.layout.item_devicelist, null);
            viewholder = new ViewHolder();
            viewholder.devicename = (TextView) view
                    .findViewById(R.id.tv_devicelist_name);
            viewholder.deviceAddress = (TextView) view
                    .findViewById(R.id.tv_devicelist_address);
            viewholder.deviceRSSI = (TextView) view
                    .findViewById(R.id.tv_devicelist_rssi);
            viewholder.devicerecord = (TextView) view
                    .findViewById(R.id.tv_devicelist_scanRecord);
            viewholder.devicerecord_name = (TextView) view
                    .findViewById(R.id.tv_devicelist_scanRecord_name);
            view.setTag(viewholder);
        } else {
            viewholder = (ViewHolder) view.getTag();
        }
        String name = mLeDevices.get(position).getName();
        if (name != null)
            viewholder.devicename.setText(name);
        else
            viewholder.devicename.setText("Unknow Device");
        String address_str = Integer.toHexString(R.string.address);
        String signal_str = Integer.toHexString(R.string.signal);
        String Broadcast_package_str = Integer.toHexString(R.string.Broadcast_package);
        String Broadcast_package_name_str = Integer.toHexString(R.string.Broadcast_package_name);
        viewholder.deviceAddress.setText(address_str + mLeDevices.get(position).getAddress());
        viewholder.deviceRSSI.setText(signal_str + RSSIs.get(position).toString());
        viewholder.devicerecord.setText(Broadcast_package_str+ "\n"+ scanRecords.get(position));
        viewholder.devicerecord_name.setText(Broadcast_package_name_str + Utils.ParseScanRecord(scanRecords.get(position)));
        return view;
    }

    static class ViewHolder {
        TextView devicename;
        TextView deviceAddress;
        TextView deviceRSSI;
        TextView devicerecord;
        TextView devicerecord_name;
    }

    public void clear() {
        // TODO Auto-generated method stub
        mLeDevices.clear();
        RSSIs.clear();
        scanRecords.clear();
        this.notifyDataSetChanged();
    }

}