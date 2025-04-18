/*
 *****************************************************************
 *                 C O P Y R I G H T  (c) 2020
 *                    All Rights Reserved
 *****************************************************************
 *
 *       THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *                      Zhang Jiaqi.
 *      The copyright notice above does not evidence any
 *     actual or intended publication of such source code.
 *
 *****************************************************************
 *
 *  Author: Zhang Jiaqi
 *
 */

#ifndef IMPORT_THIRD_PARTED_LIBS_3E73D48ED3B0498D971791E3B1786B11
#define IMPORT_THIRD_PARTED_LIBS_3E73D48ED3B0498D971791E3B1786B11

// JsonCPP
#pragma comment(lib, "jsoncpp.lib")

// Dcmtk Libs
#pragma comment(lib, "dcmtk/charls.lib")
#pragma comment(lib, "dcmtk/cmr.lib")
#pragma comment(lib, "dcmtk/dcmdata.lib")
#pragma comment(lib, "dcmtk/dcmdsig.lib")
#pragma comment(lib, "dcmtk/dcmfg.lib")
#pragma comment(lib, "dcmtk/dcmimage.lib")
#pragma comment(lib, "dcmtk/dcmimgle.lib")
#pragma comment(lib, "dcmtk/dcmiod.lib")
#pragma comment(lib, "dcmtk/dcmjpeg.lib")
#pragma comment(lib, "dcmtk/dcmjpls.lib")
#pragma comment(lib, "dcmtk/dcmnet.lib")
#pragma comment(lib, "dcmtk/dcmpmap.lib")
#pragma comment(lib, "dcmtk/dcmpstat.lib")
#pragma comment(lib, "dcmtk/dcmqrdb.lib")
#pragma comment(lib, "dcmtk/dcmrt.lib")
#pragma comment(lib, "dcmtk/dcmseg.lib")
#pragma comment(lib, "dcmtk/dcmsr.lib")
#pragma comment(lib, "dcmtk/dcmtls.lib")
#pragma comment(lib, "dcmtk/dcmtract.lib")
#pragma comment(lib, "dcmtk/dcmwlm.lib")
#pragma comment(lib, "dcmtk/i2d.lib")
#pragma comment(lib, "dcmtk/ijg12.lib")
#pragma comment(lib, "dcmtk/ijg16.lib")
#pragma comment(lib, "dcmtk/ijg8.lib")
#pragma comment(lib, "dcmtk/oflog.lib")
#pragma comment(lib, "dcmtk/ofstd.lib")

// VTK
//#ifndef DEBUG
//	#pragma comment(lib, "vtkChartsCore-9.0.lib")
//	#pragma comment(lib, "vtkCommonColor-9.0.lib")
//	#pragma comment(lib, "vtkCommonComputationalGeometry-9.0.lib")
//	#pragma comment(lib, "vtkCommonCore-9.0.lib")
//	#pragma comment(lib, "vtkCommonDataModel-9.0.lib")
//	#pragma comment(lib, "vtkCommonExecutionModel-9.0.lib")
//	#pragma comment(lib, "vtkCommonMath-9.0.lib")
//	#pragma comment(lib, "vtkCommonMisc-9.0.lib")
//	#pragma comment(lib, "vtkCommonSystem-9.0.lib")
//	#pragma comment(lib, "vtkCommonTransforms-9.0.lib")
//	#pragma comment(lib, "vtkDICOMParser-9.0.lib")
//	#pragma comment(lib, "vtkDomainsChemistry-9.0.lib")
//	#pragma comment(lib, "vtkdoubleconversion-9.0.lib")
//	#pragma comment(lib, "vtkexodusII-9.0.lib")
//	#pragma comment(lib, "vtkexpat-9.0.lib")
//	#pragma comment(lib, "vtkFiltersAMR-9.0.lib")
//	#pragma comment(lib, "vtkFiltersCore-9.0.lib")
//	#pragma comment(lib, "vtkFiltersExtraction-9.0.lib")
//	#pragma comment(lib, "vtkFiltersFlowPaths-9.0.lib")
//	#pragma comment(lib, "vtkFiltersGeneral-9.0.lib")
//	#pragma comment(lib, "vtkFiltersGeneric-9.0.lib")
//	#pragma comment(lib, "vtkFiltersGeometry-9.0.lib")
//	#pragma comment(lib, "vtkFiltersHybrid-9.0.lib")
//	#pragma comment(lib, "vtkFiltersHyperTree-9.0.lib")
//	#pragma comment(lib, "vtkFiltersImaging-9.0.lib")
//	#pragma comment(lib, "vtkFiltersModeling-9.0.lib")
//	#pragma comment(lib, "vtkFiltersParallel-9.0.lib")
//	#pragma comment(lib, "vtkFiltersParallelImaging-9.0.lib")
//	#pragma comment(lib, "vtkFiltersPoints-9.0.lib")
//	#pragma comment(lib, "vtkFiltersProgrammable-9.0.lib")
//	#pragma comment(lib, "vtkFiltersSelection-9.0.lib")
//	#pragma comment(lib, "vtkFiltersSMP-9.0.lib")
//	#pragma comment(lib, "vtkFiltersSources-9.0.lib")
//	#pragma comment(lib, "vtkFiltersStatistics-9.0.lib")
//	#pragma comment(lib, "vtkFiltersTexture-9.0.lib")
//	#pragma comment(lib, "vtkFiltersTopology-9.0.lib")
//	#pragma comment(lib, "vtkFiltersVerdict-9.0.lib")
//	#pragma comment(lib, "vtkfreetype-9.0.lib")
//	#pragma comment(lib, "vtkGeovisCore-9.0.lib")
//	#pragma comment(lib, "vtkgl2ps-9.0.lib")
//	#pragma comment(lib, "vtkglew-9.0.lib")
//	#pragma comment(lib, "vtkhdf5-9.0.lib")
//	#pragma comment(lib, "vtkhdf5_hl-9.0.lib")
//	#pragma comment(lib, "vtkImagingColor-9.0.lib")
//	#pragma comment(lib, "vtkImagingCore-9.0.lib")
//	#pragma comment(lib, "vtkImagingFourier-9.0.lib")
//	#pragma comment(lib, "vtkImagingGeneral-9.0.lib")
//	#pragma comment(lib, "vtkImagingHybrid-9.0.lib")
//	#pragma comment(lib, "vtkImagingMath-9.0.lib")
//	#pragma comment(lib, "vtkImagingMorphological-9.0.lib")
//	#pragma comment(lib, "vtkImagingSources-9.0.lib")
//	#pragma comment(lib, "vtkImagingStatistics-9.0.lib")
//	#pragma comment(lib, "vtkImagingStencil-9.0.lib")
//	#pragma comment(lib, "vtkInfovisCore-9.0.lib")
//	#pragma comment(lib, "vtkInfovisLayout-9.0.lib")
//	#pragma comment(lib, "vtkInteractionImage-9.0.lib")
//	#pragma comment(lib, "vtkInteractionStyle-9.0.lib")
//	#pragma comment(lib, "vtkInteractionWidgets-9.0.lib")
//	#pragma comment(lib, "vtkIOAMR-9.0.lib")
//	#pragma comment(lib, "vtkIOAsynchronous-9.0.lib")
//	#pragma comment(lib, "vtkIOCityGML-9.0.lib")
//	#pragma comment(lib, "vtkIOCore-9.0.lib")
//	#pragma comment(lib, "vtkIOEnSight-9.0.lib")
//	#pragma comment(lib, "vtkIOExodus-9.0.lib")
//	#pragma comment(lib, "vtkIOExport-9.0.lib")
//	#pragma comment(lib, "vtkIOExportGL2PS-9.0.lib")
//	#pragma comment(lib, "vtkIOExportPDF-9.0.lib")
//	#pragma comment(lib, "vtkIOGeometry-9.0.lib")
//	#pragma comment(lib, "vtkIOImage-9.0.lib")
//	#pragma comment(lib, "vtkIOImport-9.0.lib")
//	#pragma comment(lib, "vtkIOInfovis-9.0.lib")
//	#pragma comment(lib, "vtkIOLegacy-9.0.lib")
//	#pragma comment(lib, "vtkIOLSDyna-9.0.lib")
//	#pragma comment(lib, "vtkIOMINC-9.0.lib")
//	#pragma comment(lib, "vtkIOMotionFX-9.0.lib")
//	#pragma comment(lib, "vtkIOMovie-9.0.lib")
//	#pragma comment(lib, "vtkIONetCDF-9.0.lib")
//	#pragma comment(lib, "vtkIOOggTheora-9.0.lib")
//	#pragma comment(lib, "vtkIOParallel-9.0.lib")
//	#pragma comment(lib, "vtkIOParallelXML-9.0.lib")
//	#pragma comment(lib, "vtkIOPLY-9.0.lib")
//	#pragma comment(lib, "vtkIOSegY-9.0.lib")
//	#pragma comment(lib, "vtkIOSQL-9.0.lib")
//	#pragma comment(lib, "vtkIOTecplotTable-9.0.lib")
//	#pragma comment(lib, "vtkIOVeraOut-9.0.lib")
//	#pragma comment(lib, "vtkIOVideo-9.0.lib")
//	#pragma comment(lib, "vtkIOXML-9.0.lib")
//	#pragma comment(lib, "vtkIOXMLParser-9.0.lib")
//	#pragma comment(lib, "vtkjpeg-9.0.lib")
//	#pragma comment(lib, "vtkjsoncpp-9.0.lib")
//	#pragma comment(lib, "vtklibharu-9.0.lib")
//	#pragma comment(lib, "vtklibproj-9.0.lib")
//	#pragma comment(lib, "vtklibxml2-9.0.lib")
//	#pragma comment(lib, "vtkloguru-9.0.lib")
//	#pragma comment(lib, "vtklz4-9.0.lib")
//	#pragma comment(lib, "vtklzma-9.0.lib")
//	#pragma comment(lib, "vtkmetaio-9.0.lib")
//	#pragma comment(lib, "vtknetcdf-9.0.lib")
//	#pragma comment(lib, "vtkogg-9.0.lib")
//	#pragma comment(lib, "vtkParallelCore-9.0.lib")
//	#pragma comment(lib, "vtkParallelDIY-9.0.lib")
//	#pragma comment(lib, "vtkpng-9.0.lib")
//	#pragma comment(lib, "vtkpugixml-9.0.lib")
//	#pragma comment(lib, "vtkRenderingAnnotation-9.0.lib")
//	#pragma comment(lib, "vtkRenderingContext2D-9.0.lib")
//	#pragma comment(lib, "vtkRenderingCore-9.0.lib")
//	#pragma comment(lib, "vtkRenderingFreeType-9.0.lib")
//	#pragma comment(lib, "vtkRenderingGL2PSOpenGL2-9.0.lib")
//	#pragma comment(lib, "vtkRenderingImage-9.0.lib")
//	#pragma comment(lib, "vtkRenderingLabel-9.0.lib")
//	#pragma comment(lib, "vtkRenderingLOD-9.0.lib")
//	#pragma comment(lib, "vtkRenderingOpenGL2-9.0.lib")
//	#pragma comment(lib, "vtkRenderingSceneGraph-9.0.lib")
//	#pragma comment(lib, "vtkRenderingUI-9.0.lib")
//	#pragma comment(lib, "vtkRenderingVolume-9.0.lib")
//	#pragma comment(lib, "vtkRenderingVolumeOpenGL2-9.0.lib")
//	#pragma comment(lib, "vtkRenderingVtkJS-9.0.lib")
//	#pragma comment(lib, "vtksqlite-9.0.lib")
//	#pragma comment(lib, "vtksys-9.0.lib")
//	#pragma comment(lib, "vtkTestingRendering-9.0.lib")
//	#pragma comment(lib, "vtktheora-9.0.lib")
//	#pragma comment(lib, "vtktiff-9.0.lib")
//	#pragma comment(lib, "vtkverdict-9.0.lib")
//	#pragma comment(lib, "vtkViewsContext2D-9.0.lib")
//	#pragma comment(lib, "vtkViewsCore-9.0.lib")
//	#pragma comment(lib, "vtkViewsInfovis-9.0.lib")
//	#pragma comment(lib, "vtkWrappingTools-9.0.lib")
//	#pragma comment(lib, "vtkzlib-9.0.lib")
//#else
//	#pragma comment(lib, "vtkChartsCore-9.0d.lib")
//	#pragma comment(lib, "vtkCommonColor-9.0d.lib")
//	#pragma comment(lib, "vtkCommonComputationalGeometry-9.0d.lib")
//	#pragma comment(lib, "vtkCommonCore-9.0d.lib")
//	#pragma comment(lib, "vtkCommonDataModel-9.0d.lib")
//	#pragma comment(lib, "vtkCommonExecutionModel-9.0d.lib")
//	#pragma comment(lib, "vtkCommonMath-9.0d.lib")
//	#pragma comment(lib, "vtkCommonMisc-9.0d.lib")
//	#pragma comment(lib, "vtkCommonSystem-9.0d.lib")
//	#pragma comment(lib, "vtkCommonTransforms-9.0d.lib")
//	#pragma comment(lib, "vtkDICOMParser-9.0d.lib")
//	#pragma comment(lib, "vtkDomainsChemistry-9.0d.lib")
//	#pragma comment(lib, "vtkdoubleconversion-9.0d.lib")
//	#pragma comment(lib, "vtkexodusII-9.0d.lib")
//	#pragma comment(lib, "vtkexpat-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersAMR-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersCore-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersExtraction-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersFlowPaths-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersGeneral-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersGeneric-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersGeometry-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersHybrid-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersHyperTree-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersImaging-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersModeling-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersParallel-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersParallelImaging-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersPoints-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersProgrammable-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersSelection-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersSMP-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersSources-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersStatistics-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersTexture-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersTopology-9.0d.lib")
//	#pragma comment(lib, "vtkFiltersVerdict-9.0d.lib")
//	#pragma comment(lib, "vtkfreetype-9.0d.lib")
//	#pragma comment(lib, "vtkGeovisCore-9.0d.lib")
//	#pragma comment(lib, "vtkgl2ps-9.0d.lib")
//	#pragma comment(lib, "vtkglew-9.0d.lib")
//	#pragma comment(lib, "vtkhdf5-9.0d.lib")
//	#pragma comment(lib, "vtkhdf5_hl-9.0d.lib")
//	#pragma comment(lib, "vtkImagingColor-9.0d.lib")
//	#pragma comment(lib, "vtkImagingCore-9.0d.lib")
//	#pragma comment(lib, "vtkImagingFourier-9.0d.lib")
//	#pragma comment(lib, "vtkImagingGeneral-9.0d.lib")
//	#pragma comment(lib, "vtkImagingHybrid-9.0d.lib")
//	#pragma comment(lib, "vtkImagingMath-9.0d.lib")
//	#pragma comment(lib, "vtkImagingMorphological-9.0d.lib")
//	#pragma comment(lib, "vtkImagingSources-9.0d.lib")
//	#pragma comment(lib, "vtkImagingStatistics-9.0d.lib")
//	#pragma comment(lib, "vtkImagingStencil-9.0d.lib")
//	#pragma comment(lib, "vtkInfovisCore-9.0d.lib")
//	#pragma comment(lib, "vtkInfovisLayout-9.0d.lib")
//	#pragma comment(lib, "vtkInteractionImage-9.0d.lib")
//	#pragma comment(lib, "vtkInteractionStyle-9.0d.lib")
//	#pragma comment(lib, "vtkInteractionWidgets-9.0d.lib")
//	#pragma comment(lib, "vtkIOAMR-9.0d.lib")
//	#pragma comment(lib, "vtkIOAsynchronous-9.0d.lib")
//	#pragma comment(lib, "vtkIOCityGML-9.0d.lib")
//	#pragma comment(lib, "vtkIOCore-9.0d.lib")
//	#pragma comment(lib, "vtkIOEnSight-9.0d.lib")
//	#pragma comment(lib, "vtkIOExodus-9.0d.lib")
//	#pragma comment(lib, "vtkIOExport-9.0d.lib")
//	#pragma comment(lib, "vtkIOExportGL2PS-9.0d.lib")
//	#pragma comment(lib, "vtkIOExportPDF-9.0d.lib")
//	#pragma comment(lib, "vtkIOGeometry-9.0d.lib")
//	#pragma comment(lib, "vtkIOImage-9.0d.lib")
//	#pragma comment(lib, "vtkIOImport-9.0d.lib")
//	#pragma comment(lib, "vtkIOInfovis-9.0d.lib")
//	#pragma comment(lib, "vtkIOLegacy-9.0d.lib")
//	#pragma comment(lib, "vtkIOLSDyna-9.0d.lib")
//	#pragma comment(lib, "vtkIOMINC-9.0d.lib")
//	#pragma comment(lib, "vtkIOMotionFX-9.0d.lib")
//	#pragma comment(lib, "vtkIOMovie-9.0d.lib")
//	#pragma comment(lib, "vtkIONetCDF-9.0d.lib")
//	#pragma comment(lib, "vtkIOOggTheora-9.0d.lib")
//	#pragma comment(lib, "vtkIOParallel-9.0d.lib")
//	#pragma comment(lib, "vtkIOParallelXML-9.0d.lib")
//	#pragma comment(lib, "vtkIOPLY-9.0d.lib")
//	#pragma comment(lib, "vtkIOSegY-9.0d.lib")
//	#pragma comment(lib, "vtkIOSQL-9.0d.lib")
//	#pragma comment(lib, "vtkIOTecplotTable-9.0d.lib")
//	#pragma comment(lib, "vtkIOVeraOut-9.0d.lib")
//	#pragma comment(lib, "vtkIOVideo-9.0d.lib")
//	#pragma comment(lib, "vtkIOXML-9.0d.lib")
//	#pragma comment(lib, "vtkIOXMLParser-9.0d.lib")
//	#pragma comment(lib, "vtkjpeg-9.0d.lib")
//	#pragma comment(lib, "vtkjsoncpp-9.0d.lib")
//	#pragma comment(lib, "vtklibharu-9.0d.lib")
//	#pragma comment(lib, "vtklibproj-9.0d.lib")
//	#pragma comment(lib, "vtklibxml2-9.0d.lib")
//	#pragma comment(lib, "vtkloguru-9.0d.lib")
//	#pragma comment(lib, "vtklz4-9.0d.lib")
//	#pragma comment(lib, "vtklzma-9.0d.lib")
//	#pragma comment(lib, "vtkmetaio-9.0d.lib")
//	#pragma comment(lib, "vtknetcdf-9.0d.lib")
//	#pragma comment(lib, "vtkogg-9.0d.lib")
//	#pragma comment(lib, "vtkParallelCore-9.0d.lib")
//	#pragma comment(lib, "vtkParallelDIY-9.0d.lib")
//	#pragma comment(lib, "vtkpng-9.0d.lib")
//	#pragma comment(lib, "vtkpugixml-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingAnnotation-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingContext2D-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingCore-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingFreeType-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingGL2PSOpenGL2-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingImage-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingLabel-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingLOD-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingOpenGL2-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingSceneGraph-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingUI-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingVolume-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingVolumeOpenGL2-9.0d.lib")
//	#pragma comment(lib, "vtkRenderingVtkJS-9.0d.lib")
//	#pragma comment(lib, "vtksqlite-9.0d.lib")
//	#pragma comment(lib, "vtksys-9.0d.lib")
//	#pragma comment(lib, "vtkTestingRendering-9.0d.lib")
//	#pragma comment(lib, "vtktheora-9.0d.lib")
//	#pragma comment(lib, "vtktiff-9.0d.lib")
//	#pragma comment(lib, "vtkverdict-9.0d.lib")
//	#pragma comment(lib, "vtkViewsContext2D-9.0d.lib")
//	#pragma comment(lib, "vtkViewsCore-9.0d.lib")
//	#pragma comment(lib, "vtkViewsInfovis-9.0d.lib")
//	#pragma comment(lib, "vtkWrappingTools-9.0d.lib")
//	#pragma comment(lib, "vtkzlib-9.0d.lib")
//
//#endif

#endif

