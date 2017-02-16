
#
# CXX Examples
#

sitk_add_test(NAME CXX.Example.DicomSeriesReader
  COMMAND "${ITK_TEST_DRIVER}"
    --compare-MD5
      "${TEST_HARNESS_TEMP_DIRECTORY}/CXX.DicomSeriesReader.nrrd"
      "8e65f711d450b6b466c6d76a667ecb72"
    $<TARGET_FILE:DicomSeriesReader>
      DATA{${SimpleITK_DATA_ROOT}/Input/DicomSeries/,REGEX:Image[0-9]+.dcm}
      "${TEST_HARNESS_TEMP_DIRECTORY}/CXX.DicomSeriesReader.nrrd"
  )
