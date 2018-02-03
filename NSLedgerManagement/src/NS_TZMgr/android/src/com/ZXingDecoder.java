package NSCom;

import java.io.OutputStream;
import java.io.FileOutputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.File;
import java.util.Iterator;
import java.util.Hashtable;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.os.Environment;

import com.google.zxing.BarcodeFormat;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.DecodeHintType;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.Result;
import com.google.zxing.WriterException;
import com.google.zxing.Binarizer;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.common.GlobalHistogramBinarizer;
import com.google.zxing.LuminanceSource;
import com.google.zxing.RGBLuminanceSource;
import com.google.zxing.PlanarYUVLuminanceSource;

public class ZXingDecoder
{
    private static final String CHARSET = "utf-8";
    private static final int BLACK = 0xFF000000;
    private static final int WHITE = 0xFFFFFFFF;
    private final MultiFormatReader reader;

    public ZXingDecoder()
    {
        reader = new MultiFormatReader();

        // 解码设置编码方式为：utf-8
        Hashtable hints = new Hashtable();
        hints.put(DecodeHintType.CHARACTER_SET, CHARSET);

        // 设置解码格式为QR_CODE
        //List<BarcodeFormat> formats = new ArrayList<BarcodeFormat>();
        //formats.add(BarcodeFormat.QR_CODE);
        //hints.put(DecodeHintType.POSSIBLE_FORMATS, formats);

        //Map<DecodeHintType, Object> hints = new HashMap<DecodeHintType, Object>();
        //List<BarcodeFormat> formats = new ArrayList<BarcodeFormat>();
        //formats.add(BarcodeFormat.QR_CODE);
        //hints.put(DecodeHintType.CHARACTER_SET, CHARSET);
        //hints.put(DecodeHintType.POSSIBLE_FORMATS, formats);

        reader.setHints(hints);
    }

    public String decodeRGBData(int[] data, int w, int h)
    {
        //System.out.println("decodeRGBData 1");
        try
        {
            LuminanceSource source = new RGBLuminanceSource(w, h, data);
            BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
            Result result = reader.decodeWithState(bitmap);

            // try to save the image:
            /*
            try
            {
                File file = new File(Environment.getExternalStorageDirectory().toString()+File.separator+"01.jpg");
                OutputStream fOut = new FileOutputStream(file);
                Bitmap img = Bitmap.createBitmap(data, w, h, Bitmap.Config.ARGB_8888);
                img.compress(Bitmap.CompressFormat.JPEG, 100, fOut); // saving the Bitmap to a file compressed as a JPEG with 100% compression rate
                fOut.flush();
                fOut.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
            */

            return result.getText();
        }
        catch (Exception e)
        {
            //e.printStackTrace();
        }

        return null;
    }

    public String decodeYUVData(byte[] data, int w, int h)
    {
        //System.out.println("data.length="+data.length+",w="+w+",h="+h);
        try
        {
            PlanarYUVLuminanceSource source = new PlanarYUVLuminanceSource(data, w, h, 0, 0, w, h, false);
            BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));
            Result result = reader.decodeWithState(bitmap);

            return result.getText();
        }
        catch (Exception e)
        {
            //e.printStackTrace();
        }

        return null;
    }
}
