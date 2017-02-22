set( proj elastix )

get_cmake_property( _varNames VARIABLES )#

if( ${ITK_WRAPPING} OR ${BUILD_SHARED_LIBS} )
  set( ELASTIX_BUILD_SHARED_LIBS ON )
else()
  set( ELASTIX_BUILD_SHARED_LIBS OFF )
endif()

file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${proj}-build/CMakeCacheInit.txt" "${ep_common_cache}" )

set( ELASTIX_GIT_REPOSITORY ${git_protocol}://github.com/kaspermarstal/elastix )
set( ELASTIX_GIT_TAG 75e4e167f3ff31c6cd32cb1c2488971b2adba898 )

if( ${ITK_WRAPPING} OR ${BUILD_SHARED_LIBS} )
  set( ELASTIX_BUILD_SHARED_LIBS ON )
else()
  set( ELASTIX_BUILD_SHARED_LIBS OFF )
  list( APPEND ep_itk_args"-DCMAKE_C_VISIBILITY_PRESET:BOOL=hidden" "-DCMAKE_CXX_VISIBILITY_PRESET:BOOL=hidden" )
endif()

ExternalProject_Add( ${proj} 
  GIT_REPOSITORY ${ELASTIX_GIT_REPOSITORY}
  GIT_TAG ${ELASTIX_GIT_TAG}
  UPDATE_COMMAND ""
  SOURCE_DIR ${proj}
  BINARY_DIR ${proj}-build
  CMAKE_GENERATOR ${gen}
  CMAKE_ARGS
  --no-warn-unused-cli
  -C "${CMAKE_CURRENT_BINARY_DIR}/${proj}-build/CMakeCacheInit.txt"
  ${ep_common_args}
  -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
  -DELASTIX_BUILD_TESTING:BOOL=OFF
  -DELASTIX_BUILD_EXECUTABLE:BOOL=OFF
  -DELASTIX_BUILD_SHARED_LIBS:BOOL=${ELASTIX_BUILD_SHARED_LIBS}
  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  -DITK_DIR:PATH=${ITK_DIR}
  -DELASTIX_IMAGE_2D_PIXELTYPES:STRING=float
  -DELASTIX_IMAGE_3D_PIXELTYPES:STRING=float
  -DELASTIX_IMAGE_4D_PIXELTYPES:STRING=float
  -DUSE_AdaptiveStochasticGradientDescent:BOOL=ON                                           
  -DUSE_AdvancedAffineTransformElastix:BOOL=ON
  -DUSE_AdvancedBSplineTransform:BOOL=ON                                           
  -DUSE_AdvancedKappaStatisticMetric:BOOL=OFF                                           
  -DUSE_AdvancedMattesMutualInformationMetric:BOOL=ON                                           
  -DUSE_AdvancedMeanSquaresMetric:BOOL=ON                                           
  -DUSE_AdvancedNormalizedCorrelationMetric:BOOL=ON                                           
  -DUSE_AffineDTITransformElastix:BOOL=ON
  -DUSE_AffineLogStackTransform:BOOL=${SITK_4D_IMAGES}  
  -DUSE_AffineLogTransformElastix=ON                                        
  -DUSE_BSplineInterpolator:BOOL=ON                                           
  -DUSE_BSplineInterpolatorFloat:BOOL=ON                                           
  -DUSE_BSplineResampleInterpolator:BOOL=ON                                           
  -DUSE_BSplineResampleInterpolatorFloat:BOOL=ON                                           
  -DUSE_BSplineStackTransform:BOOL=${SITK_4D_IMAGES}                                              
  -DUSE_BSplineTransformWithDiffusion:BOOL=OFF                                           
  -DUSE_CMAEvolutionStrategy:BOOL=OFF                                           
  -DUSE_CUDAResampler:BOOL=OFF                                          
  -DUSE_ConjugateGradient:BOOL=OFF                                           
  -DUSE_ConjugateGradientFRPR:BOOL=OFF                                           
  -DUSE_CorrespondingPointsEuclideanDistanceMetric:BOOL=ON
  -DUSE_DeformationFieldTransform:BOOL=ON                                           
  -DUSE_DisplacementMagnitudePenalty:BOOL=ON                                           
  -DUSE_DistancePreservingRigidityPenalty:BOOL=OFF                                           
  -DUSE_EulerTransformElastix:BOOL=ON                                           
  -DUSE_FiniteDifferenceGradientDescent:BOOL=OFF                                           
  -DUSE_FixedGenericPyramid:BOOL=OFF                                       
  -DUSE_FixedRecursivePyramid:BOOL=OFF                                         
  -DUSE_FixedShrinkingPyramid:BOOL=OFF                                           
  -DUSE_FixedSmoothingPyramid:BOOL=ON                                           
  -DUSE_FullSampler:BOOL=OFF                                           
  -DUSE_FullSearch:BOOL=OFF                                           
  -DUSE_GradientDifferenceMetric:BOOL=OFF                                           
  -DUSE_GridSampler:BOOL=ON                                           
  -DUSE_KNNGraphAlphaMutualInformationMetric:BOOL=OFF                                         
  -DUSE_LinearInterpolator:BOOL=ON                                           
  -DUSE_LinearResampleInterpolator:BOOL=ON                                           
  -DUSE_MissingStructurePenalty:BOOL=OFF
  -DUSE_MovingGenericPyramid:BOOL=OFF   
  -DUSE_MovingRecursivePyramid:BOOL=OFF                                           
  -DUSE_MovingShrinkingPyramid:BOOL=OFF                                          
  -DUSE_MovingSmoothingPyramid:BOOL=ON                                           
  -DUSE_MultiBSplineTransformWithNormal:BOOL=OFF                                           
  -DUSE_MultiInputRandomCoordinateSampler:BOOL=OFF                                           
  -DUSE_MultiMetricMultiResolutionRegistration:BOOL=ON                                           
  -DUSE_MultiResolutionRegistration:BOOL=ON                                           
  -DUSE_MultiResolutionRegistrationWithFeatures:BOOL=OFF                                           
  -DUSE_MutualInformationHistogramMetric:BOOL=ON                                           
  -DUSE_MyStandardResampler:BOOL=ON                                           
  -DUSE_NearestNeighborInterpolator:BOOL=ON                                           
  -DUSE_NearestNeighborResampleInterpolator:BOOL=ON                                           
  -DUSE_NormalizedGradientCorrelationMetric:BOOL=ON                                           
  -DUSE_NormalizedMutualInformationMetric:BOOL=ON
  -DUSE_PCAMetric:BOOL=${SITK_4D_IMAGES}   
  -DUSE_PCAMetric2:BOOL=${SITK_4D_IMAGES}                                      
  -DUSE_PatternIntensityMetric:BOOL=OFF                                           
  -DUSE_PolydataDummyPenalty:BOOL=OFF                                           
  -DUSE_Powell:BOOL=OFF
  -DUSE_QuasiNewtonLBFGS:BOOL=ON                                           
  -DUSE_RSGDEachParameterApart:BOOL=OFF                                           
  -DUSE_RandomCoordinateSampler:BOOL=ON                                           
  -DUSE_RandomSampler:BOOL=ON                                           
  -DUSE_RandomSamplerSparseMask:BOOL=ON                                           
  -DUSE_RayCastInterpolator:BOOL=OFF                                           
  -DUSE_RayCastResampleInterpolator:BOOL=OFF                                           
  -DUSE_ReducedDimensionBSplineInterpolator:BOOL=${SITK_4D_IMAGES}                                            
  -DUSE_ReducedDimensionBSplineResampleInterpolator:BOOL=${SITK_4D_IMAGES} 
  -DUSE_RegularStepGradientDescent:BOOL=OFF                                           
  -DUSE_SimilarityTransformElastix:BOOL=ON                                           
  -DUSE_Simplex:BOOL=OFF                                           
  -DUSE_SimultaneousPerturbation:BOOL=OFF                                           
  -DUSE_SplineKernelTransform:BOOL=ON                                           
  -DUSE_StandardGradientDescent:BOOL=OFF                                           
  -DUSE_StatisticalShapePenalty:BOOL=OFF                                           
  -DUSE_TransformBendingEnergyPenanalty:BOOL=ON
  -DUSE_TransformRigidityPenalty:BOOL=ON                                           
  -DUSE_TranslationTransformElastix:BOOL=ON                                           
  -DUSE_VarianceOverLastDimensionMetric:BOOL=${SITK_4D_IMAGES}                                           
  -DUSE_ViolaWellsMutualInformationMetric:BOOL=OFF                                           
  -DUSE_WeightedCombinationTransformElastix:BOOL=OFF
  DEPENDS ${${CMAKE_PROJECT_NAME}_DEPENDENCIES}
)

ExternalProject_Get_Property( elastix BINARY_DIR )
set( ELASTIX_USE_FILE "${BINARY_DIR}/src/UseElastix.cmake" )
