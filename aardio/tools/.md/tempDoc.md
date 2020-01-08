com.interface.IPicture帮助文档
===========================================
<a id="com.interface.IPicture"></a>
com.interface.IPicture 成员列表
-------------------------------------------------------------------------------------------------

<h6 id="com.interface.IPicture.Query">com.interface.IPicture.Query() </h6>
 [返回对象:RawIPictureObject](#RawIPictureObject)

<h6 id="com.interface.IPicture.Query">com.interface.IPicture.Query(IPicture对象) </h6>
 参数为com.picture 对象  
返回原生接口

<a id="RawIPictureObject"></a>
RawIPictureObject 成员列表
-------------------------------------------------------------------------------------------------

<h6 id="RawIPictureObject.attributes">RawIPictureObject.attributes </h6>
 选项数值

<h6 id="RawIPictureObject.curDC">RawIPictureObject.curDC </h6>
 当前绘图设备

<h6 id="RawIPictureObject.hPal">RawIPictureObject.hPal </h6>
 调色板

<h6 id="RawIPictureObject.handle">RawIPictureObject.handle </h6>
 句柄

<h6 id="RawIPictureObject.height">RawIPictureObject.height </h6>
 高  
单位是0.01毫米而非像素

<h6 id="RawIPictureObject.keepOriginalFormat">RawIPictureObject.keepOriginalFormat </h6>
 保持格式

<h6 id="RawIPictureObject.pictureChanged">RawIPictureObject.pictureChanged() </h6>
 图像是否已被修改

<h6 id="RawIPictureObject.render">RawIPictureObject.render(hDC,x,y, cx,cy, xSrc, ySrc, cxSrc,cySrc,pRcWBounds) </h6>
 绘图  
hdc 绘图设备句柄  
x,y 设备显示坐标  
cx,cy 显示长度和宽度  
xSrc,ySrc 源图像偏移坐标  
cxSrc,cySrc 源图像输出宽度和高度,注意位图在内存是倒过来的,cySrc要取负数

<h6 id="RawIPictureObject.saveAsFile">RawIPictureObject.saveAsFile(steam,saveMemCopy) </h6>
 保存图像,成功返回保存大小  
saveMemCopy指定是否保存内存副本,默认为true

<h6 id="RawIPictureObject.saveToBuffer">RawIPictureObject.saveToBuffer(saveMemCopy) </h6>
 保存图像到缓冲区  
saveMemCopy指定是否保存内存副本,默认为true

<h6 id="RawIPictureObject.selectPicture">RawIPictureObject.selectPicture(hDCIn) </h6>
 选择绘图设备  
成功返回原设备,以及位图句柄

<h6 id="RawIPictureObject.type">RawIPictureObject.type </h6>
 图像类型  
图标还是位图

<h6 id="RawIPictureObject.width">RawIPictureObject.width </h6>
 宽  
单位是0.01毫米而非像素
