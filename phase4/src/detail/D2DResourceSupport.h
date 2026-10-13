#pragma once
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "detail/D2DFactorySupport.h"
#include <cstddef>

#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif

extern "C" long MS_ABI impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(void*, void*);
#define OPENMFC_D2D_CLASS_GETTER(Class) \
    extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_##Class##__SAPEAUCRuntimeClass__XZ();
OPENMFC_D2D_CLASS_GETTER(CD2DResource)
OPENMFC_D2D_CLASS_GETTER(CD2DBrush)
OPENMFC_D2D_CLASS_GETTER(CD2DSolidColorBrush)
OPENMFC_D2D_CLASS_GETTER(CD2DGeometry)
OPENMFC_D2D_CLASS_GETTER(CD2DPathGeometry)
OPENMFC_D2D_CLASS_GETTER(CD2DLayer)
OPENMFC_D2D_CLASS_GETTER(CD2DMesh)
#undef OPENMFC_D2D_CLASS_GETTER
void OpenMfcRegisterD2DResource(CRenderTarget*, void*);
void OpenMfcUnregisterD2DResource(CRenderTarget*, void*);

// Private ABI models matching afxrendertarget.h. CObject's five virtual slots
// are followed by ReCreate, Create, Destroy and IsValid (slots 5..8).
class CD2DResource : public CObject {
    friend class CRenderTarget;
    friend long MS_ABI impl__ReCreate_CD2DResource__MEAAJPEAVCRenderTarget___Z(void*, void*);
protected:
    CD2DResource(CRenderTarget*, BOOL);
    virtual HRESULT ReCreate(CRenderTarget*);
public:
    ~CD2DResource() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ(); }
    virtual HRESULT Create(CRenderTarget*) = 0;
    virtual void Destroy() = 0;
    virtual BOOL IsValid() const = 0;
    static void* ConstructorVtable();
    BOOL m_bIsAutoDestroy;
    CRenderTarget* m_pParentTarget;
};
static_assert(sizeof(CD2DResource) == 24, "retail resource layout");
static_assert(offsetof(CD2DResource, m_bIsAutoDestroy) == 8, "auto-destroy offset");
static_assert(offsetof(CD2DResource, m_pParentTarget) == 16, "parent offset");

class CD2DBrush : public CD2DResource {
protected:
    CD2DBrush(CRenderTarget*, const D2D1_BRUSH_PROPERTIES*, BOOL);
public:
    ~CD2DBrush() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ(); }
    void Destroy() override;
    BOOL IsValid() const override { return m_pBrush != nullptr; }
    static void* ConstructorVtable();
    ID2D1Brush* m_pBrush;
    D2D1_BRUSH_PROPERTIES* m_pBrushProperties;
};
static_assert(sizeof(CD2DBrush) == 40, "retail brush layout");
static_assert(offsetof(CD2DBrush, m_pBrush) == 24, "brush interface offset");
static_assert(offsetof(CD2DBrush, m_pBrushProperties) == 32, "brush properties offset");

class CD2DSolidColorBrush : public CD2DBrush {
public:
    CD2DSolidColorBrush(CRenderTarget*, D2D1_COLOR_F, const D2D1_BRUSH_PROPERTIES*, BOOL);
    ~CD2DSolidColorBrush() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DSolidColorBrush__SAPEAUCRuntimeClass__XZ(); }
    HRESULT Create(CRenderTarget*) override;
    void Destroy() override;
    ID2D1SolidColorBrush* m_pSolidColorBrush;
    D2D1_COLOR_F m_colorSolid;
};
static_assert(sizeof(CD2DSolidColorBrush) == 64, "retail solid brush layout");
static_assert(offsetof(CD2DSolidColorBrush, m_pSolidColorBrush) == 40, "solid brush interface offset");
static_assert(offsetof(CD2DSolidColorBrush, m_colorSolid) == 48, "solid color offset");

class CD2DGeometry : public CD2DResource {
public:
    CD2DGeometry(CRenderTarget*, BOOL);
    ~CD2DGeometry() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DGeometry__SAPEAUCRuntimeClass__XZ(); }
    void Destroy() override;
    BOOL IsValid() const override { return m_pGeometry != nullptr; }
    static void* ConstructorVtable();
    ID2D1Geometry* m_pGeometry;
};
static_assert(sizeof(CD2DGeometry) == 32, "retail geometry layout");

class CD2DPathGeometry : public CD2DGeometry {
public:
    CD2DPathGeometry(CRenderTarget*, BOOL);
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DPathGeometry__SAPEAUCRuntimeClass__XZ(); }
    HRESULT Create(CRenderTarget*) override;
    void Destroy() override;
    ID2D1PathGeometry* m_pPathGeometry;
};
static_assert(sizeof(CD2DPathGeometry) == 40, "retail path geometry layout");

class CD2DLayer : public CD2DResource {
public:
    CD2DLayer(CRenderTarget*, BOOL);
    ~CD2DLayer() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DLayer__SAPEAUCRuntimeClass__XZ(); }
    HRESULT Create(CRenderTarget*) override;
    void Destroy() override;
    BOOL IsValid() const override { return m_pLayer != nullptr; }
    ID2D1Layer* m_pLayer;
};
static_assert(sizeof(CD2DLayer) == 32, "retail layer layout");

class CD2DMesh : public CD2DResource {
public:
    CD2DMesh(CRenderTarget*, BOOL);
    ~CD2DMesh() override;
    CRuntimeClass* GetRuntimeClass() const override { return impl__GetThisClass_CD2DMesh__SAPEAUCRuntimeClass__XZ(); }
    HRESULT Create(CRenderTarget*) override;
    void Destroy() override;
    BOOL IsValid() const override { return m_pMesh != nullptr; }
    ID2D1Mesh* m_pMesh;
};
static_assert(sizeof(CD2DMesh) == 32, "retail mesh layout");
