
#include <zbar.h>
using namespace zbar;

extern "C"  __declspec(dllexport) void * __cdecl CreateImageScanner()
{
	ImageScanner *scanner = new ImageScanner();
	scanner->set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	return scanner;
}

extern "C"  __declspec(dllexport) void __cdecl DeleteImageScanner(ImageScanner *scanner)
{
	delete scanner;
}

extern "C"  __declspec(dllexport) int __cdecl ScannerConfig(ImageScanner *scanner,const char * cfgstr)
{
	return scanner->set_config(cfgstr);
}

extern "C"  __declspec(dllexport) int __cdecl Scan( ImageScanner *scanner,const void *raw,int width,int height,const char * format,void  (__cdecl *callback)(const char* type_name,const char * data)  )
{
    Image image(width, height, format, raw, width * height);
    int n = scanner->scan(image);

    for(Image::SymbolIterator symbol = image.symbol_begin();
        symbol != image.symbol_end();
        ++symbol) {
			callback( symbol->get_type_name().c_str(),symbol->get_data().c_str() );
    }

    image.set_data(NULL, 0);
    return(n);
}
