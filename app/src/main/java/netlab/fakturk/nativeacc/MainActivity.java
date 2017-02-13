package netlab.fakturk.nativeacc;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity
{
    float[] acc;

    // Used to load the 'native-lib' library on application startup.
    static
    {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        acc = new float[3];

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        sensorValue();
        acc = getAcc();
        System.out.println(acc[0]+", "+acc[1]+", "+acc[2]);
//        setAccXFromC(acc[0]);
//        setAccYFromC(acc[0]);
//        setAccZFromC(acc[0]);
    }

     void setAccX(float a)
    {


        System.out.print(a);
    }
    void setAccY(float a)
    {


        System.out.print(" "+a);
    }
    void setAccZ(float a)
    {


        System.out.println(" "+a);
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public  native void sensorValue();
    public native float[] getAcc();
    public native void setAccXFromC(float a);
    public native void setAccYFromC(float a);
    public native void setAccZFromC(float a);
}
