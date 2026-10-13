#pragma once
// Resource fixtures are generated in the Windows temporary directory.
// No image, PE or resource binaries are stored in the repository.
extern "C" void* MS_ABI impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__H_Z(void*, void*, unsigned int, const wchar_t*, unsigned long long, int);
extern "C" void* MS_ABI impl__GetSize_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void*, void*);
extern "C" void* MS_ABI impl__GetPixelSize_CD2DBitmap__QEBA_AVCD2DSizeU__XZ(const void*, void*);
extern "C" void* MS_ABI impl__GetPixelFormat_CD2DBitmap__QEBA_AUD2D1_PIXEL_FORMAT__XZ(const void*, void*);
extern "C" void* MS_ABI impl__GetDPI_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(const void*, void*);
extern "C" long MS_ABI impl__CopyFromMemory_CD2DBitmap__QEAAJPEBXIPEBVCD2DRectU___Z(void*, const void*, unsigned int, const D2D1_RECT_U*);
extern "C" long MS_ABI impl__CopyFromBitmap_CD2DBitmap__QEAAJPEBV1_PEBVCD2DPointU__PEBVCD2DRectU___Z(void*, const void*, const D2D1_POINT_2U*, const D2D1_RECT_U*);
extern "C" long MS_ABI impl__CopyFromRenderTarget_CD2DBitmap__QEAAJPEBVCRenderTarget__PEBVCD2DPointU__PEBVCD2DRectU___Z(void*, const void*, const D2D1_POINT_2U*, const D2D1_RECT_U*);
extern "C" void MS_ABI impl__DrawBitmap_CRenderTarget__QEAAXPEAVCD2DBitmap__AEBVCD2DRectF__MW4D2D1_BITMAP_INTERPOLATION_MODE__PEBV3__Z(CRenderTarget*, void*, const CD2DRectF*, float, int, const CD2DRectF*);

static unsigned long long sizeBits(UINT width, UINT height) { return width | (static_cast<unsigned long long>(height) << 32); }
static unsigned long long pointBits(float x, float y) { D2D1_POINT_2F point{x,y}; unsigned long long bits; std::memcpy(&bits,&point,8); return bits; }
static bool pixelIs(IWICBitmap* image, int x, int y, BYTE b, BYTE g, BYTE r) {
    WICRect rect{x,y,1,1}; BYTE pixel[4]{};
    return SUCCEEDED(image->CopyPixels(&rect,4,4,pixel)) && pixel[0]==b && pixel[1]==g && pixel[2]==r && pixel[3]==255;
}
struct BitmapFixture {
    wchar_t path[MAX_PATH]{}, resourcePath[MAX_PATH]{};
    HMODULE module = nullptr;
    std::vector<BYTE> bytes;
    BitmapFixture() {
        wchar_t temp[MAX_PATH]{}; GetTempPathW(MAX_PATH,temp);
        GetTempFileNameW(temp,L"omf",0,path);
        BITMAPFILEHEADER file{}; BITMAPINFOHEADER info{};
        DWORD pixels[8] = {0xffff0000,0xff00ff00,0xff0000ff,0xffffffff,0xff0000ff,0xffff0000,0xffffffff,0xff00ff00};
        file.bfType=0x4d42; file.bfOffBits=sizeof(file)+sizeof(info); file.bfSize=file.bfOffBits+sizeof(pixels);
        info.biSize=sizeof(info); info.biWidth=4; info.biHeight=-2; info.biPlanes=1; info.biBitCount=32;
        bytes.resize(file.bfSize);
        std::memcpy(bytes.data(),&file,sizeof(file)); std::memcpy(bytes.data()+sizeof(file),&info,sizeof(info));
        std::memcpy(bytes.data()+file.bfOffBits,pixels,sizeof(pixels));
        HANDLE output=CreateFileW(path,GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
        DWORD written=0;
        check(output!=INVALID_HANDLE_VALUE && WriteFile(output,bytes.data(),static_cast<DWORD>(bytes.size()),&written,nullptr) && written==bytes.size(),
              "generate an encoded 4x2 bitmap file fixture");
        if(output!=INVALID_HANDLE_VALUE) CloseHandle(output);
        wchar_t exe[MAX_PATH]{}; GetModuleFileNameW(nullptr,exe,MAX_PATH);
        GetTempFileNameW(temp,L"omr",0,resourcePath);
        bool copied=CopyFileW(exe,resourcePath,FALSE)!=FALSE;
        HANDLE update=copied?BeginUpdateResourceW(resourcePath,FALSE):nullptr;
        bool updated=update && UpdateResourceW(update,L"image",MAKEINTRESOURCEW(101),MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL),bytes.data(),static_cast<DWORD>(bytes.size()));
        if(update) updated=EndUpdateResourceW(update,!updated)!=FALSE && updated;
        if(updated) module=LoadLibraryExW(resourcePath,nullptr,LOAD_LIBRARY_AS_DATAFILE|LOAD_LIBRARY_AS_IMAGE_RESOURCE);
        check(module!=nullptr,"generate and load an image resource module without executing it");
    }
    ~BitmapFixture() { if(module) FreeLibrary(module); DeleteFileW(resourcePath); DeleteFileW(path); }
};
static void bitmapResources(CRenderTarget& render, IWICBitmap* image) {
    BitmapFixture fixture;
    CD2DBitmap file(&render,fixture.path,D2D1_SIZE_U{8,0},FALSE);
    check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render,&file) && file.IsValid() &&
          file.m_pBitmap->GetPixelSize().width==8 && file.m_pBitmap->GetPixelSize().height==4,
          "file bitmap decoding performs native WIC conversion and aspect-preserving resizing");
    if(!file.IsValid()) return;
    D2D1_SIZE_F dips{},dpi{}; D2D1_SIZE_U pixels{}; D2D1_PIXEL_FORMAT format{};
    check(impl__GetSize_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(&file,&dips)==&dips &&
          impl__GetPixelSize_CD2DBitmap__QEBA_AVCD2DSizeU__XZ(&file,&pixels)==&pixels &&
          impl__GetPixelFormat_CD2DBitmap__QEBA_AUD2D1_PIXEL_FORMAT__XZ(&file,&format)==&format &&
          impl__GetDPI_CD2DBitmap__QEBA_AVCD2DSizeF__XZ(&file,&dpi)==&dpi && pixels.width==8 && pixels.height==4 &&
          dips.width==8 && dpi.width==96 && format.format==DXGI_FORMAT_B8G8R8A8_UNORM,
          "bitmap getters honor hidden MSVC return slots and query actual native dimensions, format and DPI");
    g_d2dTestResourceModule=fixture.module;
    alignas(CD2DBitmap) unsigned char resourceStorage[sizeof(CD2DBitmap)]; std::memset(resourceStorage,0xa5,sizeof(resourceStorage));
    auto* resource=static_cast<CD2DBitmap*>(impl___0CD2DBitmap__QEAA_PEAVCRenderTarget__IPEB_WVCD2DSizeU__H_Z(
        resourceStorage,&render,101,nullptr,sizeBits(4,2),FALSE));
    check(resource->m_uiResID==101 && resource->m_sizeDest.width==4 && resource->m_sizeDest.height==2 &&
          !resource->m_bIsAutoDestroy && resource->GetRuntimeClass()==impl__GetThisClass_CD2DBitmap__SAPEAUCRuntimeClass__XZ() &&
          impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render,resource),
          "resource constructor forwards every ABI argument and decodes the default image resource through the real module finder");
    CD2DBitmap missing(&render,999,L"image",D2D1_SIZE_U{},FALSE);
    check(missing.Create(&render)==E_FAIL && !missing.IsValid(),"missing image resource reports failure without inventing an interface");
    g_d2dTestResourceModule=nullptr;
    CD2DBitmap invalid(&render,L"Z:\\openmfc-file-that-does-not-exist.bmp",D2D1_SIZE_U{},FALSE);
    check(FAILED(invalid.Create(&render)) && !invalid.IsValid(),"decoder errors propagate and leave the bitmap invalid");
    auto* old=file.m_pBitmap; old->AddRef();
    check(impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(&file,&render)==S_OK && file.m_pBitmap!=old,
          "bitmap recreation releases the old interface and reloads its stored file source"); old->Release();
    CD2DBitmap destination(&render,FALSE);
    D2D1_BITMAP_PROPERTIES properties=D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED));
    check(SUCCEEDED(render.GetRenderTarget()->CreateBitmap(D2D1_SIZE_U{4,2},nullptr,0,&properties,&destination.m_pBitmap)),"allocate an actual destination bitmap for copy tests");
    if(destination.IsValid() && resource->IsValid()) {
        DWORD green[8]; for(auto& value:green)value=0xff00ff00;
        check(impl__CopyFromMemory_CD2DBitmap__QEAAJPEBXIPEBVCD2DRectU___Z(&destination,green,16,nullptr)==S_OK,"CopyFromMemory uses the byte pitch to upload native pixels");
        D2D1_POINT_2U point{1,0}; D2D1_RECT_U rect{2,0,3,1};
        check(impl__CopyFromBitmap_CD2DBitmap__QEAAJPEBV1_PEBVCD2DPointU__PEBVCD2DRectU___Z(&destination,resource,&point,&rect)==S_OK,
              "CopyFromBitmap forwards the source region and destination point without aliasing ownership");
        render.BeginDraw(); render.Clear(CD2DColorF{0,0,0,1}); CD2DRectF bounds(8,8,12,10);
        impl__DrawBitmap_CRenderTarget__QEAAXPEAVCD2DBitmap__AEBVCD2DRectF__MW4D2D1_BITMAP_INTERPOLATION_MODE__PEBV3__Z(&render,&destination,&bounds,1,D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,nullptr);
        check(SUCCEEDED(render.EndDraw()) && pixelIs(image,9,8,255,0,0),"production DrawBitmap displays the copied blue source pixel at the requested destination offset");
        point={2,1}; rect={9,8,10,9};
        HRESULT copied=impl__CopyFromRenderTarget_CD2DBitmap__QEAAJPEBVCRenderTarget__PEBVCD2DPointU__PEBVCD2DRectU___Z(&destination,&render,&point,&rect);
        check(copied==S_OK,"CopyFromRenderTarget copies the actual rendered source region");
        DWORD red=0xffff0000; rect={3,0,4,1};
        check(impl__CopyFromMemory_CD2DBitmap__QEAAJPEBXIPEBVCD2DRectU___Z(&destination,&red,4,&rect)==S_OK,"partial memory upload preserves pixels outside its destination rectangle");
        render.BeginDraw(); bounds=CD2DRectF(16,16,20,18);
        impl__DrawBitmap_CRenderTarget__QEAAXPEAVCD2DBitmap__AEBVCD2DRectF__MW4D2D1_BITMAP_INTERPOLATION_MODE__PEBV3__Z(&render,&destination,&bounds,1,D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,nullptr);
        check(SUCCEEDED(render.EndDraw()) && pixelIs(image,19,16,0,0,255) && pixelIs(image,18,17,255,0,0),
              "render-target copy and partial memory upload produce the expected native pixels");
    }
    resource->~CD2DBitmap();
    D2D1_BITMAP_BRUSH_PROPERTIES brushProps=D2D1::BitmapBrushProperties(D2D1_EXTEND_MODE_WRAP,D2D1_EXTEND_MODE_MIRROR,D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
    CD2DBitmapBrush brush(&render,fixture.path,D2D1_SIZE_U{4,2},&brushProps,nullptr,FALSE);
    check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render,&brush) && brush.m_pBitmapBrush &&
          brush.m_pBrush==brush.m_pBitmapBrush && brush.m_pBitmapBrush->GetExtendModeX()==D2D1_EXTEND_MODE_WRAP,
          "bitmap brush verification lazily loads its registered source and creates both native brush aliases");
    if(brush.IsValid()) {
        render.BeginDraw(); CD2DRectF rect(24,0,32,4);
        impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render,&rect,&brush);
        check(SUCCEEDED(render.EndDraw()) && pixelIs(image,24,0,0,0,255) && pixelIs(image,26,0,255,0,0),"native bitmap brush tiles the decoded image into rendered pixels");
        brush.Destroy();
        check(!brush.m_pBrush && !brush.m_pBitmapBrush && !brush.m_pBitmapBrushProperties && brush.m_pBitmap->IsValid(),
              "bitmap brush destruction clears both native aliases and properties while retaining its separately owned source");
        check(brush.Create(&render)==S_OK,"bitmap brush can be created again from its retained source");
    }
    BITMAPINFO info{}; info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER); info.bmiHeader.biWidth=4; info.bmiHeader.biHeight=-2;
    info.bmiHeader.biPlanes=1; info.bmiHeader.biBitCount=32; void* bits=nullptr;
    HBITMAP handle=CreateDIBSection(nullptr,&info,DIB_RGB_COLORS,&bits,nullptr,0);
    check(handle && bits,"create a source HBITMAP for eager and deferred bitmap constructors");
    if(handle && bits) {
        std::memcpy(bits,fixture.bytes.data()+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER),32);
        CRenderTarget deferredParent;
        CD2DBitmap deferred(&deferredParent,handle,D2D1_SIZE_U{},FALSE);
        HBITMAP ownedHandle=deferred.m_hBmpSrc;
        check(ownedHandle && ownedHandle!=handle && deferred.m_bAutoDestroyHBMP,"deferred HBITMAP construction owns an independent copy");
        render.GetRenderTarget()->AddRef(); deferredParent.Attach(render.GetRenderTarget());
        DeleteObject(handle);
        HRESULT uploaded=deferred.Create(&deferredParent);
        BITMAP object{};
        int remaining=GetObjectW(ownedHandle,sizeof(object),&object);
        if(uploaded!=S_OK || !deferred.IsValid() || deferred.m_hBmpSrc || remaining)
            std::printf("HBITMAP upload hr=%08lx valid=%d source=%p old handle query=%d\n",static_cast<unsigned long>(uploaded),deferred.IsValid(),deferred.m_hBmpSrc,remaining);
        check(uploaded==S_OK && deferred.IsValid() && !deferred.m_hBmpSrc && remaining==0,
              "deferred bitmap creation uploads its copied HBITMAP and releases the owned GDI handle");
    }
    // A borrowed bitmap brush must not retain a dangling source-wrapper pointer.
    auto* owner=new CRenderTarget;
    CD2DBitmapBrush survivor(owner,nullptr,nullptr,FALSE);
    delete owner;
    check(!survivor.m_pParentTarget && !survivor.m_pBitmap,"parent destruction clears borrowed brush pointers to deleted source wrappers");
}

extern "C" void* MS_ABI impl__GetStartPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(const void*,void*);
extern "C" void MS_ABI impl__SetStartPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(void*,unsigned long long);
extern "C" void MS_ABI impl__SetEndPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(void*,unsigned long long);
extern "C" void* MS_ABI impl__GetCenter_CD2DRadialGradientBrush__QEBA_AVCD2DPointF__XZ(const void*,void*);
extern "C" void MS_ABI impl__SetCenter_CD2DRadialGradientBrush__QEAAXVCD2DPointF___Z(void*,unsigned long long);
extern "C" void MS_ABI impl__SetRadiusX_CD2DRadialGradientBrush__QEAAXM_Z(void*,float);
struct GradientFailureProbe {
    void** table;
    int creates=0, releases=0, brushCreates=0;
};
static HRESULT STDMETHODCALLTYPE failGradientStops(GradientFailureProbe* self,const D2D1_GRADIENT_STOP*,UINT,D2D1_GAMMA,D2D1_EXTEND_MODE,ID2D1GradientStopCollection** result) {
    ++self->creates;*result=nullptr;
    return E_ACCESSDENIED;
}
static HRESULT STDMETHODCALLTYPE failLinearBrush(GradientFailureProbe* self,const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES*,const D2D1_BRUSH_PROPERTIES*,ID2D1GradientStopCollection*,ID2D1LinearGradientBrush** result) {
    ++self->brushCreates;*result=nullptr;return E_OUTOFMEMORY;
}
struct FailedCollection { void** table;int* releases; };
static ULONG STDMETHODCALLTYPE releaseCollection(FailedCollection* self) { ++*self->releases;return 0; }
static FailedCollection* partialCollection;
static HRESULT STDMETHODCALLTYPE createPartialCollection(GradientFailureProbe* self,const D2D1_GRADIENT_STOP*,UINT,D2D1_GAMMA,D2D1_EXTEND_MODE,ID2D1GradientStopCollection** result) {
    ++self->creates;*result=reinterpret_cast<ID2D1GradientStopCollection*>(partialCollection);return S_OK;
}
static void controlledGradientFailures(CRenderTarget&,const D2D1_GRADIENT_STOP* stops,D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES props) {
    void* slots[12]{};slots[9]=reinterpret_cast<void*>(failGradientStops);slots[10]=reinterpret_cast<void*>(failLinearBrush);
    GradientFailureProbe probe{slots};
    CRenderTarget parent;parent.Attach(&probe);
    CD2DLinearGradientBrush invalid(&parent,stops,2,props,D2D1_GAMMA_1_0,D2D1_EXTEND_MODE_CLAMP,nullptr,FALSE);
    check(invalid.Create(&parent)==E_ACCESSDENIED && probe.creates==1 && probe.brushCreates==0 && !invalid.m_pGradientStops && !invalid.m_pBrush,
          "gradient collection HRESULT failure propagates without publishing an interface");
    void* collectionSlots[3]={nullptr,nullptr,reinterpret_cast<void*>(releaseCollection)};
    FailedCollection collection{collectionSlots,&probe.releases};partialCollection=&collection;
    slots[9]=reinterpret_cast<void*>(createPartialCollection);
    check(invalid.Create(&parent)==E_OUTOFMEMORY && probe.creates==2 && probe.brushCreates==1 && probe.releases==1 && !invalid.m_pGradientStops && !invalid.m_pBrush,
          "failed brush creation releases its intermediate stop collection exactly once");
    parent.Detach();partialCollection=nullptr;
}
extern "C" void* MS_ABI impl___0CD2DLinearGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(void*,void*,const D2D1_GRADIENT_STOP*,unsigned int,const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES*,int,int,void*,int);
extern "C" void* MS_ABI impl___0CD2DRadialGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(void*,void*,const D2D1_GRADIENT_STOP*,unsigned int,const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES*,int,int,void*,int);
static void gradientConstructorABI(CRenderTarget& render) {
    D2D1_GRADIENT_STOP stops[2]={{0,{1,0,0,1}},{1,{0,0,1,1}}};
    D2D1_BRUSH_PROPERTIES props=D2D1::BrushProperties(0.7f);
    {
        alignas(CD2DLinearGradientBrush) unsigned char storage[sizeof(CD2DLinearGradientBrush)];std::memset(storage,0xa5,sizeof(storage));
        D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES geometry={{2.5f,3.75f},{61,4.25f}};
        auto* object=static_cast<CD2DLinearGradientBrush*>(impl___0CD2DLinearGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(storage,&render,stops,2,&geometry,
            D2D1_GAMMA_1_0,D2D1_EXTEND_MODE_MIRROR,&props,FALSE));
        check(object==reinterpret_cast<CD2DLinearGradientBrush*>(storage) && object->GetRuntimeClass()==impl__GetThisClass_CD2DLinearGradientBrush__SAPEAUCRuntimeClass__XZ() &&
              object->m_pParentTarget==&render && !object->m_bIsAutoDestroy && object->m_arGradientStops.size==2 &&
              object->m_colorInterpolationGamma==D2D1_GAMMA_1_0 && object->m_extendMode==D2D1_EXTEND_MODE_MIRROR &&
              object->m_pBrushProperties && object->m_pBrushProperties!=&props && object->m_pBrushProperties->opacity==0.7f &&
              !object->m_pLinearGradientBrush && std::memcmp(&object->m_LinearGradientBrushProperties,&geometry,sizeof(geometry))==0,
              "exported linear constructor installs its vtable and forwards stops, by-value geometry, modes, properties and ownership");
        object->~CD2DLinearGradientBrush();
    }
    {
        alignas(CD2DRadialGradientBrush) unsigned char storage[sizeof(CD2DRadialGradientBrush)];std::memset(storage,0xa5,sizeof(storage));
        D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES geometry={{31.5f,30.5f},{1.25f,2.5f},14,15};
        auto* object=static_cast<CD2DRadialGradientBrush*>(impl___0CD2DRadialGradientBrush__QEAA_PEAVCRenderTarget__PEBUD2D1_GRADIENT_STOP__IUD2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES__W4D2D1_GAMMA__W4D2D1_EXTEND_MODE__PEAVCD2DBrushProperties__H_Z(storage,&render,stops,2,&geometry,
            D2D1_GAMMA_1_0,D2D1_EXTEND_MODE_MIRROR,&props,FALSE));
        check(object==reinterpret_cast<CD2DRadialGradientBrush*>(storage) && object->GetRuntimeClass()==impl__GetThisClass_CD2DRadialGradientBrush__SAPEAUCRuntimeClass__XZ() &&
              object->m_pParentTarget==&render && !object->m_bIsAutoDestroy && object->m_arGradientStops.size==2 &&
              object->m_colorInterpolationGamma==D2D1_GAMMA_1_0 && object->m_extendMode==D2D1_EXTEND_MODE_MIRROR &&
              object->m_pBrushProperties && object->m_pBrushProperties!=&props && object->m_pBrushProperties->opacity==0.7f &&
              !object->m_pRadialGradientBrush && std::memcmp(&object->m_RadialGradientBrushProperties,&geometry,sizeof(geometry))==0,
              "exported radial constructor installs its vtable and forwards stops, by-value geometry, modes, properties and ownership");
        object->~CD2DRadialGradientBrush();
    }
}
static void gradientResources(CRenderTarget& render,IWICBitmap* image) {
    gradientConstructorABI(render);
    D2D1_GRADIENT_STOP stops[2]={{0,{1,0,0,1}},{1,{0,0,1,1}}};
    D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linearProps={{0,0},{64,0}};
    CD2DLinearGradientBrush linear(&render,stops,2,linearProps,D2D1_GAMMA_1_0,D2D1_EXTEND_MODE_CLAMP,nullptr,FALSE);
    stops[0].color={0,1,0,1};
    D2D1_POINT_2F point{};
    check(linear.m_arGradientStops.size==2 && linear.m_arGradientStops.data!=stops && linear.m_arGradientStops.data[0].color.r==1 &&
          impl__GetStartPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(&linear,&point)==&point && point.x==0,
          "gradient constructor owns an inline ABI-compatible stop array and point getters honor hidden return slots");
    impl__SetStartPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(&linear,pointBits(2.5f,3.75f));
    check(impl__GetStartPoint_CD2DLinearGradientBrush__QEBA_AVCD2DPointF__XZ(&linear,&point)==&point && point.x==2.5f && point.y==3.75f,
          "linear point setter forwards both float components in the MSVC integer argument register");
    impl__SetStartPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(&linear,pointBits(0,0));
    impl__SetEndPoint_CD2DLinearGradientBrush__QEAAXVCD2DPointF___Z(&linear,pointBits(64,0));
    check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render,&linear) && linear.m_pLinearGradientBrush && linear.m_pGradientStops &&
          linear.m_pBrush==linear.m_pLinearGradientBrush && linear.m_pGradientStops->GetGradientStopCount()==2 &&
          linear.m_pGradientStops->GetColorInterpolationGamma()==D2D1_GAMMA_1_0,
          "linear gradient creation uses the copied stops, gamma and real native interfaces");
    D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES radialProps={{32,32},{0,0},16,16};
    stops[0].color={1,0,0,1};
    CD2DRadialGradientBrush radial(&render,stops,2,radialProps,D2D1_GAMMA_1_0,D2D1_EXTEND_MODE_CLAMP,nullptr,FALSE);
    impl__SetCenter_CD2DRadialGradientBrush__QEAAXVCD2DPointF___Z(&radial,pointBits(32,32));
    check(impl__VerifyResource_CRenderTarget__IEAAHPEAVCD2DResource___Z(&render,&radial) && radial.IsValid(),"radial gradient creates a native brush and registered stop collection");
    if(linear.IsValid() && radial.IsValid()) {
        impl__SetRadiusX_CD2DRadialGradientBrush__QEAAXM_Z(&radial,16);
        check(impl__GetCenter_CD2DRadialGradientBrush__QEBA_AVCD2DPointF__XZ(&radial,&point)==&point && point.x==32 && radial.m_pRadialGradientBrush->GetRadiusX()==16,
              "radial geometry setters/getters dispatch to the actual COM brush");
        BYTE actual[64*64*4]{},reference[sizeof(actual)]{};
        auto draw=[&](bool production,BYTE* result){
            render.BeginDraw(); render.Clear(CD2DColorF{0,0,0,1});
            CD2DRectF left(0,0,64,16),middle(16,16,48,48);
            if(production) {
                impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render,&left,&linear);
                impl__FillRectangle_CRenderTarget__QEAAXAEBVCD2DRectF__PEAVCD2DBrush___Z(&render,&middle,&radial);
            } else {
                D2D1_RECT_F a{0,0,64,16},b{16,16,48,48};
                render.GetRenderTarget()->FillRectangle(&a,linear.m_pLinearGradientBrush);
                render.GetRenderTarget()->FillRectangle(&b,radial.m_pRadialGradientBrush);
            }
            check(SUCCEEDED(render.EndDraw()) && SUCCEEDED(image->CopyPixels(nullptr,64*4,sizeof(actual),result)),"finish and read native gradient drawing");
        };
        draw(true,actual);draw(false,reference);
        check(std::memcmp(actual,reference,sizeof(actual))==0 && actual[2]>actual[0] && actual[63*4]>actual[63*4+2],
              "production linear and radial gradients match direct COM pixels and interpolate between the supplied colors");
        auto* old=linear.m_pLinearGradientBrush;old->AddRef();
        check(impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(&linear,&render)==S_OK && linear.m_pLinearGradientBrush!=old &&
              linear.m_arGradientStops.size==2 && linear.m_pGradientStops,
              "gradient recreation releases native interfaces while preserving the source stops and geometry");old->Release();
    }
    CD2DLinearGradientBrush empty(&render,nullptr,0,linearProps,D2D1_GAMMA_2_2,D2D1_EXTEND_MODE_CLAMP,nullptr,FALSE);
    check(empty.Create(&render)==E_FAIL && !empty.IsValid(),"gradient creation rejects an empty stop list without reporting false success");
    controlledGradientFailures(render,stops,linearProps);
}
static void bitmapAndGradients() {
    auto& state=OpenMfcD2DState(); IWICBitmap* image=nullptr; ID2D1RenderTarget* target=nullptr;
    HRESULT hr=state.GetWICFactory()->CreateBitmap(64,64,GUID_WICPixelFormat32bppPBGRA,WICBitmapCacheOnLoad,&image);
    if(SUCCEEDED(hr))hr=state.GetDirect2dFactory()->CreateWicBitmapRenderTarget(image,D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE),&target);
    check(SUCCEEDED(hr),"create software surface for bitmap and gradient integration tests");
    if(SUCCEEDED(hr)) { CRenderTarget render;render.Attach(target);bitmapResources(render,image);gradientResources(render,image); }
    if(image)image->Release();
}
