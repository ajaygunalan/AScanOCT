// SpectralRadarDemo.cpp : This file contains demonstrations programs how to use the SDK (software development kit).

#include <iostream>
#include <vector>
#include <conio.h>
#include <SpectralRadar.h>

using namespace std;

void SimpleMeasurement()
{
	// initialization of device, probe and processing handles
	OCTDeviceHandle Dev = initDevice();
	ProbeHandle Probe = initProbe(Dev, "Probe_Standard_OCTG_LSM03.ini"); // pass the correct string for configuration file of the appropiate objective which are named normally ProbeLSMxx
	ProcessingHandle Proc = createProcessingForDevice(Dev);

	// the #RawDataHandle will be used to get the raw data handle and will contain the data from the detector (e.g. line scan camera is SD-OCT systems) without any modification
	RawDataHandle Raw = createRawData();
	// the #DataHandle will be used for the processed data and will contain the OCT image
	DataHandle BScan = createData();

	// define simple B-scan pattern with 2mm range and 1024 A-scans
	ScanPatternHandle Pattern = createBScanPattern(Probe, 2.0, 1024);

	// start the measurement to acquire the specified scan pattern once
	startMeasurement(Dev, Pattern, Acquisition_AsyncFinite);

	// grabs the spectral data from the framegrabber and copies it to the #RawDataHandle
	getRawData(Dev, Raw);

	// specifies the output of the processing routine and executes the processing
	setProcessedDataOutput(Proc, BScan);
	executeProcessing(Proc, Raw);

	// stops the measurement
	stopMeasurement(Dev);

	// exports the processed data to a csv-file to the specified folder. For more information about the data export see #ExportDataAndImage
	exportData(BScan, DataExportFormat::DataExport_CSV, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\BScanProcessed.csv");

	// clear up everything 
	clearScanPattern(Pattern);

	clearData(BScan);
	clearRawData(Raw);

	clearProcessing(Proc);
	closeProbe(Probe);
	closeDevice(Dev);

	// check if an error occured and write it to the command prompt
	char message[1024];
	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	// waits to close the command prompt until an input from the user is done
	(void) getchar();
}

void ExportDataAndImage()
{
	char message[1024];

	OCTDeviceHandle Dev = initDevice();
	ProbeHandle Probe = initProbe(Dev, "Probe");
	ProcessingHandle Proc = createProcessingForDevice(Dev);

	RawDataHandle Raw = createRawData();
	DataHandle BScan = createData();

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		_getch();
		return;
	}

	ScanPatternHandle Pattern = createBScanPattern(Probe, 2.0, 1024);

	startMeasurement(Dev, Pattern, Acquisition_AsyncFinite);

	getRawData(Dev, Raw);

	setProcessedDataOutput(Proc, BScan);
	executeProcessing(Proc, Raw);

	stopMeasurement(Dev);

	// Exports the processed data to a csv-file to the specified folder. Several different export formats are available, see #DataExportFormat
	exportData(BScan, DataExportFormat::DataExport_CSV, "C:\\test_oct_data.csv");

	// The OCT image can be exported as an image in common image format as well. It needs to be colored for that, e.g. the colormap and boundaries for the coloring need to be defined.
	// #ColoringHandle with specified #ColorScheme, here simple black and white, and #ColoringByteOrder
	ColoringHandle Coloring = createColoring32Bit(ColorScheme_BlackAndWhite, Coloring_RGBA);
	// set the boundaries for the colormap, 0.0 as lower and 70.0 as upper boundary are a good choice normally.
	setColoringBoundaries(Coloring, 0.0, 70.0);
	// Exports the processed data to an image with the specified slice normal direction since this will result in 2D-images.
	// To get the B-scan in one image with depth and scan field as axes for a single B-scan #Direction_3 is chosen.
	exportDataAsImage(BScan, Coloring, ColoredDataExport_JPG, Direction_3, "C:\\test_oct_image.jpg", ExportOption_DrawScaleBar | ExportOption_DrawMarkers | ExportOption_UsePhysicalAspectRatio);

	// The unprocessed data from the detector in #RawDataHandle can be exported as well, here to a binary raw/srm file
	exportRawData(Raw, RawDataExportFormat::RawDataExport_RAW, "C::\\test_raw_data.raw");
	// TODO: warum nicht .srm?

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		_getch();
		return;
	}

	clearScanPattern(Pattern);

	clearData(BScan);
	clearRawData(Raw);

	clearProcessing(Proc);
	closeProbe(Probe);
	closeDevice(Dev);

	_getch();
}

/**
void ExportDataAndImage()
{
	char message[1024];

	OCTDeviceHandle Dev = initDevice();
	ProbeHandle Probe = initProbe(Dev, "Probe");
	ProcessingHandle Proc = createProcessingForDevice(Dev);

	RawDataHandle Raw = createRawData();
	DataHandle BScan = createData();

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	ScanPatternHandle Pattern = createBScanPattern(Probe, 2.0, 1024);

	startMeasurement(Dev, Pattern, Acquisition_AsyncFinite);

	getRawData(Dev, Raw);

	setProcessedDataOutput(Proc, BScan);
	executeProcessing(Proc, Raw);

	stopMeasurement(Dev);

	// Exports the processed data to a csv-file to the specified folder. Several different export formats are available, see #DataExportFormat
	exportData(BScan, DataExportFormat::DataExport_CSV, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\BScanProcessed.csv");

	// The OCT image can be exported as an image in common image format as well. It needs to be colored for that, e.g. the colormap and boundaries for the coloring need to be defined.
	// #ColoringHandle with specified #ColorScheme, here simple black and white, and #ColoringByteOrder
	ColoringHandle Coloring = createColoring32Bit(ColorScheme_BlackAndWhite, Coloring_RGBA);
	// set the boundaries for the colormap, 0.0 as lower and 70.0 as upper boundary are a good choice normally.
	setColoringBoundaries(Coloring, 0.0, 70.0);
	// Exports the processed data to an image with the specified slice normal direction since this will result in 2D-images.
	// To get the B-scan in one image with depth and scan field as axes for a single B-scan #Direction_3 is chosen.
	exportDataAsImage(BScan, Coloring, ColoredDataExport_JPG, Direction_3, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\BScanImage.jpg", ExportOption_DrawScaleBar | ExportOption_DrawMarkers | ExportOption_UsePhysicalAspectRatio);

	// The unprocessed data from the detector in #RawDataHandle can be exported as well, here to a binary raw/srm file
	exportRawData(Raw, RawDataExportFormat::RawDataExport_SRR, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\BScanRawSRR.csv");
	exportRawData(Raw, RawDataExportFormat::RawDataExport_RAW, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\BScan.raw");
	// TODO: warum nicht .srm?

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	clearScanPattern(Pattern);

	clearData(BScan);
	clearRawData(Raw);

	clearProcessing(Proc);
	closeProbe(Probe);
	closeDevice(Dev);

	(void) getchar();
}
**/
void AveragingAndImagingSpeed()
{
	char message[1024];

	OCTDeviceHandle Dev = initDevice();
	ProbeHandle Probe = initProbe(Dev, "Probe");
	ProcessingHandle Proc = createProcessingForDevice(Dev);

	RawDataHandle Raw = createRawData();
	DataHandle BScan = createData();

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	// The scan speed of SD-OCT systems can be changed. A better image quality can be obtained with a longer integration time and therefore lower scan speed.
	// Preset 0 is the default scan speed followed by the highest. Please note to adjust the reference intensity on your scanner manually.
	// The number of available device presets can be obtained with #getNumberOfDevicePresets and the description of each preset with #getDevicePresetDescription
	int NumberOfDevicePresets = getNumberOfDevicePresets(Dev, 0);
	cout << getDevicePresetDescription(Dev, 0, 0) << endl;
	setDevicePreset(Dev, 0, Probe, Proc, 0);

	// Another possibilty to modify the image quality is averaging. In the SDK is the data acquisition and processing separated. 
	// Therefore the adjustment of averaging parameters need to be done for both parts.
	// With #setProbeParameterInt the averaging for the data acquisiton can be specified.
	int AScanAveraging = 2;
	int BScanAveraging = 3;
	setProbeParameterInt(Probe, Probe_Oversampling, AScanAveraging); // this results in a repetition of each scan point in the B-scan
	setProbeParameterInt(Probe, Probe_Oversampling_SlowAxis, BScanAveraging); // this results in a repetition of each B-scan in the pattern
	// With #setProcessingParameterInt the averaging parameter for the processing routine can be adjusted. 
	// Please pay attention to match the averaging parameters for acquisition and processing!
	setProcessingParameterInt(Proc, Processing_AScanAveraging, AScanAveraging);
	setProcessingParameterInt(Proc, Processing_BScanAveraging, BScanAveraging);

	ScanPatternHandle Pattern = createBScanPattern(Probe, 2.0, 1024);
	// Please see the documentation of #AcquisitionType to get more information about the different acquisition types
	startMeasurement(Dev, Pattern, Acquisition_AsyncFinite);

	getRawData(Dev, Raw);

	setProcessedDataOutput(Proc, BScan);
	executeProcessing(Proc, Raw);

	stopMeasurement(Dev);

	ColoringHandle Coloring = createColoring32Bit(ColorScheme_BlackAndWhite, Coloring_RGBA);
	setColoringBoundaries(Coloring, 0.0, 70.0);
	exportDataAsImage(BScan, Coloring, ColoredDataExport_JPG, Direction_3, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\test_oct_image.jpg", 0);

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	clearScanPattern(Pattern);

	clearData(BScan);
	clearRawData(Raw);

	clearProcessing(Proc);
	closeProbe(Probe);
	closeDevice(Dev);

	(void) getchar();
}

void VolumeScanPattern()
{
	char message[1024];

	OCTDeviceHandle Dev = initDevice();
	ProbeHandle Probe = initProbe(Dev, "Probe");
	ProcessingHandle Proc = createProcessingForDevice(Dev);

	RawDataHandle RawSlice = createRawData();
	RawDataHandle RawVolume = createRawData();
	DataHandle BScan = createData();
	DataHandle Volume = createData();

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	// Parameter for the volume pattern
	int AScansPerBScan = 256;
	int BScansPerVolume = 16;

	// Averaging parameter
	int BScanAveraging = 3;
	setProbeParameterInt(Probe, Probe_Oversampling_SlowAxis, BScanAveraging);
	setProcessingParameterInt(Proc, Processing_BScanAveraging, BScanAveraging);

	// Several different scan pattern, e.g. for multiple A-scans at one position, B-scans as a line or circle or volumes can be created.
	// For more information please see region #ScanPattern in the documentation.
	// Creating a volume scan pattern requires an additional input parameter for the type of the acquisition order which specifies how to grab the raw data. 
	// With #ScanPattern_AcqOrderAll the whole data of the scan pattern will be returned by calling the function #getRawData once.
	ScanPatternHandle Pattern = createVolumePattern(Probe, 2.0, AScansPerBScan, 2.0, BScansPerVolume, ScanPattern_ApoEachBScan, ScanPattern_AcqOrderAll);
	startMeasurement(Dev, Pattern, Acquisition_AsyncFinite);
	getRawData(Dev, RawVolume);

	setProcessedDataOutput(Proc, Volume);
	executeProcessing(Proc, RawVolume);

	stopMeasurement(Dev);

	// With #ScanPattern_AcqOrderFrameByFrame the data of the scan pattern will be returned slice by slice by calling the function #getRawData.
	// To get the data of the whole volume the function #getRawData need to be called BScansPerVolume * Probe_Oversampling_SlowAxis times.
	Pattern = createVolumePattern(Probe, 2.0, AScansPerBScan, 2.0, BScansPerVolume, ScanPattern_ApoEachBScan, ScanPattern_AcqOrderFrameByFrame);
	startMeasurement(Dev, Pattern, Acquisition_AsyncFinite);

	for (int i = 0; i < BScansPerVolume * BScanAveraging; ++i)
	{
		getRawData(Dev, RawSlice);

		setProcessedDataOutput(Proc, BScan);
		executeProcessing(Proc, RawSlice);

		// To collect the data of the whole volume the data can be added slice by slice to one #DataHandle
		appendData(Volume, BScan, Direction_3);

		// To collect the raw data of the whole volume the data can be added slice by slice to one #RawDataHandle
		appendRawData(RawVolume, RawSlice, Direction_3);
	}

	stopMeasurement(Dev);

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	clearScanPattern(Pattern);

	clearData(BScan);
	clearData(Volume);
	clearRawData(RawSlice);
	clearRawData(RawVolume);

	clearProcessing(Proc);
	closeProbe(Probe);
	closeDevice(Dev);

	(void) getchar();
}

void ModifyScanPatterns()
{
	char message[1024];

	OCTDeviceHandle Dev = initDevice();
	ProbeHandle Probe = initProbe(Dev, "Probe");
	ProcessingHandle Proc = createProcessingForDevice(Dev);

	RawDataHandle Raw = createRawData();
	DataHandle BScan = createData();

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	// All scan patterns are created as horizontal scans centered at (0.0, 0.0) normally. With the following functions you can shift, rotate and zoom the specified pattern.
	// The modifications of a scan pattern (rotation and zoom) are all around the optical center which is (0,0) in mm. Therefore the rotation and zoom should be applied before #shiftScanPattern is used.
	ScanPatternHandle Pattern = createBScanPattern(Probe, 2.0, 1024);

	// Please note that the angle of #rotateScanPattern is in radians
	double AngleDegree = 45;
	rotateScanPattern(Pattern, AngleDegree * 3.14159265 / 180);

	// Increases or decreases the size of the scan pattern aorund the specified #ScanPatternCenter
	double ZoomFactor = 1.5;
	zoomScanPattern(Pattern, ZoomFactor);

	// shifts the specified scan pattern relative to its original position, here (0.0, 0.0), by (ShiftX, ShiftY).
	double ShiftX = 1.0;
	double ShiftY = 2.0;
	shiftScanPattern(Pattern, ShiftX, ShiftY);

	startMeasurement(Dev, Pattern, Acquisition_AsyncFinite);

	getRawData(Dev, Raw);

	setProcessedDataOutput(Proc, BScan);
	executeProcessing(Proc, Raw);

	stopMeasurement(Dev);

	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void) getchar();
		return;
	}

	clearScanPattern(Pattern);

	clearData(BScan);
	clearRawData(Raw);

	clearProcessing(Proc);
	closeProbe(Probe);
	closeDevice(Dev);

	(void) getchar();
}

void AScanAcquisitionAndExportData()
{
	/** STEP-1: initialization of device, probe and processing handles
	1.1 OCTDeviceHandle -> the complete device will be initialized, the SLD will be switched on and all start-up dependent calibration will be performed.
	1.2 ProcessingHandle -> used to create A-scans, B-scans, volume scans out of directly measured spectra.
	1.3 ProbeHandle -> It needs to calibrated to map the suitable output voltages(for analog galvo drivers) or digital values (digital galvo drivers). **/
	OCTDeviceHandle Dev = initDevice();
	ProbeHandle Probe = initProbe(Dev, "Probe_Standard_OCTG_LSM03.ini");
	ProcessingHandle Proc = createProcessingForDevice(Dev);

	/** STEP-2: initialize raw, and processed, data handles	
	2.1 RawDataHandle -> will be used to get the raw data handle and will contain the data from the detector (e.g. line scan camera is SD-OCT systems) without any modification
	2.2 DataHandle -> will be used for the processed data and will contain the OCT image. **/
	RawDataHandle Raw = createRawData();
	DataHandle AScan = createData();

	/** STEP-3: Define Scan Pattern **/
	ScanPatternHandle Pattern = createAScanPattern(Probe, 1, 0, 0);

	/** STEP-4: start the measurement to acquire the specified scan pattern once.	
	4.1 AcquisitionType -> Determines the kind of acquistion process. The type of acquisition process affects 
	e.g. whether consecutive B scans are acquired or if it is possible to lose some data. 
	4.1.1 Acquisition_AsyncContinuous -> Specifies an asynchronous infinite/continuous measurement. With this a
	quisition type an infinite loop to acquire the specified scan pattern will be started  and stopped with the 
	call of stopMeasurment(). Several buffers will be created internally to hold the data of the specified scan 
	pattern several times. With this acquisition mode it is possible to lose data if the acquisition is faster than 
	the copying from the framegarbber with getRawdata(). If you lose data you will always lose a whole frame, 
	e.g. a whole B-scan. The acquisition thread runs independently from the thread for grabbing the data to 
	acquire the data as fast as possible. To get the information whether the data of a whole scan pattern got 
	lost please use getRawDataPropertyInt() wit RawData_LostFramers when grabbing the data.
	4.1.2 Acquisition_AsyncFinite -> Specifies an asychronous finite measurment. With this acquistions type
	enough memory is created internally to holde the data for the whole scan pattern once. Therefore it 
	is guaranteed to grab all the data and not losing frames. Please note that it is possible to acquire the
	scan pattern once only with this acquisition mode.
	4.1.3 Acquisition_Sync -> Specifies an synchronous measurment. With this acquisition mode the acquisition of the
	specified scan pattern will be started with the call of getRawData(). You can interpret this acquisition type
	as a software trigger to start the measurment, To start the data acquisition externally please see the
	chapter in the software manual about external triggering. **/
	startMeasurement(Dev, Pattern, Acquisition_AsyncContinuous);

	/** STEP-5: Grabs the spectral data form the framegrabber and copies it to the RawDataHandle().	**/
	getRawData(Dev, Raw);

	/** STEP-6: Specifies the output of the processing routine and executes the prcoessing. **/
	setProcessedDataOutput(Proc, AScan);
	executeProcessing(Proc, Raw);

	/** STEP-7: Stops the measurement **/
	stopMeasurement(Dev);

	/** STEP-8: Export the data	**/
	exportData(AScan, DataExportFormat::DataExport_CSV, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\AScanProcessed.csv"); 

	// The OCT image can be exported as an image in common image format as well. It needs to be colored for that, e.g. the colormap and boundaries for the coloring need to be defined.
	// #ColoringHandle with specified #ColorScheme, here simple black and white, and #ColoringByteOrder
	//ColoringHandle Coloring = createColoring32Bit(ColorScheme_BlackAndWhite, Coloring_RGBA);
	// set the boundaries for the colormap, 0.0 as lower and 70.0 as upper boundary are a good choice normally.
	//setColoringBoundaries(Coloring, 0.0, 70.0);
	// Exports the processed data to an image with the specified slice normal direction since this will result in 2D-images.
	// To get the B-scan in one image with depth and scan field as axes for a single B-scan #Direction_3 is chosen.
	// exportDataAsImage(AScan, Coloring, ColoredDataExport_JPG, Direction_1, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\test_oct_image.jpg", ExportOption_DrawScaleBar | ExportOption_DrawMarkers | ExportOption_UsePhysicalAspectRatio);
	// The unprocessed data from the detector in #RawDataHandle can be exported as well, here to a binary raw/srm file
	exportRawData(Raw, RawDataExportFormat::RawDataExport_SRR, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\AScanRawSRR.csv");
	exportRawData(Raw, RawDataExportFormat::RawDataExport_RAW, "C:\\Ajay_OCT\\MATLAB\\OCTExportData\\AScanRaw.csv");
	// TODO: warum nicht .srm?

	/** STEP-9: Clear the memory **/
	// Pattern
	clearScanPattern(Pattern);
	// Data
	clearData(AScan);
	clearRawData(Raw);
	// Handles
	clearProcessing(Proc);
	closeProbe(Probe);
	closeDevice(Dev);

	/** STEP-10:  check if an error occured and write it to the command prompt **/
	char message[1024];
	if (getError(message, 1024))
	{
		cout << "ERROR: " << message << endl;
		(void)getchar();
		return;
	}

	/** waits to close the command prompt until an input from the user is done **/
	(void)getchar();
}


int main()
{
	cout << "The following simple demonstration pograms are available: " << endl;
	cout << "a: Single A-scan acquisition and export the data \n";
	cout << "b: Single B-scan acquisition \n";
	cout << "c: Data export as csv and jpg format \n";
	cout << "d: Adjusting image quality by averaging \n";
	cout << "e: Acquisition of a volume pattern with averaging\n";
	cout << "f: Rotation, shifting and zooming of a scan pattern\n";

	char c = getchar();
	switch (c)
	{
	case 'a': AScanAcquisitionAndExportData();
		break;
	case 'b': SimpleMeasurement();
		break;
	case 'c': ExportDataAndImage();
		break;
	case 'd': AveragingAndImagingSpeed();
		break;
	case 'e': VolumeScanPattern();
		break;
	case 'f': ModifyScanPatterns();
		break;
	}
	(void) getchar();
	return 0;
}


