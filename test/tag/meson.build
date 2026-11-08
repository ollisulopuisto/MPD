test(
  'TestMixRamp',
  executable(
    'TestMixRamp',
    'TestMixRampParser.cxx',
    'TestMixRampInterpolate.cxx',
    include_directories: inc,
    dependencies: [
      log_dep,
      tag_dep,
      util_dep,
      gtest_dep,
    ],
  ),
  protocol: 'gtest',
)

test(
  'TestApeTag',
  executable(
    'TestApeTag',
    'TestApeTag.cxx',
    include_directories: inc,
    dependencies: [
      tag_dep,
      input_glue_dep,
      gtest_dep,
    ],
  ),
  protocol: 'gtest',
)

test(
  'TestTable',
  executable(
    'TestTable',
    'TestTable.cxx',
    include_directories: inc,
    dependencies: [
      tag_dep,
      gtest_dep,
    ],
  ),
  protocol: 'gtest',
)

test(
  'TestParseName',
  executable(
    'TestParseName',
    'TestParseName.cxx',
    include_directories: inc,
    dependencies: [
      tag_dep,
      gtest_dep,
    ],
  ),
  protocol: 'gtest',
)

if xiph_dep.found()
  test(
    'TestScanVorbisComment',
    executable(
      'TestScanVorbisComment',
      'TestScanVorbisComment.cxx',
      include_directories: inc,
      dependencies: [
        xiph_dep,
        tag_dep,
        gtest_dep,
      ],
    ),
    protocol: 'gtest',
  )
endif
