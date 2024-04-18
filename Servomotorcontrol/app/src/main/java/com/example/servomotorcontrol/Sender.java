package com.example.servomotorcontrol;

import android.os.AsyncTask;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

public class Sender extends AsyncTask<Integer, Void, Void> {

    Socket socket;
    DataOutputStream dos;
    PrintWriter pw;

    @Override
    protected Void doInBackground(Integer... voids) {

        int x = voids[0];

        try{
            socket = new Socket(MainActivity.addr, MainActivity.PORT);
            pw = new PrintWriter(socket.getOutputStream());
            pw.write(x);
            pw.flush();
            pw.close();
            socket.close();
        }
        catch (IOException e){
            e.printStackTrace();
        }

        return null;
    }
}
