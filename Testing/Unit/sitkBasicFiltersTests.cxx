/*=========================================================================
*
*  Copyright Insight Software Consortium
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*         http://www.apache.org/licenses/LICENSE-2.0.txt
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*=========================================================================*/
#include <SimpleITKTestHarness.h>

#include <sitkImage.h>
#include <sitkImageFileReader.h>
#include <sitkImageFileWriter.h>
#include <sitkHashImageFilter.h>
#include <sitkCastImageFilter.h>
#include <sitkPixelIDValues.h>
#include <sitkStatisticsImageFilter.h>
#include <sitkLabelStatisticsImageFilter.h>
#include <sitkExtractImageFilter.h>
#include <sitkJoinSeriesImageFilter.h>
#include <sitkLabelMapContourOverlayImageFilter.h>
#include <sitkMergeLabelMapFilter.h>
#include <sitkCommand.h>

#include "itkVectorImage.h"
#include "itkVector.h"
#include "itkExtractImageFilter.h"
#include "itkLabelMapContourOverlayImageFilter.h"
#include "itkMergeLabelMapFilter.h"

#include "sitkShow.h"

TEST(BasicFilters,MergeLabelMap_ENUMCHECK) {
  typedef itk::MergeLabelMapFilter< itk::LabelMap< itk::LabelObject<int, 3> > >  ITKType;
  EXPECT_EQ( (int)ITKType::KEEP, (int)itk::simple::MergeLabelMapFilter::Keep);
  EXPECT_EQ( (int)ITKType::AGGREGATE, (int)itk::simple::MergeLabelMapFilter::Aggregate);
  EXPECT_EQ( (int)ITKType::PACK, (int)itk::simple::MergeLabelMapFilter::Pack);
  EXPECT_EQ( (int)ITKType::STRICT, (int)itk::simple::MergeLabelMapFilter::Strict);
}

TEST(BasicFilters,Extract_ENUMCHECK) {
  typedef itk::ExtractImageFilter< itk::Image<float,3>, itk::Image<float,3> > ITKExtractType;
  EXPECT_EQ( (int)ITKExtractType::DIRECTIONCOLLAPSETOUNKOWN, (int)itk::simple::ExtractImageFilter::DIRECTIONCOLLAPSETOUNKOWN );
  EXPECT_EQ( (int)ITKExtractType::DIRECTIONCOLLAPSETOIDENTITY, (int)itk::simple::ExtractImageFilter::DIRECTIONCOLLAPSETOIDENTITY );
  EXPECT_EQ( (int)ITKExtractType::DIRECTIONCOLLAPSETOSUBMATRIX, (int)itk::simple::ExtractImageFilter::DIRECTIONCOLLAPSETOSUBMATRIX );
  EXPECT_EQ( (int)ITKExtractType::DIRECTIONCOLLAPSETOGUESS, (int)itk::simple::ExtractImageFilter::DIRECTIONCOLLAPSETOGUESS );
}
TEST(BasicFilters,LabelMapContourOverlay_ENUMCHECK) {
  typedef itk::LabelMapContourOverlayImageFilter< itk::LabelMap<itk::LabelObject<int,3> >, itk::Image<float,3>, itk::VectorImage<int,3> > ITKType;
  EXPECT_EQ( (int) ITKType::PLAIN, (int)         itk::simple::LabelMapContourOverlayImageFilter::PLAIN );
  EXPECT_EQ( (int) ITKType::CONTOUR, (int)       itk::simple::LabelMapContourOverlayImageFilter::CONTOUR );
  EXPECT_EQ( (int) ITKType::SLICE_CONTOUR, (int) itk::simple::LabelMapContourOverlayImageFilter::SLICE_CONTOUR );


  EXPECT_EQ( (int) ITKType::HIGH_LABEL_ON_TOP, (int) itk::simple::LabelMapContourOverlayImageFilter::HIGH_LABEL_ON_TOP );
  EXPECT_EQ( (int) ITKType::LOW_LABEL_ON_TOP, (int) itk::simple::LabelMapContourOverlayImageFilter::LOW_LABEL_ON_TOP );

}


TEST(BasicFilters,ImageFilter) {
  namespace sitk = itk::simple;

  sitk::CastImageFilter caster;
  sitk::ImageFilter<1> &filter = caster;

  filter.DebugOn();
}

TEST(BasicFilters,ProcessObject_Debug) {
  namespace sitk = itk::simple;

  sitk::CastImageFilter caster;
  sitk::ProcessObject &filter = caster;

  EXPECT_FALSE(filter.GetGlobalDefaultDebug());
  EXPECT_FALSE(filter.GetDebug());

  filter.DebugOff();
  EXPECT_FALSE(filter.GetDebug());
  EXPECT_FALSE(filter.GetGlobalDefaultDebug());

  filter.DebugOn();
  EXPECT_TRUE(filter.GetDebug());
  EXPECT_FALSE(filter.GetGlobalDefaultDebug());

  filter.GlobalDefaultDebugOn();
  EXPECT_TRUE(filter.GetDebug());
  EXPECT_TRUE(filter.GetGlobalDefaultDebug());

  filter.GlobalDefaultDebugOff();
  EXPECT_TRUE(filter.GetDebug());
  EXPECT_FALSE(filter.GetGlobalDefaultDebug());

  filter.GlobalDefaultDebugOn();

  sitk::CastImageFilter caster2;
  EXPECT_TRUE(caster2.GetDebug());
  EXPECT_TRUE(caster2.GetGlobalDefaultDebug());

}

TEST(BasicFilters,ProcessObject_NumberOfThreads) {
  namespace sitk = itk::simple;

  sitk::CastImageFilter caster;
  sitk::ProcessObject &filter = caster;

  unsigned int gNum = filter.GetGlobalDefaultNumberOfThreads();
  EXPECT_NE(filter.GetGlobalDefaultNumberOfThreads(), 0u);
  EXPECT_NE(filter.GetNumberOfThreads(), 0u);
  EXPECT_EQ(filter.GetNumberOfThreads(), filter.GetGlobalDefaultNumberOfThreads());

  filter.SetNumberOfThreads(3);
  EXPECT_EQ(3u, filter.GetNumberOfThreads());
  EXPECT_EQ(gNum, filter.GetGlobalDefaultNumberOfThreads());

  filter.SetGlobalDefaultNumberOfThreads(gNum+1);
  EXPECT_EQ(gNum+1, filter.GetGlobalDefaultNumberOfThreads());
  EXPECT_EQ(3u, filter.GetNumberOfThreads());

  sitk::CastImageFilter caster2;
  EXPECT_EQ(gNum+1, caster2.GetNumberOfThreads());
  EXPECT_EQ(gNum+1, caster2.GetGlobalDefaultNumberOfThreads());
}

TEST(BasicFilters,Cast) {
  itk::simple::HashImageFilter hasher;
  itk::simple::ImageFileReader reader;

  reader.SetFileName ( dataFinder.GetFile ( "Input/RA-Float.nrrd" ) );
  itk::simple::Image image = reader.Execute();
  ASSERT_TRUE ( image.GetITKBase() != NULL );
  hasher.SetHashFunction ( itk::simple::HashImageFilter::MD5 );
  EXPECT_EQ ( "3ccccde44efaa3d688a86e94335c1f16", hasher.Execute ( image ) );

  EXPECT_EQ ( image.GetPixelIDValue(), itk::simple::sitkFloat32 );
  EXPECT_EQ ( image.GetPixelID(), itk::simple::sitkFloat32 );
  EXPECT_EQ ( image.GetPixelIDTypeAsString(), "32-bit float" );

  typedef std::map<std::string,itk::simple::PixelIDValueType> MapType;
  MapType mapping;
  mapping["2f27e9260baeba84fb83dd35de23fa2d"] = (itk::simple::PixelIDValueType)itk::simple::sitkUInt8;
  mapping["2f27e9260baeba84fb83dd35de23fa2d"] = (itk::simple::PixelIDValueType)itk::simple::sitkInt8;
  mapping["a963bd6a755b853103a2d195e01a50d3"] = (itk::simple::PixelIDValueType)itk::simple::sitkUInt16;
  mapping["a963bd6a755b853103a2d195e01a50d3"] = (itk::simple::PixelIDValueType)itk::simple::sitkInt16;
  mapping["6ceea0011178a955b5be2d545d107199"] = (itk::simple::PixelIDValueType)itk::simple::sitkUInt32;
  mapping["6ceea0011178a955b5be2d545d107199"] = (itk::simple::PixelIDValueType)itk::simple::sitkInt32;
  mapping["efa4c3b27349b97b02a64f3d2b5ca9ed"] = (itk::simple::PixelIDValueType)itk::simple::sitkUInt64;
  mapping["efa4c3b27349b97b02a64f3d2b5ca9ed"] = (itk::simple::PixelIDValueType)itk::simple::sitkInt64;
  mapping["3ccccde44efaa3d688a86e94335c1f16"] = (itk::simple::PixelIDValueType)itk::simple::sitkFloat32;
  mapping["ac0228acc17038fd1f1ed28eb2841c73"] = (itk::simple::PixelIDValueType)itk::simple::sitkFloat64;
  mapping["2f27e9260baeba84fb83dd35de23fa2d"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorUInt8;
  mapping["2f27e9260baeba84fb83dd35de23fa2d"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorInt8;
  mapping["a963bd6a755b853103a2d195e01a50d3"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorUInt16;
  mapping["a963bd6a755b853103a2d195e01a50d3"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorInt16;
  mapping["6ceea0011178a955b5be2d545d107199"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorUInt32;
  mapping["6ceea0011178a955b5be2d545d107199"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorInt32;
  mapping["efa4c3b27349b97b02a64f3d2b5ca9ed"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorUInt64;
  mapping["efa4c3b27349b97b02a64f3d2b5ca9ed"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorInt64;
  mapping["3ccccde44efaa3d688a86e94335c1f16"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorFloat32;
  mapping["ac0228acc17038fd1f1ed28eb2841c73"] = (itk::simple::PixelIDValueType)itk::simple::sitkVectorFloat64;
  mapping["sitkLabelUInt8"] = (itk::simple::PixelIDValueType)itk::simple::sitkLabelUInt8;
  mapping["sitkLabelUInt16"] = (itk::simple::PixelIDValueType)itk::simple::sitkLabelUInt16;
  mapping["sitkLabelUInt32"] = (itk::simple::PixelIDValueType)itk::simple::sitkLabelUInt32;
  mapping["sitkLabelUInt64"] = (itk::simple::PixelIDValueType)itk::simple::sitkLabelUInt64;

  bool failed = false;

  // Loop over the map, load each file, and compare the hash value
  for ( MapType::iterator it = mapping.begin(); it != mapping.end(); ++it )
    {
    itk::simple::PixelIDValueType pixelID = it->second;
    std::string hash = it->first;

    std::cerr << std::flush;
    std::cerr << std::flush;
    if ( pixelID == itk::simple::sitkUnknown )
      {
      std::cerr << "Enum value: " << pixelID << " (" << hash << ") is unknown and not instantiated" << std::endl;
      continue;
      }

    std::cerr << "Testing casting to pixelID: " << pixelID << " is " << itk::simple::GetPixelIDValueAsString ( pixelID ) << std::endl;

    try
      {
      itk::simple::CastImageFilter caster;
      itk::simple::Image test = caster.SetOutputPixelType ( pixelID ).Execute ( image );

      hasher.SetHashFunction ( itk::simple::HashImageFilter::MD5 );
      EXPECT_EQ ( hash, hasher.Execute ( test ) ) << "Cast to " << itk::simple::GetPixelIDValueAsString ( pixelID );

      }
    catch ( ::itk::simple::GenericException &e )
      {

      // hashing currently doesn't work for label images
      if ( hash.find( "sitkLabel" ) == 0 )
        {
        std::cerr << "Hashing currently is not implemented for Label Images" << std::endl;
        }
      else
        {
        failed = true;
        std::cerr << "Failed to hash: " << e.what() << std::endl;
        }

      continue;
      }

  }
  EXPECT_FALSE ( failed ) << "Cast failed, or could not take the hash of the imoge";

}

TEST(BasicFilters,HashImageFilter) {
  itk::simple::HashImageFilter hasher;
  std::string out = hasher.ToString();
  EXPECT_TRUE ( out.find("itk::simple::HashImageFilter") != std::string::npos );
  EXPECT_TRUE ( out.find("HashFunction: SHA1") != std::string::npos );
  EXPECT_TRUE ( out.find("Debug:") != std::string::npos );
  EXPECT_TRUE ( out.find("NumberOfThreads:") != std::string::npos );
  EXPECT_EQ ( itk::simple::HashImageFilter::SHA1, hasher.SetHashFunction ( itk::simple::HashImageFilter::SHA1 ).GetHashFunction() );
  EXPECT_EQ ( itk::simple::HashImageFilter::MD5, hasher.SetHashFunction ( itk::simple::HashImageFilter::MD5 ).GetHashFunction() );
}

TEST(BasicFilters,Cast_Commands) {
  // test cast filter with a bunch of commands

  namespace sitk = itk::simple;
  sitk::Image img = sitk::ReadImage( dataFinder.GetFile ( "Input/RA-Short.nrrd" ) );
  EXPECT_EQ ( "a963bd6a755b853103a2d195e01a50d3", sitk::Hash(img, sitk::HashImageFilter::MD5));

  sitk::CastImageFilter caster;
  caster.SetOutputPixelType( sitk::sitkInt32 );

  ProgressUpdate progressCmd(caster);
  caster.AddCommand(sitk::sitkProgressEvent, progressCmd);

  CountCommand abortCmd(caster);
  caster.AddCommand(sitk::sitkAbortEvent, abortCmd);

  CountCommand deleteCmd(caster);
  caster.AddCommand(sitk::sitkDeleteEvent, deleteCmd);

  CountCommand endCmd(caster);
  caster.AddCommand(sitk::sitkEndEvent, endCmd);

  CountCommand iterCmd(caster);
  caster.AddCommand(sitk::sitkIterationEvent, iterCmd);

  CountCommand startCmd(caster);
  caster.AddCommand(sitk::sitkStartEvent, startCmd);

  CountCommand userCmd(caster);
  caster.AddCommand(sitk::sitkUserEvent, userCmd);


  sitk::Image out = caster.Execute(img);
  EXPECT_EQ ( "6ceea0011178a955b5be2d545d107199", sitk::Hash(out, sitk::HashImageFilter::MD5));

  EXPECT_EQ ( 1.0f, caster.GetProgress() );
  EXPECT_EQ ( 1.0f, progressCmd.m_Progress );
  EXPECT_EQ ( 0, abortCmd.m_Count );
  EXPECT_EQ ( 1, deleteCmd.m_Count );
  EXPECT_EQ ( 1, endCmd.m_Count );
  EXPECT_EQ ( 0, iterCmd.m_Count );
  EXPECT_EQ ( 1, startCmd.m_Count );
  EXPECT_EQ ( 0, userCmd.m_Count );

}

TEST(BasicFilters,Statistics_Abort) {
  // test Statistics filter with a bunch of commands

  namespace sitk = itk::simple;
  sitk::Image img = sitk::ReadImage( dataFinder.GetFile ( "Input/RA-Short.nrrd" ) );
  EXPECT_EQ ( "a963bd6a755b853103a2d195e01a50d3", sitk::Hash(img, sitk::HashImageFilter::MD5));

  sitk::StatisticsImageFilter stats;
  stats.SetNumberOfThreads(1);

  AbortAtCommand progressCmd(stats,.05);
  stats.AddCommand(sitk::sitkProgressEvent, progressCmd);

  CountCommand abortCmd(stats);
  stats.AddCommand(sitk::sitkAbortEvent, abortCmd);

  CountCommand deleteCmd(stats);
  stats.AddCommand(sitk::sitkDeleteEvent, deleteCmd);

  CountCommand endCmd(stats);
  stats.AddCommand(sitk::sitkEndEvent, endCmd);

  CountCommand iterCmd(stats);
  stats.AddCommand(sitk::sitkIterationEvent, iterCmd);

  CountCommand startCmd(stats);
  stats.AddCommand(sitk::sitkStartEvent, startCmd);

  CountCommand userCmd(stats);
  stats.AddCommand(sitk::sitkUserEvent, userCmd);

  EXPECT_ANY_THROW( stats.Execute(img) );

  EXPECT_EQ ( 1, abortCmd.m_Count );
  EXPECT_EQ ( 1, deleteCmd.m_Count );
  EXPECT_EQ ( 0, endCmd.m_Count );
  EXPECT_EQ ( 0, iterCmd.m_Count );
  EXPECT_EQ ( 1, startCmd.m_Count );
  EXPECT_EQ ( 0, userCmd.m_Count );

}
