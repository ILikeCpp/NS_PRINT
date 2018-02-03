package an.qt.android;

import com.dothantech.printer.IDzPrinter;
import com.dothantech.printer.IDzPrinter.*;
import com.dothantech.lpapi.IAtBitmap;
import com.dothantech.lpapi.LPAPI;
import com.dothantech.lpapi.LPAPI.Factory;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;

public class NSPrinter//qt的通知
{
    //定义native方法
    public static native void onStateChange_qt(String s);
    public static native void onPrintProgress_qt(String s);
    public static native void onProgressInfo_qt(String s);

// LPAPI 打印机操作相关的回调函数。
    private final LPAPI.Callback mCallback = new LPAPI.Callback() {

        /****************************************************************************************************************************************/
        // 所有回调函数都是在打印线程中被调用，因此如果需要刷新界面，需要发送消息给界面主线程，以避免互斥等繁琐操作。
        /****************************************************************************************************************************************/

        // 打印机连接状态发生变化时被调用
        @Override
        public void onStateChange(PrinterAddress arg0, PrinterState arg1) {
            final PrinterAddress printer = arg0;
            String s;
            switch (arg1) {
            case Disconnected:
               s = "打印机连接失败";
               onStateChange_qt(s);
               break;
            }
        }

        // 打印标签的进度发生变化是被调用
        @Override
        public void onPrintProgress(PrinterAddress address, Object bitmapData, PrintProgress progress, Object addiInfo) {
            String s;
            switch (progress) {
            case Success:
               s = "Success";
               onPrintProgress_qt(s);
               break;
            case Failed:
               s = addiInfo.toString();
               onPrintProgress_qt(s);
               break;
            }
        }

        // 蓝牙适配器状态发生变化时被调用
        @Override
        public void onProgressInfo(ProgressInfo arg0, Object arg1) {
            String s = arg0.toString();
            onProgressInfo_qt(s);
        }

        @Override
        public void onPrinterDiscovery(PrinterAddress arg0, PrinterInfo arg1) {
        }
    };

    private LPAPI api;

    public NSPrinter(){
        this.api = LPAPI.Factory.createInstance(mCallback);
    }

    public boolean isPaired(){//是否配对打印机
        String str = this.api.getAllPrinters();
        if(str == null || str.length() <= 0){
            return false;
        }
        return true;
    }

    public boolean openPrinter(){//连接打印机
        if(!isPrinterOpened()){
            return this.api.openPrinterSync(null);//同步方式打开
        }
        return true;
    }

    public String getPrinterName(){//获得打印机名称
        this.api.openPrinter(null);
        return this.api.getPrinterName();
    }

    public boolean isPrinterOpened(){//判断是否已连接打印机
        return this.api.isPrinterOpened();
    }

    public void closePrinter(){//关闭打印机
        this.api.closePrinter();
    }

    public void printerQuit(){//断开蓝牙连接
        // 断开蓝牙连接，释放 LPAPI 对象
        this.api.quit();
    }

    public void cancel(){
        this.api.cancel();
    }

    public void printTextOnClick() {//打印文本
        printText("国电南思");
    }

    public void print1DBarcode(){//打印一维码
        printText1DBarcode("南京国电南思","12345678",null);
    }

    public void printQRCode1(String bdzmc, String zzmc, String sbsbm, String dlgsmc, String dyrq){//打印二维码
        print2dBarcode_new1(bdzmc,zzmc,sbsbm,dlgsmc,dyrq,null);
    }

    public void printQRCode2(String code, String dlgsmc, String dyrq){//打印二维码
        print2dBarcode_new2(code,dlgsmc,dyrq,null);
    }

    public void printQRCode22(String code1, String dlgsmc1, String dyrq1, String code2, String dlgsmc2, String dyrq2){//打印二维码
        print2dBarcode_new22(code1,dlgsmc1,dyrq1,code2,dlgsmc2,dyrq2,null);
    }

    private void printText(String content) {

        // 开始绘图任务，传入参数(页面宽度, 页面高度)
        this.api.startJob(40, 30, 0);

        // 开始一个页面的绘制，绘制文本字符串
        this.api.drawText(content, 4, 5, 40, 30, 4, IAtBitmap.FontStyle.REGULAR);

        // 结束绘图任务提交打印
        this.api.commitJob();
    }

    // 打印文本一维码
    private void printText1DBarcode(String text, String onedBarcde, Bundle param) {

        // 开始绘图任务，传入参数(页面宽度, 页面高度)
        this.api.startJob(40, 30, 0);

        // 开始一个页面的绘制，绘制文本字符串
        // 传入参数(需要绘制的文本字符串, 绘制的文本框左上角水平位置, 绘制的文本框左上角垂直位置, 绘制的文本框水平宽度, 绘制的文本框垂直高度, 文字大小, 字体风格)
//            api.drawText(text, 4, 4, 40, 20, 4, LPAPI.FontStyle.REGULAR);

        // 设置之后绘制的对象内容旋转180度
//            api.setItemOrientation(180);

        // 绘制一维码，此一维码绘制时内容会旋转180度，
        // 传入参数(需要绘制的一维码的数据, 绘制的一维码左上角水平位置, 绘制的一维码左上角垂直位置, 绘制的一维码水平宽度, 绘制的一维码垂直高度)
        this.api.draw1DBarcode(onedBarcde, LPAPI.BarcodeType.AUTO, 0, 10, 30, 10, 3);

        // 结束绘图任务提交打印
        this.api.commitJob();
    }

    // 打印二维码
    private boolean print2dBarcode(String bdzmc, String zzmc, String sbsbm, String dlgsmc, String dyrq, Bundle param) {
        // 开始绘图任务，传入参数(页面宽度, 页面高度)
        //宽度、高度、旋转角度
        this.api.startJob(105, 25, 270);

        //分两部分0-45 45-105

        //第一部分
        //文字
        this.api.drawText(bdzmc,0,2,45,0,2.47,IAtBitmap.FontStyle.REGULAR);
        //分割线 起点坐标 终点坐标
        this.api.drawLine(0,8.94,45,8.94,0.1);
        //文字
        this.api.drawText(zzmc,0,9.95,45,0,2.47,IAtBitmap.FontStyle.REGULAR);
        //分割线
        this.api.drawLine(0,17.89,45,17.89,0.1);
        //文字
        this.api.drawText(sbsbm, 0, 18.90,45,0,2.47, IAtBitmap.FontStyle.REGULAR);

        //第二部分
        //国网图标45-75 高度是10
        //矩形框 坐标 宽度 高度
        this.api.drawText("这里是国网图标",46,2,29,0,2.82,IAtBitmap.FontStyle.REGULAR);
        this.api.drawLine(46,10,75,10,0.1);
        //文字
        this.api.drawText(sbsbm,46,10,29,0,2.2,IAtBitmap.FontStyle.REGULAR);
        //矩形框 坐标 宽度 高度
        this.api.drawRectangle(46,15.6,29,9.1,0.2);
        //文字
        this.api.drawText(dlgsmc,47,16,27,0,2.2,IAtBitmap.FontStyle.REGULAR);
        //矩形中间画一条线
        this.api.drawLine(46,22,75,22,0.1);
        //文字
        this.api.drawText(dyrq,47,22.3,27,0,2.2,IAtBitmap.FontStyle.REGULAR);
        //二维码 76-105
        this.api.draw2DQRCode(sbsbm, 77, 0,25);

        // 结束绘图任务提交打印
        return api.commitJob();
    }

    //新式二维码
    private boolean print2dBarcode_new1(String bdzmc, String zzmc, String sbsbm, String dlgsmc, String dyrq, Bundle param) {
        // 开始绘图任务，传入参数(页面宽度, 页面高度)
        //宽度、高度、旋转角度
        this.api.startJob(50, 50, 270);

        //以高度分两部分 0-20 30-50

        //第一部分
        //文字
        this.api.drawText(bdzmc,0,0,50,0,2.47,IAtBitmap.FontStyle.REGULAR);
//        //分割线 起点坐标 终点坐标
        this.api.drawLine(0,6,50,6,0.1);
//        //文字
        this.api.drawText(zzmc,0,7,50,0,2.47,IAtBitmap.FontStyle.REGULAR);
//        //分割线
        this.api.drawLine(0,14,50,14,0.1);
//        //文字
        this.api.drawText(sbsbm, 0,16,50,16,2.47, IAtBitmap.FontStyle.REGULAR);

//        //第二部分
//        //文字
//        api.setItemOrientation(270);
        this.api.drawText(sbsbm,1,37,25,0,2.2,IAtBitmap.FontStyle.REGULAR);

//        //矩形框 坐标 宽度 高度
        this.api.drawRectangle(1,40,25,9,0.2);
//        //文字
        this.api.drawText(dlgsmc,1,41,25,0,2.2,IAtBitmap.FontStyle.REGULAR);
//        //矩形中间画一条线
        this.api.drawLine(1,44,26,44,0.15);
//        //文字
//        api.setItemOrientation(270);
        this.api.drawText(dyrq,1,45,25,0,2.2,IAtBitmap.FontStyle.REGULAR);

        //二维码
//        api.setItemOrientation(0);
        this.api.draw2DQRCode(sbsbm, 26,26,20);

        // 结束绘图任务提交打印
        return api.commitJob();
    }

    private boolean print2dBarcode_new2(String code, String dlgsmc, String dyrq, Bundle param) {
        // 开始绘图任务，传入参数(页面宽度, 页面高度)
        //宽度、高度、旋转角度
        this.api.startJob(50, 50, 270);

        //以高度分两部分 0-20 30-50

        //第一部分
        //文字
//        this.api.drawText(bdzmc,0,0,50,0,2.47,IAtBitmap.FontStyle.REGULAR);
    //        //分割线 起点坐标 终点坐标
//        this.api.drawLine(0,6,50,6,0.1);
    //        //文字
//        this.api.drawText(zzmc,0,7,50,0,2.47,IAtBitmap.FontStyle.REGULAR);
    //        //分割线
//        this.api.drawLine(0,14,50,14,0.1);
    //        //文字
//        this.api.drawText(sbsbm, 0,16,50,16,2.47, IAtBitmap.FontStyle.REGULAR);

    //        //第二部分
    //        //文字
    //        api.setItemOrientation(270);
        this.api.drawText(code,1,37,25,0,2.2,IAtBitmap.FontStyle.REGULAR);

    //        //矩形框 坐标 宽度 高度
        this.api.drawRectangle(1,40,25,9,0.2);
    //        //文字
        this.api.drawText(dlgsmc,1,41,25,0,2.2,IAtBitmap.FontStyle.REGULAR);
    //        //矩形中间画一条线
        this.api.drawLine(1,44,26,44,0.15);
    //        //文字
    //        api.setItemOrientation(270);
        this.api.drawText(dyrq,1,45,25,0,2.2,IAtBitmap.FontStyle.REGULAR);

        //二维码
    //        api.setItemOrientation(0);
        this.api.draw2DQRCode(code, 26,26,20);

        // 结束绘图任务提交打印
        return api.commitJob();
    }

    private boolean print2dBarcode_new22(String code1, String dlgsmc1, String dyrq1, String code2, String dlgsmc2, String dyrq2, Bundle param) {
        this.api.startJob(60, 50, 270);

        //第一部分
        String ywdwbm1 = code1.substring(0,4);
        String uuid1 = code1.substring(4,24);
        String uuid11 = code1.substring(24);
        this.api.drawText(ywdwbm1,1,8.2,6,0,2.2,IAtBitmap.FontStyle.REGULAR);
        this.api.drawText(uuid1,7,8.2,31.5,0,2.2,IAtBitmap.FontStyle.REGULAR);
        this.api.drawText(uuid11,7,10.6,31.5,0,2.2,IAtBitmap.FontStyle.REGULAR);
//        this.api.drawText(code1,1,8.2,37.5,0,2.2,IAtBitmap.FontStyle.REGULAR);
        //矩形框 坐标 宽度 高度
        this.api.drawRectangle(1,13.7,37.5,9,0.2);
        //文字
        this.api.drawText(dlgsmc1,1.2,14.5,37.3,0,2.2,IAtBitmap.FontStyle.REGULAR);
        //矩形中间画一条线
        this.api.drawLine(1,19.5,38,19.5,0.15);
        //文字
        this.api.drawText(dyrq1,1.2,20,37.3,0,2.2,IAtBitmap.FontStyle.REGULAR);
        //二维码
        this.api.draw2DQRCode(code1, 39,0,20);


        if(code2.isEmpty() || dlgsmc2.isEmpty() || dyrq2.isEmpty()){

        }else{
            //第二部分
            String ywdwbm2 = code2.substring(0,4);
            String uuid2 = code2.substring(4,24);
            String uuid22 = code2.substring(24);
            this.api.drawText(ywdwbm2,1,33,6,0,2.2,IAtBitmap.FontStyle.REGULAR);
            this.api.drawText(uuid2,7,33,31.5,0,2.2,IAtBitmap.FontStyle.REGULAR);
            this.api.drawText(uuid22,7,35.5,31.5,0,2.2,IAtBitmap.FontStyle.REGULAR);
//        this.api.drawText(code2,1,33,37.5,0,2.2,IAtBitmap.FontStyle.REGULAR);

            //矩形框 坐标 宽度 高度
        this.api.drawRectangle(1,38.7,37.5,9,0.2);
            //文字
        this.api.drawText(dlgsmc2,1.2,39.5,37.3,0,2.2,IAtBitmap.FontStyle.REGULAR);
            //矩形中间画一条线
        this.api.drawLine(1,44.5,38,44.5,0.15);
            //文字
        this.api.drawText(dyrq2,1.2,45,37.3,0,2.2,IAtBitmap.FontStyle.REGULAR);

        //二维码
        this.api.draw2DQRCode(code2, 39,25,20);
        }

        // 结束绘图任务提交打印
        return api.commitJob();
    }
}
