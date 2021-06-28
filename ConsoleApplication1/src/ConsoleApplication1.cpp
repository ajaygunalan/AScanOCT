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
	ProbeHandle Probe = initProbe(Dev, "Probe"); // pass the correct string for configuration file of the appropiate objective which are named normally ProbeLSMxx
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
	exportData(BScan, DataExportFormat::DataExport_CSV, "C:\\test_oct_data.csv");

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
		_getch();
		return;
	}

	// waits to close the command prompt until an input from the user is done
	_getch();
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
		_getch();
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
	exportDataAsImage(BScan, Coloring, ColoredDataExport_JPG, Direction_3, "C:\\test_oct_image.jpg", 0);

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
		_getch();
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
		_getch();
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

	_getch();
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
		_getch();
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

int main()
{
	cout << "The following simple demonstration pograms are available: " << endl;
	cout << "a: Single B-scan acquisition \n";
	cout << "b: Data export as csv and jpg format \n";
	cout << "c: Adjusting image quality by averaging \n";
	cout << "d: Acquisition of a volume pattern with averaging\n";
	cout << "e: Rotation, shifting and zooming of a scan pattern\n";

	char c = _getch();
	switch (c)
	{
	case 'a': SimpleMeasurement();
		break;
	case 'b': ExportDataAndImage();
		break;
	case 'c': AveragingAndImagingSpeed();
		break;
	case 'd': VolumeScanPattern();
		break;
	case 'e': ModifyScanPatterns();
		break;
	}
	_getch();
	return 0;
}


