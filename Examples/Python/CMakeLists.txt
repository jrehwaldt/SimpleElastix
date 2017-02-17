sitk_add_python_test( Example.ExtractSlice
  "${CMAKE_CURRENT_SOURCE_DIR}/ExtractSlice.py"
  DATA{${SimpleITK_DATA_ROOT}/Input/OAS1_0001_MR1_mpr-1_anon.nrrd}
  "30"
  "${TEST_HARNESS_TEMP_DIRECTORY}/Python.ExtractSlice.nrrd")

sitk_add_python_test( Example.ReadAndShow
  "${CMAKE_CURRENT_SOURCE_DIR}/ReadAndShow.py"
  DATA{${SimpleITK_DATA_ROOT}/Input/VM1111Shrink-RGB.png} )

sitk_add_python_test( Example.DicomSeriesReader
  "${CMAKE_CURRENT_SOURCE_DIR}/DicomSeriesReader.py"
  --compare-MD5
  "${TEST_HARNESS_TEMP_DIRECTORY}/DicomSeriesReader.nrrd"
  "8e65f711d450b6b466c6d76a667ecb72"
    DATA{${SimpleITK_DATA_ROOT}/Input/DicomSeries/,REGEX:Image[0-9]+.dcm}
    "${TEST_HARNESS_TEMP_DIRECTORY}/DicomSeriesReader.nrrd"
  )

sitk_add_python_test( Example.DicomSeriesReader2
  "${CMAKE_CURRENT_SOURCE_DIR}/DicomSeriesReader2.py"
  --compare-MD5
  "${TEST_HARNESS_TEMP_DIRECTORY}/DicomSeriesReader2.nrrd"
  "8e65f711d450b6b466c6d76a667ecb72"
    DATA{${SimpleITK_DATA_ROOT}/Input/DicomSeries/,REGEX:Image[0-9]+.dcm}
    "${TEST_HARNESS_TEMP_DIRECTORY}/DicomSeriesReader2.nrrd"
  )


sitk_add_python_test( Example.DicomImagePrintTags
  "${CMAKE_CURRENT_SOURCE_DIR}/DicomImagePrintTags.py"
    DATA{${SimpleITK_DATA_ROOT}/Input/DicomSeries/Image0075.dcm}
  )

set_tests_properties( Python.Example.DicomImagePrintTags
  PROPERTIES PASS_REGULAR_EXPRESSION "\\(0008|0060\\) = = \"MR\"")
