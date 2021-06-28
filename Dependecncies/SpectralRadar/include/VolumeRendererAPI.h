#ifndef VOLUMERENDERER_API_H__
#define VOLUMERENDERER_API_H__

#include "RenderingLibrary.h"



#ifdef __cplusplus
extern "C" {
#endif
    #include <SpectralRadar.h>
	#include "PrivateAPI.h"

	typedef enum {
		RendererNoError = 0x00000000,
		RendererError = 0xE0000000
	} RendererErrorCode;

	RENDERINGLIBRARY_API RendererErrorCode isRendererError(); // does not clear error flag...
	RENDERINGLIBRARY_API RendererErrorCode getRendererError(char* Message, int StringSize);

	typedef void(*refIntensityCallback)(double);

    struct C_LiveVolumeRendering;
    typedef C_LiveVolumeRendering* LiveVolumeRenderingHandle;

    struct C_LiveDataSource;
    typedef C_LiveDataSource* LiveDataSourceHandle;

	struct C_VolumeRenderer;
	typedef C_VolumeRenderer* VolumeRendererHandle; 

	struct C_OfflineVolumeRendering;
	typedef C_OfflineVolumeRendering* OfflineVolumeRenderingHandle;

	typedef enum C_RenderingMode {
		RenderingMode_Volume,
		RenderingMode_Sectional
	} RenderingMode;

	typedef enum C_VolumeMode {
		VolumeMode_Default,
		VolumeMode_MIP,
		VolumeMode_Mean
	} VolumeMode;

	RENDERINGLIBRARY_API VolumeRendererHandle createVolumeRenderer(void);
	RENDERINGLIBRARY_API void deleteVolumeRenderer(VolumeRendererHandle VR);

	RENDERINGLIBRARY_API void initializeVolumeRenderer(VolumeRendererHandle Renderer);
	RENDERINGLIBRARY_API void uninitializeVolumeRenderer(VolumeRendererHandle Renderer);

    RENDERINGLIBRARY_API void setAngles(VolumeRendererHandle Renderer, double AngleX, double AngleY);
	RENDERINGLIBRARY_API void setTranslation(VolumeRendererHandle Renderer, double TransX, double TransY, double TransZ);
    RENDERINGLIBRARY_API void setZoom(VolumeRendererHandle Renderer, double Zoom);
	

    RENDERINGLIBRARY_API void setViewportSize(int SizeX, int SizeY);
    RENDERINGLIBRARY_API void setRenderingSlices(VolumeRendererHandle Renderer, int Slices);
	RENDERINGLIBRARY_API void setLateralVoxelsPerPixel(VolumeRendererHandle Renderer, double VPP);
	RENDERINGLIBRARY_API void setRendererColoringBoundaries(VolumeRendererHandle Renderer, double Lower, double Upper);
	RENDERINGLIBRARY_API void setRendererColormap(VolumeRendererHandle Renderer, unsigned int* Colormap, int Size);
	RENDERINGLIBRARY_API void setSurfaceColormap(VolumeRendererHandle Renderer, unsigned int* Colormap, int Size);
	RENDERINGLIBRARY_API void setSectionalView(VolumeRendererHandle Renderer, BOOL ShowZ, float PosZ, BOOL ShowX, float PosX, BOOL ShowY, float PosY);
	RENDERINGLIBRARY_API void setClipPlane(VolumeRendererHandle Renderer, BOOL Show, BOOL Opaque, float Depth);
	RENDERINGLIBRARY_API void fixClipPlane(VolumeRendererHandle Renderer, BOOL Fix);
	RENDERINGLIBRARY_API void setBorder(VolumeRendererHandle Renderer, BOOL ShowScale, BOOL ShowBox, BOOL ShowText);
	RENDERINGLIBRARY_API void setRenderingMode(VolumeRendererHandle Renderer, RenderingMode Mode);
	RENDERINGLIBRARY_API void setVolumeMode(VolumeRendererHandle Renderer, VolumeMode Mode);
	RENDERINGLIBRARY_API void setRenderElements(VolumeRendererHandle Renderer, BOOL RenderVolume, BOOL RenderSurface);
	RENDERINGLIBRARY_API void setSurface(VolumeRendererHandle Renderer, int SizeX, int SizeY, double RangeZ, float* SurfaceData);
	RENDERINGLIBRARY_API void setSurfaceDepths(VolumeRendererHandle Renderer, double LowerDepth, double UpperDepth);
	RENDERINGLIBRARY_API void setRange(VolumeRendererHandle Renderer, double RangeZ, double RangeX, double RangeY);
	RENDERINGLIBRARY_API void setRenderBackground(VolumeRendererHandle Renderer, unsigned int top, unsigned int bottom);
	RENDERINGLIBRARY_API void setRenderForeground(VolumeRendererHandle Renderer, unsigned int color);

	RENDERINGLIBRARY_API void setRemoveSegment(VolumeRendererHandle Renderer, BOOL RemoveSegment);
	RENDERINGLIBRARY_API void clearVolumeRenderer(VolumeRendererHandle Renderer);
	

	// Live Rendering

	RENDERINGLIBRARY_API LiveVolumeRenderingHandle createLiveRendering(void);
    RENDERINGLIBRARY_API void clearLiveRendering(LiveVolumeRenderingHandle Render);  
	RENDERINGLIBRARY_API void setLiveVolumeRenderingRenderer(LiveVolumeRenderingHandle Rendering, VolumeRendererHandle Renderer);
	RENDERINGLIBRARY_API void renderLiveVolume(LiveVolumeRenderingHandle Renderer);
	RENDERINGLIBRARY_API void getLiveVolumeRenderingSize(LiveVolumeRenderingHandle DataSource, int* SizeZ, int* SizeX, int*SizeY);

	RENDERINGLIBRARY_API void setSizeZ(LiveDataSourceHandle DataSource, int SizeZ);
	RENDERINGLIBRARY_API double getRenderedVolumesPerSec(LiveVolumeRenderingHandle Renderer);
	RENDERINGLIBRARY_API BOOL isLiveVolumeRunning(LiveVolumeRenderingHandle Rendering);

	// Live Rendering data source
	RENDERINGLIBRARY_API LiveDataSourceHandle createOnlineDataSource(OCTDeviceHandle Dev, ProcessingHandle Proc);

	/// \fn RENDERINGLIBRARY_API void selectOnlineDataSource_Camera(LiveDataSourceHandle DataSource, int Camera)
	/// \brief Creates a data source (contains the data-acquisition and processing routines) for 3D preview-representations.
	RENDERINGLIBRARY_API void selectOnlineDataSource_Camera(LiveDataSourceHandle DataSource, int Camera);

	/// \fn LiveDataSourceHandle createOnlineDataSource_PS(OCTDeviceHandle Dev, ProcessingHandle Proc0, ProcessingHandle Proc1, PolarizationProcessingHandle PS_Proc)
	/// \brief Creates a data source (contains the data-acquisition and processing routines) for 3D preview-representations.
	RENDERINGLIBRARY_API LiveDataSourceHandle createOnlineDataSource_PS(OCTDeviceHandle Dev, ProcessingHandle Proc0, ProcessingHandle Proc1, PolarizationProcessingHandle PS_Proc);
	RENDERINGLIBRARY_API void clearDataSource(LiveDataSourceHandle DataSource);

	/// \fn void selectOnlineDataSource(LiveDataSourceHandle DataSource, PolarizationMode_DisplaySource PS_Quantity);
	/// \brief Selects the parameter (Camera-0, camera-1, retardation, Stokes parm,...) that will be computed and displayed.
	RENDERINGLIBRARY_API void selectOnlineDataSource_PS(LiveDataSourceHandle DataSource, PolarizationMode_DisplaySource PS_Quantity);

	/// \fn	RENDERINGLIBRARY_API void setOnlineAveraging_PS(LiveDataSourceHandle DataSource, const int avg_x, const int avg_y, const int avg_z)
	/// \brief Sets the averaging lengths over the axes to be used on the PS post-processing.
	RENDERINGLIBRARY_API void setOnlineAveraging_PS(LiveDataSourceHandle DataSource, const int avg_z, const int avg_x, const int avg_y, const int ascan_avg);

	/// \fn	RENDERINGLIBRARY_API void setOnlineDOPU_PS(LiveDataSourceHandle DataSource, const int dopu_avg_z, const int dopu_avg_x, const int dopu_avg_y, const PolarizationDOPUFilterType dopu_algorithm)
	/// \brief Sets the averaging lengths over the axes to be used on the PS post-processing.
	RENDERINGLIBRARY_API void setOnlineDOPU_PS(LiveDataSourceHandle DataSource, const int dopu_avg_z, const int dopu_avg_x, const int dopu_avg_y, const PolarizationDOPUFilterType dopu_algorithm);

	/// \fn void setOnlineDataThreshold_PS(LiveDataSourceHandle DataSource, double Threshold);
	/// \brief Selects the threshold (on the totalintensity) that will be used to nullify other parameters.
	RENDERINGLIBRARY_API void setOnlineDataThreshold_PS(LiveDataSourceHandle DataSource, double Threshold, BOOL OnOff);

	RENDERINGLIBRARY_API void setRefIntensityCallback(LiveDataSourceHandle DataSource, refIntensityCallback Callback);

    RENDERINGLIBRARY_API void startDataSource(LiveVolumeRenderingHandle Renderer, LiveDataSourceHandle DataSource, ScanPatternHandle Pattern);
    RENDERINGLIBRARY_API void stopDataSource(LiveDataSourceHandle DataSource);

	// Offline Rendering
	RENDERINGLIBRARY_API OfflineVolumeRenderingHandle createOfflineRendering(void);
	RENDERINGLIBRARY_API void clearOfflineRendering(OfflineVolumeRenderingHandle Rendering);
	RENDERINGLIBRARY_API void setOfflineRenderingRenderer(OfflineVolumeRenderingHandle Rendering, VolumeRendererHandle Renderer);
	RENDERINGLIBRARY_API void renderOfflineVolume(OfflineVolumeRenderingHandle Rendering);
	RENDERINGLIBRARY_API void allocateOfflineVolume(OfflineVolumeRenderingHandle Rendering, int SizeZ, int SizeX, int SizeY);
	RENDERINGLIBRARY_API void setOfflineAspectRatio(OfflineVolumeRenderingHandle Rendering, float RangeZ, float RangeX, float RangeY);
	RENDERINGLIBRARY_API void uploadOfflineVolume(OfflineVolumeRenderingHandle Rendering, float* Ptr);
	RENDERINGLIBRARY_API void uploadOfflineSubVolume(OfflineVolumeRenderingHandle Rendering, int SliceStartY, int SliceEndY, float* Ptr);
	RENDERINGLIBRARY_API void uploadOfflineVolumeSlice(OfflineVolumeRenderingHandle Rendering, int SliceY, float* Ptr);

	
	

#ifdef __cplusplus
}
#endif

#endif